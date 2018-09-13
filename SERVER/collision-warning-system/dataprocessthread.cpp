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

bool DataProcessThread::addMessageToDB(const QJsonObject &message){
    int messageId = message.find("id").value().toString().toInt();
    long long timeStamp = message.find("timeStamp").value().toString().toLongLong();
    float speed = message.find("speed").value().toString().toFloat();
    float direction = message.find("direction").value().toString().toFloat();
    double acc = message.find("acc").value().toString().toDouble();
    double lon = message.find("lon").value().toString().toDouble();
    double lat = message.find("lat").value().toString().toDouble();

    QSqlQuery query;
    query.prepare("INSERT INTO message(messageID, speed, direction, acc, lon, lat, timeStamp)" "VALUES(:messageID, :speed, :direction, :acc, :lon, :lat, :timeStamp)");
    query.bindValue(":messageID", messageId);
    query.bindValue(":timeStamp", timeStamp);
    query.bindValue(":speed", speed);
    query.bindValue(":direction", direction);
    query.bindValue(":acc", acc);
    query.bindValue(":lon", lon);
    query.bindValue(":lat", lat);
    if(!query.exec()){
        return false;
    } else {
        return true;
    }

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



/******************************
 *  当线程计时器到时间时
 * ****************************/
void DataProcessThread::timeOutSlot(){
    QList<QJsonObject> jsonArray = isComputed();
    QString message = "队列中的个数：";
    message.append(QString::number(jsonArray.size()));
    emit newLogInfo(message);
    if(4 == jsonArray.size()){
        emit newComputableByAverageFeatures(jsonArray);
    }

    /**********************
     * 先得到当前时间戳
     * 根据时间戳从数据库中查询从此时前向的20内的Message
     * *********************/
//    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
//    long long timeEnd = time.toMSecsSinceEpoch();
//    emit newLogInfo(QString::number(timeEnd));
//    long long timeStart = timeEnd - 20 * 1000;

//    QSqlQuery query;
//    query.prepare("SELECT * FROM message WHERE timeStamp >= timeStart AND timeStamp <= timeEnd");
//    query.bindValue(":timeStart", timeStart);
//    query.bindValue(":timeEnd", timeEnd);
//    if(!query.exec()){
//        return false;
//    } else {
//        return true;
//    }




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

/**
 * 根据车辆发送的信息进行车辆轨迹预测
 *
 * @brief DataProcessThread::computerResult
 * @param messages
 */
void DataProcessThread::computerResult(const QList<QJsonObject> &messages){

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

    //double timeCrash = this->rsuLocation.find("time").value().toString().toDouble();
    double timeCrash = 10000;

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
    RVehicleOne.insert("warning",isCrash);
    RVehicleOne.insert("time",t1);
    RVehicleOne.insert("distance",dist1);

    RVehicleTwo.insert("id",id2);
    RVehicleTwo.insert("warning",isCrash);
    RVehicleTwo.insert("time",t2);
    RVehicleTwo.insert("distance",dist2);


//    printf(QJsonDocument(RVehicleOne).toJson()+ "\n");
//    printf(QJsonDocument(RVehicleTwo).toJson()+ "\n");

    emit sendResult(RVehicleOne);
    emit sendResult(RVehicleTwo);
}


/***********************
 * 方案三
 * 使用线性回归方法
 * **********************/
void DataProcessThread::computerResultByLinearRegression(const QList<QJsonObject> &location){

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
