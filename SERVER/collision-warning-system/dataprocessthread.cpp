#include <dataprocessthread.h>

DataProcessThread::DataProcessThread(const QJsonObject &rsulocation)
{
    this->setRsuLocation(rsulocation);
    this->THRESHOLD = rsuLocation.find("THRESHOLD").value().toString().toDouble();
    this->DISTANCE_THRESHOLD = rsulocation.find("DISTANCE_THRESHOLD").value().toString().toDouble();

    warningStatusOne = false;
    warningStatusTwo = false;
    idOne = ERROR_VALUE;
    idTwo = ERROR_VALUE;

    connect(this, SIGNAL(shouldSendResult(QJsonObject)), this, SLOT(sendOrNotSend(QJsonObject)));
}

DataProcessThread::~DataProcessThread(){
    if(timer != nullptr){
        disconnect(timer, &QTimer::timeout, this, &DataProcessThread::timeOutSlot);
        this->timer->stop();
        this->timer->destroyed();
        this->timer = nullptr;
    }
    disconnect(this, SIGNAL(shouldSendResult(QJsonObject)), this, SLOT(sendOrNotSend(QJsonObject)));
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
 * 设置阈值参数
 * @brief DataProcessThread::setRsuLocation
 * @param rsuLocation
 */
void DataProcessThread::setRsuLocation(const QJsonObject &rsuLocation){
    if (this->isRunning()){  //正在运行
        this->exit();
        this->rsuLocation = rsuLocation;
        this->run();   //重新运行
    } else{
        this->rsuLocation = rsuLocation;
    }
}

/**
 * 得到当前时间戳
 * @brief DataProcessThread::getTimeStamp
 * @return
 */
long long DataProcessThread::getTimeStamp(){
       QDateTime time = QDateTime::currentDateTime();   //获取当前时间
       long long timeStamp = time.toMSecsSinceEpoch();
       return timeStamp;
}

/**
 * 向队列中添加Message
 * @brief DataProcessThread::addMessage
 * @param message
 * @return
 */
bool DataProcessThread::addMessage(const QJsonObject &message){
    bool isSuccess = true;
    int flag = 0;
    int sizeOne = queueVehicleOne.size();
    int sizeTwo = queueVehicleTwo.size();
    int idMessage = message.find("id").value().toString().toInt();
    double lon = message.find("lon").value().toString().toDouble();
    double lat = message.find("lat").value().toString().toDouble();
    long long timeStamp = message.find("timeStamp").value().toString().toLongLong();
    emit newLogInfo("idMessage:");
    emit newLogInfo(QString::number(idMessage));
    if(0 == sizeOne && 0 == sizeTwo){    //两个队列均为空
        queueVehicleOne.enqueue(message); //在队尾添加一个元素
        emit newVehicleOne(lon, lat);
        idOne = idMessage;
        flag = 11;
    } else if(0 != sizeOne && 0 == sizeTwo){  //第二个队列为空
        if(idMessage == queueVehicleOne.head().find("id").value().toString().toInt()){   //idOne message 来了两个，判断时间戳，保留较新的消息
            if (timeStamp > queueVehicleOne.head().find("timeStamp").value().toString().toLongLong()){
                queueVehicleOne.dequeue();
                queueVehicleOne.enqueue(message);
                flag = 21;
            }
            emit newVehicleOne(lon, lat);
        }else{
            queueVehicleTwo.enqueue(message);
            idTwo = idMessage;
            emit newVehicleTwo(lon, lat);
            flag = 22;
        }
    } else if(0 != sizeTwo && 0 == sizeOne){   //第一个队列为空
        if(idMessage != queueVehicleTwo.head().find("id").value().toString().toInt()){
            queueVehicleOne.enqueue(message);
            idOne = idMessage;
            emit newVehicleOne(lon, lat);
            flag = 31;
        }else{                                    //idTwo 来了两个，保留较新的消息
            if(timeStamp > queueVehicleTwo.head().find("timeStamp").value().toString().toLongLong()){
                queueVehicleTwo.dequeue();
                queueVehicleTwo.enqueue(message);
                flag = 32;
            }
            emit newVehicleTwo(lon, lat);
        }
    } else{
        flag = 4;
    }
    emit newLogInfo("flag：");
    emit newLogInfo(QString::number(flag));
    return isSuccess;
}

//bool DataProcessThread::addMessage(const QJsonObject &message){
//    bool isSuccess = true;
//    int flag = 0;
//    int sizeOne = queueVehicleOne.size();
//    int sizeTwo = queueVehicleTwo.size();
//    int idMessage = message.find("id").value().toString().toInt();
//    double lon = message.find("lon").value().toString().toDouble();
//    double lat = message.find("lat").value().toString().toDouble();
//    emit newLogInfo("idMessage:");
//    emit newLogInfo(QString::number(idMessage));
//    if(0 == sizeOne && 0 == sizeTwo){    //两个队列均为空
//        queueVehicleOne.enqueue(message); //在队尾添加一个元素
//        emit newVehicleOne(lon, lat);
//        flag = 11;
//    } else if(0 != sizeOne && 0 == sizeTwo){  //第二个队列为空
//        if(idMessage == queueVehicleOne.head().find("id").value().toString().toInt()){
//            queueVehicleOne.enqueue(message);
//            emit newVehicleOne(lon, lat);
//            flag = 21;
//        }else{
//            queueVehicleTwo.enqueue(message);
//            emit newVehicleTwo(lon, lat);
//            flag = 22;
//        }
//    } else if(0 != sizeTwo && 0 == sizeOne){   //第一个队列为空
//        if(idMessage != queueVehicleTwo.head().find("id").value().toString().toInt()){
//            queueVehicleOne.enqueue(message);
//            emit newVehicleOne(lon, lat);
//            flag = 31;
//        }else{
//            queueVehicleTwo.enqueue(message);
//            emit newVehicleTwo(lon, lat);
//            flag = 32;
//        }
//    } else{                                  //两个队列均不为空
//        if(idMessage == queueVehicleOne.head().find("id").value().toString().toInt()){
//            queueVehicleOne.enqueue(message);
//            emit newVehicleOne(lon, lat);
//            flag = 41;
//        } else if(idMessage == queueVehicleTwo.head().find("id").value().toString().toInt()){
//            queueVehicleTwo.enqueue(message);
//            emit newVehicleTwo(lon, lat);
//            flag = 42;
//        } else{
//            isSuccess = false;
//            flag = 666;
//        }
//    }
//    emit newLogInfo("flag：");
//    emit newLogInfo(QString::number(flag));
//    return isSuccess;
//}


/***********************************************
 * 队列出队
 * ********************************************/
/**
 * 返回是否可以计算, 如果可以，返回包含四个QJsonObject的List
 * @brief DataProcessThread::isComputed
 * @return
 */
//QList<QJsonObject> DataProcessThread::isComputed(){
//    QList<QJsonObject> jsonArray;
//    int sizeOne = queueVehicleOne.size();
//    int sizeTwo = queueVehicleTwo.size();
//    if(sizeOne >= 2 && sizeTwo >= 2){
//        emit newLogInfo("队列各出队两个");
//        //车辆一的队列出队两个
//        jsonArray.append(queueVehicleOne.dequeue());
//        jsonArray.append(queueVehicleOne.dequeue());
//        //车辆二的队列出队两个
//        jsonArray.append(queueVehicleTwo.dequeue());
//        jsonArray.append(queueVehicleTwo.dequeue());
//        emit newLogInfo(QString(QJsonDocument(jsonArray.at(0)).toJson()));
//        emit newLogInfo(QString(QJsonDocument(jsonArray.at(1)).toJson()));
//        emit newLogInfo(QString(QJsonDocument(jsonArray.at(2)).toJson()));
//        emit newLogInfo(QString(QJsonDocument(jsonArray.at(3)).toJson()));
//    }
//    return jsonArray;
//}

QList<QJsonObject> DataProcessThread::isComputed(){
    QList<QJsonObject> jsonArray;
    int sizeOne = queueVehicleOne.size();
    int sizeTwo = queueVehicleTwo.size();
    if(sizeOne >= 1 && sizeTwo >= 1){
        emit newLogInfo("队列各出队一个");
        //车辆一的队列出队1个
        jsonArray.append(queueVehicleOne.dequeue());
        //车辆二的队列出队1个
        jsonArray.append(queueVehicleTwo.dequeue());
        emit newLogInfo(QString(QJsonDocument(jsonArray.at(0)).toJson()));
        emit newLogInfo(QString(QJsonDocument(jsonArray.at(1)).toJson()));
    }
    return jsonArray;
}



/******************************
 *  当线程计时器到时间时
 * ****************************/
void DataProcessThread::timeOutSlot(){
    QList<QJsonObject> jsonArray = isComputed();
    QString message = "队列中的个数：";
    message.append(QString::number(jsonArray.size()));
    emit newLogInfo(message);
    if(2 == jsonArray.size()){
        emit newComputable(jsonArray);
    }

}


/*******************************************
 * 实验实际使用算法
 * ************************************/
/**
 * 根据车辆发送的信息进行车辆轨迹预测
 *
 * @brief DataProcessThread::computerResult
 * @param messages
 */
void DataProcessThread::computerResult(const QList<QJsonObject> &messages){

    QJsonObject nodeOne, nodeTwo;
    QJsonObject resultOne, resultTwo;

    /*************************
     * 对初始点进行判断
     * ***********************/

    nodeOne.insert("lat", messages.at(0).find("lat").value().toString().toDouble());
    nodeOne.insert("lon", messages.at(0).find("lon").value().toString().toDouble());
    nodeTwo.insert("lat", messages.at(1).find("lat").value().toString().toDouble());
    nodeTwo.insert("lon", messages.at(1).find("lon").value().toString().toDouble());

    emit newLogInfo(QString(QJsonDocument(nodeOne).toJson()));
    emit newLogInfo(QString(QJsonDocument(nodeTwo).toJson()));

    double distanceStart = getDistance(nodeOne, nodeTwo);

    if (distanceStart <= DISTANCE_THRESHOLD){
        resultOne.insert("warning", true);
        resultOne.insert("distance", distanceStart);
        resultOne.insert("time", 0);
        resultOne.insert("sendTimeStamp", getTimeStamp());

        resultTwo.insert("warning", true);
        resultTwo.insert("distance", distanceStart);
        resultTwo.insert("time", 0);
        resultTwo.insert("sendTimeStamp", getTimeStamp());

        emit shouldSendResult(resultOne);
        emit shouldSendResult(resultTwo);
        addResultToDB(resultOne, false);
        addResultToDB(resultTwo, false);

        emit newLogInfo("预测结束：会碰撞");
        return;
    }

    /****************************************
     *  提取数据
     * *************************************/
    //提取两辆车的id
    int id1 = messages.at(0).find("id").value().toString().toInt();
    int id2 = messages.at(1).find("id").value().toString().toInt();

    //提取两辆车的速度
    double v1 = messages.at(0).find("speed").value().toString().toDouble()/3.6;
    double v2 = messages.at(1).find("speed").value().toString().toDouble()/3.6;

    //求两辆车的加速度
    double acc1 = messages.at(0).find("acc").value().toString().toDouble();
    double acc2 = messages.at(1).find("acc").value().toString().toDouble();

    //提取两辆车的lat
    double lat1 = messages.at(0).find("lat").value().toString().toDouble();
    double lat2 = messages.at(1).find("lat").value().toString().toDouble();

    //提取两辆车的lon
    double lon1 = messages.at(0).find("lon").value().toString().toDouble();
    double lon2 = messages.at(1).find("lon").value().toString().toDouble();

    //提取两辆车的方向
    double dir1 = messages.at(0).find("direction").value().toString().toDouble();
    double dir2 = messages.at(1).find("direction").value().toString().toDouble();

    dir1=dir1/180*3.14;
    dir2=dir2/180*3.14;

    /*************************************
     * 预测第i秒点的位置并进行评估
     * ***********************************/
    emit newLogInfo("预测开始");

    resultOne.insert("id", id1);
    resultOne.insert("type",TYPE_RESULT);
    resultOne.insert("timeStamp",ERROR_VALUE);
    resultOne.insert("receiverTimeStamp", ERROR_VALUE);

    resultTwo.insert("id", id2);
    resultTwo.insert("type",TYPE_RESULT);
    resultTwo.insert("timeStamp",ERROR_VALUE);
    resultTwo.insert("receiverTimeStamp", ERROR_VALUE);

    double distance;
    for(int i = 1; i <= THRESHOLD; i++){
        nodeOne = Trajectory(i,v1,acc1,dir1,lat1,lon1);
        nodeTwo = Trajectory(i,v2,acc2,dir2,lat2,lon2);
        distance = getDistance(nodeOne, nodeTwo);
        if (distance <= DISTANCE_THRESHOLD){

            resultOne.insert("warning", true);
            resultOne.insert("distance", distance);
            resultOne.insert("time", i);
            resultOne.insert("sendTimeStamp", getTimeStamp());

            resultTwo.insert("warning", true);
            resultTwo.insert("distance", distance);
            resultTwo.insert("time", i);
            resultTwo.insert("sendTimeStamp", getTimeStamp());

            emit shouldSendResult(resultOne);
            emit shouldSendResult(resultTwo);
            addResultToDB(resultOne, false);
            addResultToDB(resultTwo, false);

            emit newLogInfo("预测结束：会碰撞");
            return;

        }
    }
    resultOne.insert("warning", false);
    resultOne.insert("distance", ERROR_VALUE);
    resultOne.insert("time", ERROR_VALUE);
    resultOne.insert("sendTimeStamp", getTimeStamp());

    resultTwo.insert("warning", false);
    resultTwo.insert("distance", ERROR_VALUE);
    resultTwo.insert("time", ERROR_VALUE);
    resultTwo.insert("sendTimeStamp", getTimeStamp());

    emit shouldSendResult(resultOne);
    emit shouldSendResult(resultTwo);
    addResultToDB(resultOne, false);
    addResultToDB(resultTwo, false);

    emit newLogInfo("预测结束：不会碰撞");

}

void DataProcessThread::sendOrNotSend(const QJsonObject result){
    bool warning = result.find("warning").value().toBool();
    int id = result.find("id").value().toInt();
    if(id == idOne){
        if(warningStatusOne ^ warning){
            warningStatusOne = warning;
            emit sendResult(result);
        } else{

        }
    } else if(id == idTwo){
        if(warningStatusTwo ^ warning){
            warningStatusTwo = warning;
            emit sendResult(result);
        } else{

        }
    } else {

    }
}

QJsonObject DataProcessThread::Trajectory(double unittime, double v, double a, double dir, double vlat, double vlon){//车辆的速度、加速度、方向、纬度、经度
    double l,h,d,lon,lat;    //小三角形斜边长,高，底边长，当前所在经纬度
    QJsonObject array;       //每个时间节点的经纬度

    int t0 = unittime;       //时间间隔
    l=(v*t0+1/2*a*pow(t0,2))/100000;    //计算斜边长
    h=l*cos(dir);            //计算纬度差
    d=l*sin(dir);            //计算经度差
    lat=vlat+h;             //计算间隔t0后的纬度
    lon=vlon+d;             //计算间隔t0后的经度

    //将当前经纬度保存进QJsonObject
    array.insert("lat",lat);
    array.insert("lon",lon);

    emit newLogInfo(QJsonDocument(array).toJson()+ "\n");
    return array;

}

double DataProcessThread::getDistance(QJsonObject nodeOne, QJsonObject nodeTwo){
    double latOne = nodeOne.find("lat").value().toDouble();
    double lonOne = nodeOne.find("lon").value().toDouble();

    double latTwo = nodeTwo.find("lat").value().toDouble();
    double lonTwo = nodeTwo.find("lon").value().toDouble();

    double distance = sqrt( pow(latOne * 100000 - latTwo * 100000 , 2) + pow(lonOne * 100000 - lonTwo * 100000, 2));
    emit newLogInfo("Distance is "+ QString::number(distance));
    return distance;
}




/*********************
 * 方案一
 * 使用物理特性，两线相交
 * ********************/
void DataProcessThread::computerResultByAverageFeatures(const QList<QJsonObject> &messages){
    static double DEF_PI180= 0.01745329252; // PI/180.0

    //提取两辆车的id
    int id1=messages.at(0).find("id").value().toString().toInt();
    int id2=messages.at(2).find("id").value().toString().toInt();

    //提取两辆车的速度
    float v11=messages.at(0).find("speed").value().toString().toFloat();
    float v12=messages.at(1).find("speed").value().toString().toFloat();
    float v21=messages.at(2).find("speed").value().toString().toFloat();
    float v22=messages.at(3).find("speed").value().toString().toFloat();
    //求两辆车的平均速度
    float v1=(v11+v12)/2/3.6;  //传过来的速度为KM/H
    float v2=(v21+v22)/2/3.6;

    //提取两辆车的方向
    /***************
     * 0为正北，90正东，-90正西，180或-180正南
     */
    float d11=messages.at(0).find("direction").value().toString().toFloat();
    float d12=messages.at(1).find("direction").value().toString().toFloat();
    float d21=messages.at(2).find("direction").value().toString().toFloat();
    float d22=messages.at(3).find("direction").value().toString().toFloat();
    //求两辆车的平均方向
    float d1=(d11+d12)/2;
    float d2=(d21+d22)/2;
    if(d1 < 0){
        d1 = d1 + 360;
    }
    if(d2 <0){
        d2 = d2 + 360;
    }

    //提取两辆车的加速度
    double acc11=messages.at(0).find("acc").value().toString().toDouble();
    double acc12=messages.at(1).find("acc").value().toString().toDouble();
    double acc21=messages.at(2).find("acc").value().toString().toDouble();
    double acc22=messages.at(3).find("acc").value().toString().toDouble();
    //求两辆车的平均加速度
    double acc1=(acc11+acc12)/2;
    double acc2=(acc21+acc22)/2;

    //提取两辆车的lat
    double lat11=messages.at(0).find("lat").value().toString().toDouble();
    double lat12=messages.at(1).find("lat").value().toString().toDouble();
    double lat21=messages.at(2).find("lat").value().toString().toDouble();
    double lat22=messages.at(3).find("lat").value().toString().toDouble();
    //求两辆车的平均lat
    double lat1=(lat11+lat12)/2;
    double lat2=(lat21+lat22)/2;

    //提取两辆车的lon
    double lon11=messages.at(0).find("lon").value().toString().toDouble();
    double lon12=messages.at(1).find("lon").value().toString().toDouble();
    double lon21=messages.at(2).find("lon").value().toString().toDouble();
    double lon22=messages.at(3).find("lon").value().toString().toDouble();
    //求两辆车的平均lon
    double lon1=(lon11+lon12)/2;
    double lon2=(lon21+lon22)/2;

    //提取两辆车message的时间戳
    long long time11 = messages.at(0).find("timeStamp").value().toString().toLongLong();
    long long time12 = messages.at(1).find("timeStamp").value().toString().toLongLong();
    long long time21 = messages.at(2).find("timeStamp").value().toString().toLongLong();
    long long time22 = messages.at(3).find("timeStamp").value().toString().toLongLong();
    //求平均时间戳
    long long time1 = (time11+time12)/2;
    long long time2 = (time21+time22)/2;

//    printf("time1 = %s\n",QString::number(time1).toStdString().data());
//    printf("time2 = %s\n",QString::number(time2).toStdString().data());
//    printf("time1-time2 = %s\n",QString::number(abs(time1-time2)).toStdString().data());

    double timeCrash = this->rsuLocation.find("THRESHOLD").value().toString().toDouble();
    //double timeCrash = 10000;

    bool isCrash = false;

    QJsonObject RVehicleOne,RVehicleTwo;
    double t1, t2, dist1 , dist2;
    t1 = t2 = dist1 = dist2 = 99999999;

    if(abs(time1-time2) < 1500){ //相隔时间戳小于1.5s
        QJsonObject jsonObject = getDistance(lon1, lat1, lon2, lat2);

//        printf(QJsonDocument(jsonObject).toJson()+ "\n");

        double distance = jsonObject.find("distance").value().toString().toDouble();
        double dx       = jsonObject.find("dx").value().toString().toDouble();
        double dy       = jsonObject.find("dy").value().toString().toDouble();
        double angle    = jsonObject.find("angle").value().toString().toDouble();

//        printf("d1 = %s\n",QString::number(d1).toStdString().data());
//        printf("d2 = %s\n",QString::number(d2).toStdString().data());
//        printf("d1-d2 = %s\n",QString::number(abs(d1-d2)).toStdString().data());
        if((abs(d1-d2) < 30) || (abs(d1-d2) > 330)){
//            printf("number one\n");
            if( (abs((d1+d2)/2-angle) < 30 ) || ( abs((d1+d2)/2-angle) > 330 )){ //共线
//                printf("number one one\n");
                double b = (v1-v2)/2;
                double a = (acc1-acc2)/2;
                double delta = pow(b,2) - 4*a*(-distance);
//                printf("delat = %lf\n", delta);
                if(delta < 0){
                    isCrash = false;
                }else{
                    if(a != 0){
                        double t = (-b+ sqrt(pow(b,2) - a*(-distance)))/a;
//                        printf("t = %lf\n", t);
                        if (t < timeCrash && 0 < t){
                            isCrash = true;
                            t1 = t2 = t;
                            dist1 = v1*t + acc1*pow(t,2)/2;
                            dist2 = v2*t + acc2*pow(t,2)/2;
                        } else{
                            isCrash = false;
                        }
                    } else{
                        double t = distance /(v1-v2);
                        if (t < timeCrash && 0 < t){
                            isCrash = true;
                            t1 = t2 = t;
                            dist1 = v1*t + acc1*pow(t,2)/2;
                            dist2 = v2*t + acc2*pow(t,2)/2;
                        } else{
                            isCrash = false;
                        }
                    }
                }

            } else if((165 < abs((d1+d2)/2-angle)) && (abs((d1+d2)/2-angle) < 195)){
//                printf("number one two\n");
                double b = (v2-v1)/2;
                double a = (acc2-acc1)/2;
                double delta = pow(b,2) - 4*a*(-distance);
//                printf("delat = %lf\n", delta);
                if(delta < 0){
                    isCrash = false;
                }else{
                    if(a != 0){
                        double t = (-b+ sqrt(pow(b,2) - a*(-distance)))/a;
//                        printf("t = %lf\n", t);
                        if (t < timeCrash){
                            isCrash = true;
                            t1 = t2 = t;
                            dist1 = v1*t + acc1*pow(t,2)/2;
                            dist2 = v2*t + acc2*pow(t,2)/2;
                        } else{
                            isCrash = false;
                        }
                    } else{
                        double t = distance /(v2-v1);
                        if (t < timeCrash){
                            isCrash = true;
                            t1 = t2 = t;
                            dist1 = v1*t + acc1*pow(t,2)/2;
                            dist2 = v2*t + acc2*pow(t,2)/2;
                        } else{
                            isCrash = false;
                        }
                    }
                }
            }else{
                isCrash = false;
            }
        } else if((165 < abs(d1-d2)) && (abs(d1-d2) < 195)){
//            printf("number two\n");
            if( abs(d1-angle) < 30 ){
                double b = (v1+v2)/2;
                double a = (acc1+acc2)/2;
                double delta = pow(b,2) - 4*a*(-distance);
//                printf("delta = %lf", delta);
                if(delta < 0){
                    isCrash = false;
                } else{
                    if(a != 0){
                        double t = (-b+ sqrt(pow(b,2) - a*(-distance)))/a;
//                        printf("t = %lf\n", t);
                        if (t < timeCrash){
                            isCrash = true;
                            t1 = t2 = t;
                            dist1 = v1*t + acc1*pow(t,2)/2;
                            dist2 = v2*t + acc2*pow(t,2)/2;
                        } else{
                            isCrash = false;
                        }
                    } else{
                        double t = distance /(v1+v2);
                        if (t < timeCrash){
                            isCrash = true;
                            t1 = t2 = t;
                            dist1 = v1*t + acc1*pow(t,2)/2;
                            dist2 = v2*t + acc2*pow(t,2)/2;
                        } else{
                            isCrash = false;
                        }
                    }
                }
            } else{
                isCrash = false;
            }
        } else{
            double angle1;
            double angle2;
            double x, y;
//            printf("first else\n");
            if(angle < 180){
//                printf("first else one\n");
                if (((angle < d1 && d1 < 180+angle) && (angle < d2 && d2 < 180+angle))
                        || ((d1 < angle || d1 > 180+angle) && (d2 < angle || d2 > 180+angle))){ // 在同一侧
                    if (d1 < 180+angle){
                        angle1 = abs(d1 - angle);
                    } else{
                        angle1 = abs(angle + 360-d1);
                    }
                    if (d2 < 180+angle){
                        angle2 = abs(d2-angle);
                    }else{
                        angle2 = abs(angle + 360-d2);
                    }
                    if(angle1 + angle2 <180){
                        double ddx = abs(dx);
                        double ddy = abs(dy);
                        x = (ddy + ddx*tan(d2*DEF_PI180))/(tan(d1*DEF_PI180) - tan(d2*DEF_PI180));
                        y = tan(d1*DEF_PI180) * x;

                        double distanceCrossOne = sqrt(pow(x,2)+pow(y,2));
                        double distanceCrossTwo = sqrt(pow(abs(dx-x),2)+ pow(abs(dy-y),2));

                        if(isSolved((acc1/2), (v1/2), -distanceCrossOne) && isSolved((acc2/2), (v2/2), -distanceCrossTwo)){
                            double t11 = solveTime((acc1/2), (v1/2), -distanceCrossOne);
                            double t22 = solveTime((acc2/2), (v2/2), -distanceCrossTwo);
                            if (abs(t11 - t22) < timeCrash ){
                                isCrash = true;
                                t1 = t11;
                                t2 = t22;
                                dist1 = distanceCrossOne;
                                dist2 = distanceCrossTwo;
                            } else{
                                isCrash = false;
                            }
                        } else{
                            isCrash = false;
                        }

                    } else{
                        isCrash = false;
                    }
                } else{
                    isCrash = false;
                }
            }else{
//                printf("first else two\n");
//                printf("180+angle-360 = %lf\n", angle-180);
                if (((angle < d1 && d1 < (180+angle-360)) && (angle < d2 && d2 < (180+angle-360)))
                        || ((d1 < angle || d1 > (180+angle-360)) && (d2 < angle || d2 > (180+angle-360)))){ // 在同一侧
                    if (d1 < (180+angle-360)){
                        angle1 = abs(180-abs((angle-180)-d1));
                    } else{
                        angle1 = abs(angle -d1);
                    }
                    if (d2 < 180+angle-360){
                        angle2 = abs(180-abs((angle-180)-d2));
                    }else{
                        angle2 = abs(angle -d2);
                    }
//                    printf("angle1 = %lf\n", angle1);
//                    printf("angle2 = %lf\n", angle2);
                    if(angle1 + angle2 <180){
                        double ddx = abs(dx);
                        double ddy = abs(dy);
                        x = (ddy + ddx*tan(d2*DEF_PI180))/(tan(d1*DEF_PI180) - tan(d2*DEF_PI180));
                        y = tan(d1*DEF_PI180) * x;

                        double distanceCrossOne = sqrt(pow(x,2)+pow(y,2));
                        double distanceCrossTwo = sqrt(pow(abs(dx-x),2)+ pow(abs(dy-y),2));

                        if(isSolved((acc1/2), (v1/2), -distanceCrossOne) && isSolved((acc2/2), (v2/2), -distanceCrossTwo)){
                            double t11 = solveTime((acc1/2), (v1/2), -distanceCrossOne);
                            double t22 = solveTime((acc2/2), (v2/2), -distanceCrossTwo);
                            if (abs(t11 - t22) < timeCrash ){
                                isCrash = true;
                                t1 = t11;
                                t2 = t22;
                                dist1 = distanceCrossOne;
                                dist2 = distanceCrossTwo;
                            } else{
                                isCrash = false;
                            }
                        } else{
                            isCrash = false;
                        }


                    } else{
                        isCrash = false;
                    }
                } else{
                    isCrash = false;
                }
            }

        }
    }

    RVehicleOne.insert("id",id1);
    RVehicleOne.insert("type",2);
    RVehicleOne.insert("warning",isCrash);
    RVehicleOne.insert("time",t1);
    RVehicleOne.insert("distance",dist1);

    RVehicleTwo.insert("id",id2);
    RVehicleTwo.insert("type",2);
    RVehicleTwo.insert("warning",isCrash);
    RVehicleTwo.insert("time",t2);
    RVehicleTwo.insert("distance",dist2);


//    printf(QJsonDocument(RVehicleOne).toJson()+ "\n");
//    printf(QJsonDocument(RVehicleTwo).toJson()+ "\n");

    emit sendResult(RVehicleOne);
    emit sendResult(RVehicleTwo);
}

bool DataProcessThread::isSolved(double a, double b, double c){
    double delta = pow(b,2) - 4*a*c;
//    printf("delta = %lf\n", delta);
    if(delta < 0){
        return false;
    } else{
        return true;
    }
}

QJsonObject DataProcessThread::getDistance(double lon1, double lat1, double lon2, double lat2){
    static double DEF_PI = 3.14159265359; // PI
    static double DEF_2PI= 6.28318530712; // 2*PI
    static double DEF_PI180= 0.01745329252; // PI/180.0
    static double DEF_R =6370693.5; // radius of earth
    //适用于近距离
    double ew1, ns1, ew2, ns2;
    double dx, dy, dew;
    double distance, angle;
    // 角度转换为弧度
    ew1 = lon1 * DEF_PI180;
    ns1 = lat1 * DEF_PI180;
    ew2 = lon2 * DEF_PI180;
    ns2 = lat2 * DEF_PI180;
    //经度差
    dew = ew1 - ew2;
    // 若跨东经和西经180 度，进行调整
    if (dew > DEF_PI)
        dew = DEF_2PI - dew;
    else if (dew < -DEF_PI)
        dew = DEF_2PI + dew;
    dx = DEF_R * cos(ns1) * dew; // 东西方向长度(在纬度圈上的投影长度)
    dy = DEF_R * (ns1 - ns2); // 南北方向长度(在经度圈上的投影长度)
    // 勾股定理求斜边长
    distance = sqrt(dx * dx + dy * dy);
    // 根据dx与dy求点二相对于点一以正北为0度的方位角
    if (dx >= 0){
        if (dy >= 0){  //dx与dy均为正数，点二在以点一为原点的坐标系的第三象限
            angle = atan2(dy, dx)/DEF_PI*180;
            angle = 270 - angle;  //angle为以正北为0度顺时针的角度
        } else{        //dx大于0， dy小于0， 点二在以点一为原点的坐标系的第四象限
            angle = atan2(-dy, dx)/DEF_PI*180;
            angle = 270 + angle;
        }
    } else{
        if (dy >= 0){  //dx小于0， dy大于0，点二在以点一为原点的坐标系的第二象限
            angle = atan2(dy, -dx)/DEF_PI*180;
            angle = 90 + angle;
        } else{  //dx与dy均小于0，点二在以点一为原点的坐标系的第一象限
            angle = atan2(-dy, -dx)/DEF_PI*180;
            angle = 90 - angle;
        }
    }
    dx = -dx;
    dy = -dy;
    QJsonObject jsonObject;
    jsonObject.insert("distance", distance);
    jsonObject.insert("angle", angle);
    jsonObject.insert("dx", dx);  //横坐标距离
    jsonObject.insert("dy", dy);  //纵坐标距离
    return jsonObject;
}


bool DataProcessThread::solveTime(double a, double b, double c){
    double time;
    if(a != 0){
        time = (-b+ sqrt(pow(b,2) - a*c))/a;
        printf("t = %lf\n", time);
    } else{
        time = (-c)/(2*b);
        printf("t = %lf\n", time);
    }
    return time;
}

/**************************
 * 方案二
 * 使用离散点来预测
 * *********************/
void DataProcessThread::computerResultByDiscretePoints(const QList<QJsonObject> &messages){
    QList<QJsonObject> result;
    QJsonObject RVehicleOne,RVehicleTwo;
    QList<QJsonObject> tra1,tra2;
    bool flag = true;

    //提取两辆车的id
    int id1=messages.at(0).find("id").value().toString().toInt();
    int id12=messages.at(1).find("id").value().toString().toInt();
    int id2=messages.at(2).find("id").value().toString().toInt();
    int id22=messages.at(3).find("id").value().toString().toInt();

    //提取两辆车的速度
    float v11=messages.at(0).find("speed").value().toString().toFloat();
    float v12=messages.at(1).find("speed").value().toString().toFloat();
    float v21=messages.at(2).find("speed").value().toString().toFloat();
    float v22=messages.at(3).find("speed").value().toString().toFloat();
    //求两辆车的平均速度
    float v1=(v11+v12)/2/3.6;  //传过来的速度为KM/H
    float v2=(v21+v22)/2/3.6;

    //提取两辆车的方向
    /***************
     * 0为正北，90正东，-90正西，180或-180正南
     */
    float d11=messages.at(0).find("direction").value().toString().toFloat();
    float d12=messages.at(1).find("direction").value().toString().toFloat();
    float d21=messages.at(2).find("direction").value().toString().toFloat();
    float d22=messages.at(3).find("direction").value().toString().toFloat();

    //求两辆车的平均方向
    float d1=(d11+d12)/2;
    float d2=(d21+d22)/2;
    if(d1 < 0){
        d1 = d1 + 360;
    }
    if(d2 <0){
        d2 = d2 + 360;
    }

    //提取两辆车的加速度
    double acc11=messages.at(0).find("acc").value().toString().toDouble();
    double acc12=messages.at(1).find("acc").value().toString().toDouble();
    double acc21=messages.at(2).find("acc").value().toString().toDouble();
    double acc22=messages.at(3).find("acc").value().toString().toDouble();
    //求两辆车的平均加速度
    double acc1=(acc11+acc12)/2;
    double acc2=(acc21+acc22)/2;

    //提取两辆车的lat
    double lat11=messages.at(0).find("lat").value().toString().toDouble();
    double lat12=messages.at(1).find("lat").value().toString().toDouble();
    double lat21=messages.at(2).find("lat").value().toString().toDouble();
    double lat22=messages.at(3).find("lat").value().toString().toDouble();

    //求两辆车的平均lat（NTU)
    double lat1=(lat11+lat12)/2;
    double lat1_NTU=lat1*100000;
    double lat2=(lat21+lat22)/2;
    double lat2_NTU=lat2*100000;

    //提取两辆车的lon
    double lon11=messages.at(0).find("lon").value().toString().toDouble();
    double lon12=messages.at(1).find("lon").value().toString().toDouble();
    double lon21=messages.at(2).find("lon").value().toString().toDouble();
    double lon22=messages.at(3).find("lon").value().toString().toDouble();

    //求两辆车的平均lon（NTU)
    double lon1=(lon11+lon12)/2;
    double lon1_NTU=lon1*100000;
    double lon2=(lon21+lon22)/2;
    double lon2_NTU=lon2*100000;

//    //提取两辆车message的时间戳
//    long long time11 = messages.at(0).find("timeStamp").value().toString().toLongLong();
//    long long time12 = messages.at(1).find("timeStamp").value().toString().toLongLong();
//    long long time21 = messages.at(2).find("timeStamp").value().toString().toLongLong();
//    long long time22 = messages.at(3).find("timeStamp").value().toString().toLongLong();
//    //求平均时间戳
//    long long time1 = (time11+time12)/2;
//    long long time2 = (time21+time22)/2;
    double dist=sqrt(pow(lat1_NTU-lat2_NTU,2)+pow(lon1_NTU-lon2_NTU,2));


    //判断输入数据是否出错
    if ((id1 != id12) || (id2 != id22)){//两组数据id不同
        flag=false;
    }

    if (abs(v11-v12)>Velocity || abs(v21-v22)>Velocity){//两组数据速度相差太大
        flag=false;
    }

    if (abs(acc11-acc12)>Acceleration || abs(acc21-acc22)>Acceleration){//两组数据加速度相差太大
        flag=false;
    }

    if (abs(lat11-lat12)>LAT || abs(lat21-lat22)>LAT){//两组数据纬度相差太大
        flag=false;
    }

    if (abs(lon11-lon12)>LON || abs(lon21-lon22)>LON){//两组数据经度相差太大
        flag=false;
    }

    //输入数据正确，求轨迹并判断是否碰撞
    if (flag==true){
        d1=d1/180*3.14;
        d2=d2/180*3.14;
        tra1=Trajectory(v1,acc1,d1,lat1,lon1);
        tra2=Trajectory(v2,acc2,d2,lat2,lon2);

        result=Judgment(id1,id2,lat1,lon1,lat2,lon2,tra1,tra2); //判断是否相撞
        emit sendResult(result.at(0));
        emit sendResult(result.at(1));
        addResultToDB(result.at(0), false);
        addResultToDB(result.at(1), false);
    }
    else{//输入数据有误
//        RVehicleOne.insert("id",id1);
//        RVehicleOne.insert("error",flag);

//        RVehicleTwo.insert("id",id2);
//        RVehicleTwo.insert("error",flag);

//        result.append(RVehicleOne);
//        result.append(RVehicleTwo);
    }

    //return result;


}

QList<QJsonObject> DataProcessThread::Trajectory(double v, double a, double dir, double vlat, double vlon){
    double l,h,d,lon,lat;    //小三角形斜边长,高，底边长，当前所在经纬度
    QJsonObject array;       //每个时间节点的经纬度
    QList<QJsonObject> Tra;  //行驶轨迹

    int t0=UNITTIME;       //时间间隔
    while (t0<= THRESHOLD) {//求一段时间内的轨迹坐标
        l=(v*t0+1/2*a*pow(t0,2))/100000;    //计算斜边长
        h=l*cos(dir);            //计算纬度差
        d=l*sin(dir);            //计算经度差
        lat=vlat+h;             //计算间隔t0后的纬度
        lon=vlon+d;             //计算间隔t0后的经度

        //将当前经纬度保存进QJsonObject
        array.insert("vlat",lat);
        array.insert("vlon",lon);

        //保存轨迹
        Tra.append(array);

        //更新时间，求下一时间点的经纬度
        t0=t0+UNITTIME;

    }
    return Tra;
}

double DataProcessThread::getDistanceDouble(double lon1, double lat1, double lon2, double lat2){
    static double DEF_PI = 3.14159265359; // PI
    static double DEF_2PI= 6.28318530712; // 2*PI
    static double DEF_PI180= 0.01745329252; // PI/180.0
    static double DEF_R =6370693.5; // radius of earth
    //适用于近距离
    double ew1, ns1, ew2, ns2;
    double dx, dy, dew;
    double distance, angle;
    // 角度转换为弧度
    ew1 = lon1 * DEF_PI180;
    ns1 = lat1 * DEF_PI180;
    ew2 = lon2 * DEF_PI180;
    ns2 = lat2 * DEF_PI180;
    //经度差
    dew = ew1 - ew2;
    // 若跨东经和西经180 度，进行调整
    if (dew > DEF_PI)
        dew = DEF_2PI - dew;
    else if (dew < -DEF_PI)
        dew = DEF_2PI + dew;
    dx = DEF_R * cos(ns1) * dew; // 东西方向长度(在纬度圈上的投影长度)
    dy = DEF_R * (ns1 - ns2); // 南北方向长度(在经度圈上的投影长度)
    // 勾股定理求斜边长
    distance = sqrt(dx * dx + dy * dy);
    return distance;
}

QList<QJsonObject> DataProcessThread::Judgment(int id1, int id2, double lat1,double lon1,double lat2,double lon2,QList<QJsonObject> tra1,QList<QJsonObject> tra2){
    QList<QJsonObject> result;
    QJsonObject RVehicleOne,RVehicleTwo;
    double vlat1,vlon1,vlat2,vlon2;    //纬度、经度
    RVehicleOne.insert("id",id1);
    RVehicleTwo.insert("id",id2);
    QJsonArray vtra1;
    QJsonArray vtra2;
    //jsonArray.add(0, "this is a jsonArray value");
    //jsonArray.add(1, "another jsonArray value");
    //jsonObject.element("jsonArray", jsonArray);

    bool flag=false;
    int t0=UNITTIME;       //时间间隔
    int i=0;    //计数器
    while(flag==false && t0<= THRESHOLD){
        vlat1=tra1.at(i).find("vlat").value().toDouble();
        vlon1=tra1.at(i).find("vlon").value().toDouble();
        vlat2=tra2.at(i).find("vlat").value().toDouble();
        vlon2=tra2.at(i).find("vlon").value().toDouble();
        vtra1.insert(i,tra1.at(i));
        vtra2.insert(i,tra2.at(i));

//        if (fabs(vlat1-vlat2)<lat_THRESHOLD && fabs(vlon1-vlon2)<lon_THRESHOLD){
//            flag=true;

//            RVehicleOne.insert("id",id1);
//            RVehicleOne.insert("warning",flag);
//            RVehicleOne.insert("time",t0);
//            double dist1=sqrt(pow(lat1-vlat1,2)+pow(lon1-vlon1,2))*100000;
//            RVehicleOne.insert("distance",dist1);
//            RVehicleOne.insert("type",2);
//        //    RVehicleOne.insert("trajectory",vtra1);


//            RVehicleTwo.insert("id",id2);
//            RVehicleTwo.insert("warning",flag);
//            RVehicleTwo.insert("time",t0);
//            double dist2=sqrt(pow(lat2-vlat2,2)+pow(lon2-vlon2,2))*100000;
//            RVehicleTwo.insert("distance",dist2);
//            RVehicleTwo.insert("type",2);
//        //    RVehicleTwo.insert("trajectory",vtra2);

//        }


        if (getDistanceDouble(vlon1,vlat1,vlon2,vlat2) < DISTANCE_THRESHOLD || sqrt(pow(vlat1*100000-vlat2*100000,2)+pow(vlon1*100000-vlon2*100000,2)) < DISTANCE_THRESHOLD){
            flag=true;

            RVehicleOne.insert("id",id1);
            RVehicleOne.insert("warning",flag);
            RVehicleOne.insert("time",t0);
            double dist1=sqrt(pow(lat1-vlat1,2)+pow(lon1-vlon1,2))*100000;
            RVehicleOne.insert("distance",dist1);
            RVehicleOne.insert("type",2);
        //    RVehicleOne.insert("trajectory",vtra1);


            RVehicleTwo.insert("id",id2);
            RVehicleTwo.insert("warning",flag);
            RVehicleTwo.insert("time",t0);
            double dist2=sqrt(pow(lat2-vlat2,2)+pow(lon2-vlon2,2))*100000;
            RVehicleTwo.insert("distance",dist2);
            RVehicleTwo.insert("type",2);
        //    RVehicleTwo.insert("trajectory",vtra2);

        }
        t0=t0+UNITTIME;
        i=i+1;
    }

    if (flag==false){
        RVehicleOne.insert("warning",flag);
        RVehicleTwo.insert("warning",flag);
    }
    result.append(RVehicleOne);
    result.append(RVehicleTwo);
    return result;
}


/***********************
 * 方案三
 * 使用线性回归方法
 * **********************/
void DataProcessThread::computerResultByLinearRegression(const QList<QJsonObject> &location){

}

/***************************
 *  将Result 添加到数据库中
 * ************************/
bool DataProcessThread::addResultToDB(const QJsonObject &result, bool sended){
    int resultID = result.find("id").value().toInt();
    int time = result.find("time").value().toInt();
    double distance = result.find("distance").value().toDouble();
    bool warning = result.find("warning").value().toBool();
    long long sendStamp = getTimeStamp();

    QSqlQuery query;
    query.prepare("INSERT INTO result(resultID, time, distance, warning, sendStamp, sended)" "VALUES(:resultID, :time, :distance, :warning, :sendStamp, :sended)");
    query.bindValue(":resultID", resultID);
    query.bindValue(":time", time);
    query.bindValue(":distance", distance);
    query.bindValue(":warning", warning);
    query.bindValue(":sendStamp", sendStamp);
    query.bindValue(":sended", sended);
    if(!query.exec()){
        emit newLogInfo("插入Result表失败"+ query.lastError().text());
        return false;
    } else {
        emit newLogInfo("插入Result表成功");
        return true;
    }
}
