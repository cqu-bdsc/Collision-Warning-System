#include <dataprocessthread.h>

DataProcessThread::DataProcessThread(const QJsonObject &rsulocation)
{
    this->setRsuLocation(rsulocation);
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
 * 设置RSU的位置
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
    emit newLogInfo("idMessage:");
    emit newLogInfo(QString::number(idMessage));
    if(0 == sizeOne && 0 == sizeTwo){    //两个队列均为空
        queueVehicleOne.enqueue(message); //在队尾添加一个元素
        emit newVehicleOne(lon, lat);
        flag = 11;
    } else if(0 != sizeOne && 0 == sizeTwo){  //第二个队列为空
        if(idMessage == queueVehicleOne.head().find("id").value().toString().toInt()){
            queueVehicleOne.enqueue(message);
            emit newVehicleOne(lon, lat);
            flag = 21;
        }else{
            queueVehicleTwo.enqueue(message);
            emit newVehicleTwo(lon, lat);
            flag = 22;
        }
    } else if(0 != sizeTwo && 0 == sizeOne){   //第一个队列为空
        if(idMessage != queueVehicleTwo.head().find("id").value().toString().toInt()){
            queueVehicleOne.enqueue(message);
            emit newVehicleOne(lon, lat);
            flag = 31;
        }else{
            queueVehicleTwo.enqueue(message);
            emit newVehicleTwo(lon, lat);
            flag = 32;
        }
    } else{                                  //两个队列均不为空
        if(idMessage == queueVehicleOne.head().find("id").value().toString().toInt()){
            queueVehicleOne.enqueue(message);
            emit newVehicleOne(lon, lat);
            flag = 41;
        } else if(idMessage == queueVehicleTwo.head().find("id").value().toString().toInt()){
            queueVehicleTwo.enqueue(message);
            emit newVehicleTwo(lon, lat);
            flag = 42;
        } else{
            isSuccess = false;
            flag = 666;
        }
    }
    emit newLogInfo("flag：");
    emit newLogInfo(QString::number(flag));
    return isSuccess;
}

/**
 * 返回是否可以计算, 如果可以，返回包含四个QJsonObject的List
 * @brief DataProcessThread::isComputed
 * @return
 */
QList<QJsonObject> DataProcessThread::isComputed(){
    QList<QJsonObject> jsonArray;
    int sizeOne = queueVehicleOne.size();
    int sizeTwo = queueVehicleTwo.size();
    if(sizeOne >= 2 && sizeTwo >= 2){
        emit newLogInfo("队列各出队两个");
        //车辆一的队列出队两个
        jsonArray.append(queueVehicleOne.dequeue());
        jsonArray.append(queueVehicleOne.dequeue());
        //车辆二的队列出队两个
        jsonArray.append(queueVehicleTwo.dequeue());
        jsonArray.append(queueVehicleTwo.dequeue());
        emit newLogInfo(QString(QJsonDocument(jsonArray.at(0)).toJson()));
        emit newLogInfo(QString(QJsonDocument(jsonArray.at(1)).toJson()));
        emit newLogInfo(QString(QJsonDocument(jsonArray.at(2)).toJson()));
        emit newLogInfo(QString(QJsonDocument(jsonArray.at(3)).toJson()));
    }
    return jsonArray;
}

QList<QList<double>> Trajectory(double t, double v, double a, double rlat, double rlon, double vlat, double vlon){
    double l,h,d;//小三角形斜边长,高，底边长
    double L,H,D;//大三角形斜边长，高，底边长
    const double lat_THRESHOLD = 0.001;//经度的精度
    const double lon_THRESHOLD = 0.001;//纬度的精度
    QList<double> array;
    QList<QList<double>> Tra;
    L=sqrt(pow(rlat-vlat,2)+pow(rlon-vlon,2));
    H=rlon-vlon;
    D=rlat-vlat;
    int t0=t;
    while (abs(rlat-vlat)>lat_THRESHOLD && abs(rlon-vlon)>lon_THRESHOLD) {
        l=v*t+1/2*a*pow(t,2);    //计算斜边长
        h=l/L*H;                 //计算高
        d=l/L*D;                 //计算底边
        vlat=vlat+d;             //计算下t时间后的经度
        vlon=vlon+h;             //计算下t时间后的纬度
        //将当前经纬度保存进QList
        array[0]=vlat;
        array[1]=vlon;
        //保存轨迹
        Tra.append(array);
        //更新时间，求下一时间点的经纬度
        t=t+t0;
    }
    return Tra;
}
/**
 * 根据List计算结果
 * @brief DataProcessThread::ComputerResult
 * @param list
 */
void DataProcessThread::ComputerResult(const QList<QJsonObject> &fourMessages){
    QJsonObject RVehicleOne,RVehicleTwo;
   // QList<QList<double>> tra1,tra2;
    //提取两辆车的id
    int id1=fourMessages.at(0).find("id").value().toString().toInt();
    int id2=fourMessages.at(2).find("id").value().toString().toInt();

    //提取两辆车的速度
    float v11=fourMessages.at(0).find("speed").value().toString().toFloat();
    float v12=fourMessages.at(1).find("speed").value().toString().toFloat();
    float v21=fourMessages.at(2).find("speed").value().toString().toFloat();
    float v22=fourMessages.at(3).find("speed").value().toString().toFloat();
    //求两辆车的平均速度
    float v1=(v11+v12)/2;
    float v2=(v21+v22)/2;

    //提取两辆车的加速度
    double acc11=fourMessages.at(0).find("acc").value().toString().toDouble();
    double acc12=fourMessages.at(1).find("acc").value().toString().toDouble();
    double acc21=fourMessages.at(2).find("acc").value().toString().toDouble();
    double acc22=fourMessages.at(3).find("acc").value().toString().toDouble();
    //求两辆车的平均加速度
    double acc1=(acc11+acc12)/2;
    double acc2=(acc21+acc22)/2;

    //提取两辆车的lat
    double lat11=fourMessages.at(0).find("lat").value().toString().toDouble();
    double lat12=fourMessages.at(1).find("lat").value().toString().toDouble();
    double lat21=fourMessages.at(2).find("lat").value().toString().toDouble();
    double lat22=fourMessages.at(3).find("lat").value().toString().toDouble();

    //求两辆车的平均lat（NTU)
    double lat1=(lat11+lat12)/2;
    double lat1_NTU=lat1*100000;
    double lat2=(lat21+lat22)/2;
    double lat2_NTU=lat2*100000;

    //提取两辆车的lon
    double lon11=fourMessages.at(0).find("lon").value().toString().toDouble();
    double lon12=fourMessages.at(1).find("lon").value().toString().toDouble();
    double lon21=fourMessages.at(2).find("lon").value().toString().toDouble();
    double lon22=fourMessages.at(3).find("lon").value().toString().toDouble();

    //求两辆车的平均lon（NTU)
    double lon1=(lon11+lon12)/2;
    double lon1_NTU=lon1*100000;
    double lon2=(lon21+lon22)/2;
    double lon2_NTU=lon2*100000;

    //提取RSU的lat(NTU)
    double Rlat=this->rsuLocation.find("lat").value().toString().toDouble();
    double Rlat_NTU=Rlat*100000;

    //提取RSU的lon(NTU)
    double Rlon=this->rsuLocation.find("lon").value().toString().toDouble();
    double Rlon_NTU=Rlon*100000;

    double timeCrash = this->rsuLocation.find("time").value().toString().toDouble();

    //求两车与路口的距离
    double dist1=sqrt(pow(lat1_NTU-Rlat_NTU,2)+pow(lon1_NTU-Rlon_NTU,2));
    double dist2=sqrt(pow(lat2_NTU-Rlat_NTU,2)+pow(lon2_NTU-Rlon_NTU,2));

    //求两车可能的碰撞时间
    double t1=(-v1+sqrt(pow(v1,2)+2*acc1*dist1))/acc1;
    double t2=(-v2+sqrt(pow(v2,2)+2*acc2*dist2))/acc2;

    if (abs(t1-t2)<= timeCrash){//碰撞
        RVehicleOne.insert("id",id1);
        RVehicleOne.insert("warning",true);
        RVehicleOne.insert("time",t1);
        RVehicleOne.insert("distance",dist1);
        //求车1的碰撞轨迹
        //tra1=Trajectory(UNITTIME,v1,acc1,Rlat,Rlon,lat1,lon1);
        //RVehicleOne.insert("trajectory",tra1);  //轨迹insert不进去

        RVehicleTwo.insert("id",id2);
        RVehicleTwo.insert("warning",true);
        RVehicleTwo.insert("time",t2);
        RVehicleTwo.insert("distance",dist2);
        //求碰撞轨迹
        //tra2=Trajectory(UNITTIME,v2,acc2,Rlat,Rlon,lat2,lon2);
        //RVehicleTwo.insert("trajectory",tra2);  //轨迹insert不进去


    }else{//安全
        RVehicleOne.insert("id",id1);
        RVehicleOne.insert("warning",false);
        RVehicleOne.insert("distance",dist1);

        RVehicleTwo.insert("id",id2);
        RVehicleTwo.insert("warning",false);
        RVehicleTwo.insert("distance",dist2);
    }
   //将结果保存进result

    //emit sendResult(RVehicleOne);
   // emit sendResult(RVehicleTwo);

    emit newLogInfo(QString(QJsonDocument(RVehicleOne).toJson()));
    emit newLogInfo(QString(QJsonDocument(RVehicleTwo).toJson()));

    emit sendResult(RVehicleOne);
    emit sendResult(RVehicleTwo);

}

void DataProcessThread::timeOutSlot(){
    QList<QJsonObject> jsonArray = isComputed();
    QString message = "队列中的个数：";
    message.append(QString::number(jsonArray.size()));
    emit newLogInfo(message);
    if(4 == jsonArray.size()){
        emit newComputable(jsonArray);
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
            angle = angle + 180;  //angle为以正北为0度顺时针的角度
        } else{        //dx大于0， dy小于0， 点二在以点一为原点的坐标系的第四象限
            angle = atan2(-dy, dx)/DEF_PI*180;
            dy = -dy;
            angle = angle + 270;
        }
    } else{
        if (dy >= 0){  //dx小于0， dy大于0，点二在以点一为原点的坐标系的第二象限
            angle = atan2(dy, -dx)/DEF_PI*180;
            dx = -dx;
            angle = angle + 90;
        } else{  //dx与dy均小于0，点二在以点一为原点的坐标系的第一象限
            angle = atan2(-dy, -dx)/DEF_PI*180;
            dx = -dx;
            dy = -dy;
        }
    }
    QJsonObject jsonObject;
    jsonObject.insert("distance", distance);
    jsonObject.insert("angle", angle);
    jsonObject.insert("dx", dx);  //横坐标距离
    jsonObject.insert("dy", dy);  //纵坐标距离
    return jsonObject;
}

/**
 * 根据车辆发送的信息进行车辆轨迹预测
 *
 * @brief DataProcessThread::computerResult
 * @param messages
 */
void DataProcessThread::computerResult(const QList<QJsonObject> &messages){

}

void DataProcessThread::computerResultByLinearRegression(const QList<QJsonObject> &messages){

}

void DataProcessThread::computerResultByAverageFeatures(const QList<QJsonObject> &messages){
    //提取两辆车的id
    int id1=messages.at(0).find("id").value().toString().toInt();
    int id2=messages.at(2).find("id").value().toString().toInt();

    //提取两辆车的速度
    float v11=messages.at(0).find("speed").value().toString().toFloat();
    float v12=messages.at(1).find("speed").value().toString().toFloat();
    float v21=messages.at(2).find("speed").value().toString().toFloat();
    float v22=messages.at(3).find("speed").value().toString().toFloat();
    //求两辆车的平均速度
    float v1=(v11+v12)/2/3.6;
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

    double timeCrash = this->rsuLocation.find("time").value().toString().toDouble();

    bool isCrash = false;
    double t; //碰撞时间

    QJsonObject RVehicleOne,RVehicleTwo;
    double t1, t2, dist1 , dist2;
    t1 = t2 = dist1 = dist2 = 99999999;

    if(abs(time1-time2) < 1500){ //相隔时间戳小于1.5s
        QJsonObject jsonObject = getDistance(lon1, lat1, lon2, lat2);
        double distance = jsonObject.find("distance").value().toDouble();
        double dx       = jsonObject.find("dx").value().toDouble();
        double dy       = jsonObject.find("dy").value().toDouble();
        double angle    = jsonObject.find("angle").value().toDouble();

        double angle1;
        double angle2;

        double x, y;

        if(0< abs(d1-d2) < 20 || abs(d1-d2) > 340){
            if(0 < abs((d1+d2)/2-angle) < 20 || abs((d1+d2)/2-angle) >340){ //共线
                double v = v1+v2;
                double acc = acc1 +acc2;
                t = (-v + sqrt(pow(v,2) + 2*acc*distance))/acc;
                if (t < timeCrash){
                    isCrash = true;
                    t1 = t2 = t;
                    dist1 = dist2  = distance /2;
                } else{
                    isCrash = false;
                }
            } else{
                isCrash = false;
            }
        } else if(170 < abs(d1-d2) <190){
            if((0< abs(d1-angle) < 20 || abs(d1-angle) > 340) || (0< abs(d2-angle) < 20 || abs(d2-angle) > 340)){
                double v = abs(v1-v2);
                double acc = abs(acc1-acc2);
                t = (-v + sqrt(pow(v,2) + 2*acc*distance))/acc;
                if (t < timeCrash) {
                    isCrash = true;
                    t1 = t2 = t;



                    dist1 = dist2  = distance;
                } else{
                    isCrash = false;
                }
            } else{
                isCrash = false;
            }
        } else{
            if (((angle < d1 < 180+angle) && (angle < d2 < 180+angle)) || ((d1 < angle || d1 > 180+angle) && (d2 < angle || d2 > 180+angle))){ // 在同一侧
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
                    x = (tan(d2)*dy+dx)/((tan(d2)/(tan(d2)))-1);
                    y = x/tan(d1);
                    double distanceCrossOne = sqrt(pow(x,2)+pow(y,2));
                    double distanceCrossTwo = sqrt(pow(abs(dx-x),2)+ pow(abs(dy-y),2));

                    double t11=(-v1+sqrt(pow(v1,2)+2*acc1*distanceCrossOne))/acc1;
                    double t22=(-v2+sqrt(pow(v2,2)+2*acc2*distanceCrossTwo))/acc2;

                    if(abs(t11-t22) < timeCrash){
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
        }
    }

    RVehicleOne.insert("id",id1);
    RVehicleOne.insert("warning",isCrash);
    RVehicleOne.insert("time",t1);
    RVehicleOne.insert("distance",dist1);

    RVehicleTwo.insert("id",id2);
    RVehicleTwo.insert("warning",isCrash);
    RVehicleTwo.insert("time",t2);
    RVehicleTwo.insert("distance",dist2);

}
