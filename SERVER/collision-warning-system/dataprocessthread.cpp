#include <dataprocessthread.h>

DataProcessThread::DataProcessThread()
{
//    queueVehicleOne = new QQueue<QJsonObject>();
//    queueVehicleTwo = new QQueue<QJsonObject>();
    idOne = 0;
    idTwo = 0;
}

DataProcessThread::~DataProcessThread(){
    if(timer != nullptr){
        disconnect(timer, &QTimer::timeout, this, &DataProcessThread::timeOutSlot);
        this->timer->stop();
        this->timer->destroyed();
        this->timer = nullptr;
    }
}

/**
 * 重载run函数，线程运行的入口
 * @brief DataProcessThread::run
 */
void DataProcessThread::run(){
    timer = new QTimer();
    timer->setInterval(FREQUENCY);
    connect(timer, &QTimer::timeout, this, &DataProcessThread::timeOutSlot);
    timer->start();
    this->exec();
}


/**
 * 向队列中添加Message
 * @brief DataProcessThread::addMessage
 * @param message
 * @return
 */
bool DataProcessThread::addMessage(const QJsonObject &message){
    bool isSuccess = true;
    int sizeOne = queueVehicleOne.size();
    int sizeTwo = queueVehicleTwo.size();
    int idMessage = message.find("id").value().toInt();
    if(0 == sizeOne && 0 == sizeTwo){    //两个队列均为空
        idOne = idMessage;
        queueVehicleOne.enqueue(message); //在队尾添加一个元素
    } else if(0 != sizeOne && 0 == sizeTwo){  //第二个队列为空
        if(idMessage != idOne){
            idTwo = idMessage;
            queueVehicleTwo.enqueue(message);
        }else{
            queueVehicleOne.enqueue(message);
        }
    } else if(0 != sizeTwo && 0 == sizeOne){   //第一个队列为空
        if(idMessage != idTwo){
            idOne = idMessage;
            queueVehicleOne.enqueue(message);
        }else{
            queueVehicleTwo.enqueue(message);
        }
    } else{                                  //两个队列均不为空
        if(idMessage == idOne){
            queueVehicleOne.enqueue(message);
        } else if(idMessage == idTwo){
            queueVehicleTwo.enqueue(message);
        } else{
            isSuccess = false;
        }
    }
    return isSuccess;
}

/**
 * 返回是否可以计算, 如果可以，返回包含四个QJsonObject的List
 * @brief DataProcessThread::isComputed
 * @return
 */
QList<QJsonObject> DataProcessThread::isComputed(){
    QList<QJsonObject> list;
    int sizeOne = queueVehicleOne.size();
    int sizeTwo = queueVehicleTwo.size();
    if(sizeOne >= 2 && sizeTwo >= 2){
        //车辆一的队列出队两个
        list.append(queueVehicleOne.dequeue());
        list.append(queueVehicleOne.dequeue());
        //车辆二的队列出队两个
        list.append(queueVehicleTwo.dequeue());
        list.append(queueVehicleTwo.dequeue());
    }
    return list;
}

/**
 * 根据List计算结果
 * @brief DataProcessThread::ComputerResult
 * @param list
 */
void DataProcessThread::ComputerResult(const QList<QJsonObject> &list){
    QJsonObject result;
}

void DataProcessThread::timeOutSlot(){
    QList<QJsonObject> list = isComputed();
    if(0 != list.size()){
        ComputerResult(list);
    }
}

