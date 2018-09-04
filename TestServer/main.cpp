#include <QCoreApplication>
#include <QJsonObject>
#include <QList>
#include <QString>
#include <QJsonDocument>
#include<QDebug>
#include <QDateTime>
#include<QFile>
#include<QTextStream>

QJsonObject getDistance(double lon1, double lat1, double lon2, double lat2){
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
            dx = -dx;
            dy = -dy;
        } else{        //dx大于0， dy小于0， 点二在以点一为原点的坐标系的第四象限
            angle = atan2(-dy, dx)/DEF_PI*180;
//            dy = -dy;
            angle = 270 + angle;
            dx = -dx;
            dy = -dy;
        }
    } else{
        if (dy >= 0){  //dx小于0， dy大于0，点二在以点一为原点的坐标系的第二象限
            angle = atan2(dy, -dx)/DEF_PI*180;
//            dx = -dx;
            angle = 90 + angle;
            dx = -dx;
            dy = -dy;
        } else{  //dx与dy均小于0，点二在以点一为原点的坐标系的第一象限
            angle = atan2(-dy, -dx)/DEF_PI*180;
            angle = 90 - angle;
            dx = -dx;
            dy = -dy;
        }
    }
    QJsonObject jsonObject;
    jsonObject.insert("distance", QString::number(distance));
    jsonObject.insert("angle", QString::number(angle));
    jsonObject.insert("dx", QString::number(dx));  //横坐标距离
    jsonObject.insert("dy", QString::number(dy));  //纵坐标距离
    return jsonObject;
}

bool isSolved(double a, double b, double c){
    double delta = pow(b,2) - 4*a*c;
    printf("delta = %lf\n", delta);
    if(delta < 0){
        return false;
    } else{
        return true;
    }
}

double solveTime(double a, double b, double c){
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

void computerResultByAverageFeatures(const QList<QJsonObject> &messages){
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
    float v1=(v11+v12)/2;
    float v2=(v21+v22)/2;

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

    printf("time1 = %s\n",QString::number(time1).toStdString().data());
    printf("time2 = %s\n",QString::number(time2).toStdString().data());
    printf("time1-time2 = %s\n",QString::number(abs(time1-time2)).toStdString().data());

    //double timeCrash = this->rsuLocation.find("time").value().toString().toDouble();
    double timeCrash = 10000;

    bool isCrash = false;

    QJsonObject RVehicleOne,RVehicleTwo;
    double t1, t2, dist1 , dist2;
    t1 = t2 = dist1 = dist2 = 99999999;

    if(abs(time1-time2) < 1500){ //相隔时间戳小于1.5s
        QJsonObject jsonObject = getDistance(lon1, lat1, lon2, lat2);

        printf(QJsonDocument(jsonObject).toJson()+ "\n");

        double distance = jsonObject.find("distance").value().toString().toDouble();
        double dx       = jsonObject.find("dx").value().toString().toDouble();
        double dy       = jsonObject.find("dy").value().toString().toDouble();
        double angle    = jsonObject.find("angle").value().toString().toDouble();

        printf("d1 = %s\n",QString::number(d1).toStdString().data());
        printf("d2 = %s\n",QString::number(d2).toStdString().data());
        printf("d1-d2 = %s\n",QString::number(abs(d1-d2)).toStdString().data());
        if((abs(d1-d2) < 30) || (abs(d1-d2) > 330)){
            printf("number one\n");
            if( (abs((d1+d2)/2-angle) < 30 ) || ( abs((d1+d2)/2-angle) > 330 )){ //共线
                printf("number one one\n");
                double b = (v1-v2)/2;
                double a = (acc1-acc2)/2;
                double delta = pow(b,2) - 4*a*(-distance);
                printf("delat = %lf\n", delta);
                if(delta < 0){
                    isCrash = false;
                }else{
                    if(a != 0){
                        double t = (-b+ sqrt(pow(b,2) - a*(-distance)))/a;
                        printf("t = %lf\n", t);
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
                printf("number one two\n");
                double b = (v2-v1)/2;
                double a = (acc2-acc1)/2;
                double delta = pow(b,2) - 4*a*(-distance);
                printf("delat = %lf\n", delta);
                if(delta < 0){
                    isCrash = false;
                }else{
                    if(a != 0){
                        double t = (-b+ sqrt(pow(b,2) - a*(-distance)))/a;
                        printf("t = %lf\n", t);
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
            printf("number two\n");
            if( abs(d1-angle) < 30 ){
                double b = (v1+v2)/2;
                double a = (acc1+acc2)/2;
                double delta = pow(b,2) - 4*a*(-distance);
                printf("delta = %lf", delta);
                if(delta < 0){
                    isCrash = false;
                } else{
                    if(a != 0){
                        double t = (-b+ sqrt(pow(b,2) - a*(-distance)))/a;
                        printf("t = %lf\n", t);
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
            printf("first else\n");
            if(angle < 180){
                printf("first else one\n");
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
                    printf("angle1 = %lf\n", angle1);
                    printf("angle2 = %lf\n", angle2);
                    if(angle1 + angle2 <180){
//                        x = (dy + dx*tan(d2*DEF_PI180))/(tan(d1*DEF_PI180) - tan(d2*DEF_PI180));
//                        y = tan(d1*DEF_PI180) * x;

//                        printf("x = %lf\n", x);
//                        printf("y = %lf\n", y);

//                        double distanceCrossOne = sqrt(pow(x,2)+pow(y,2));
//                        double distanceCrossTwo = sqrt(pow(abs(dx-x),2)+ pow(abs(dy-y),2));

                        double ddx = abs(dx);
                        double ddy = abs(dy);
                        x = (ddy + ddx*tan(d2*DEF_PI180))/(tan(d1*DEF_PI180) - tan(d2*DEF_PI180));
                        y = tan(d1*DEF_PI180) * x;

                        printf("x = %lf\n", x);
                        printf("y = %lf\n", y);

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
                printf("first else two\n");
                printf("180+angle-360 = %lf\n", angle-180);
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
                    printf("angle1 = %lf\n", angle1);
                    printf("angle2 = %lf\n", angle2);
                    if(angle1 + angle2 <180){
                        double ddx = abs(dx);
                        double ddy = abs(dy);
                        x = (ddy + ddx*tan(d2*DEF_PI180))/(tan(d1*DEF_PI180) - tan(d2*DEF_PI180));
                        y = tan(d1*DEF_PI180) * x;

                        printf("x = %lf\n", x);
                        printf("y = %lf\n", y);

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

    printf(QJsonDocument(RVehicleOne).toJson()+ "\n");
    printf(QJsonDocument(RVehicleTwo).toJson()+ "\n");


}

void saveFile(const QJsonObject &message){
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString time = currentDateTime.toString();
    QFile file("C://Collision-Warning-System/mess.txt");
    bool ok = file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text );
    if(ok)
    {
        QTextStream out(&file);
        QJsonDocument document;
        document.setObject(message);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);
        QString jsonStr(byteArray);

        out<<time<<"  "<<jsonStr<<endl;//转化成纯文本
        file.close();
    }
    else
    {
        printf("file fail to save");
    }
}

QList<QJsonObject> inputMessage(){

    /***************************
     * 测试例
     * @brief message1
     ****************************/
//    QJsonObject message1;
//    message1.insert("id",       "1");
//    message1.insert("timeStamp","1535507945116");
//    message1.insert("speed",    "10");
//    message1.insert("direction","-140");
//    message1.insert("lat",      "29.5695799110");
//    message1.insert("lon",      "106.4772897844");
//    message1.insert("acc",      "1");

//    QJsonObject message2;
//    message2.insert("id",       "1");
//    message2.insert("timeStamp","1535507946210");
//    message2.insert("speed",    "10");
//    message2.insert("direction","-140");
//    message2.insert("lat",      "29.5695799110");
//    message2.insert("lon",      "106.4772897844");
//    message2.insert("acc",      "1");

//    QJsonObject message3;
//    message3.insert("id",       "2");
//    message3.insert("timeStamp","1535507945116");
//    message3.insert("speed",    "20");
//    message3.insert("direction","-140");
//    message3.insert("lat",      "29.5690899782");
//    message3.insert("lon",      "106.4769518261");
//    message3.insert("acc",      "1");

//    QJsonObject message4;
//    message4.insert("id",       "2");
//    message4.insert("timeStamp","1535507946210");
//    message4.insert("speed",    "20");
//    message4.insert("direction","-140");
//    message4.insert("lat",      "29.5690899782");
//    message4.insert("lon",      "106.4769518261");
//    message4.insert("acc",      "1");


    /************************************
     * 测试例，
     *
     * ************************************/

//    QJsonObject message1;
//    message1.insert("id",       "1");
//    message1.insert("timeStamp","1535507945116");
//    message1.insert("speed",    "10");
//    message1.insert("direction","-140");
//    message1.insert("lat",      "29.5695799110");
//    message1.insert("lon",      "106.4772897844");
//    message1.insert("acc",      "1");

//    QJsonObject message2;
//    message2.insert("id",       "1");
//    message2.insert("timeStamp","1535507946210");
//    message2.insert("speed",    "10");
//    message2.insert("direction","-140");
//    message2.insert("lat",      "29.5695799110");
//    message2.insert("lon",      "106.4772897844");
//    message2.insert("acc",      "1");

//    QJsonObject message3;
//    message3.insert("id",       "2");
//    message3.insert("timeStamp","1535507945116");
//    message3.insert("speed",    "10");
//    message3.insert("direction","40");
//    message3.insert("lat",      "29.5690899782");
//    message3.insert("lon",      "106.4769518261");
//    message3.insert("acc",      "1");

//    QJsonObject message4;
//    message4.insert("id",       "2");
//    message4.insert("timeStamp","1535507946210");
//    message4.insert("speed",    "10");
//    message4.insert("direction","40");
//    message4.insert("lat",      "29.5690899782");
//    message4.insert("lon",      "106.4769518261");
//    message4.insert("acc",      "1");

    /*****************************
     * 测试例，十字路口碰撞
     * ***************************/
        QJsonObject message1;
        message1.insert("id",       "1");
        message1.insert("timeStamp","1535507945116");
        message1.insert("speed",    "5");
        message1.insert("direction","-140");
        message1.insert("lat",      "29.5701911572");
        message1.insert("lon",      "106.4777135734");
        message1.insert("acc",      "0");

        QJsonObject message2;
        message2.insert("id",       "1");
        message2.insert("timeStamp","1535507946210");
        message2.insert("speed",    "5");
        message2.insert("direction","-140");
        message2.insert("lat",      "29.5701911572");
        message2.insert("lon",      "106.4777135734");
        message2.insert("acc",      "0");

        QJsonObject message3;
        message3.insert("id",       "2");
        message3.insert("timeStamp","1535507945116");
        message3.insert("speed",    "5");
        message3.insert("direction","125");
        message3.insert("lat",      "29.5694305984");
        message3.insert("lon",      "106.4750474577");
        message3.insert("acc",      "0");

        QJsonObject message4;
        message4.insert("id",       "2");
        message4.insert("timeStamp","1535507946210");
        message4.insert("speed",    "5");
        message4.insert("direction","125");
        message4.insert("lat",      "29.5694305984");
        message4.insert("lon",      "106.4750474577");
        message4.insert("acc",      "0");

    /***********************************
     *
     * *********/

//        QJsonObject message1;
//        message1.insert("id",       "1");
//        message1.insert("timeStamp","1535507945116");
//        message1.insert("speed",    "5");
//        message1.insert("direction","125");
//        message1.insert("lat",      "29.5694305984");
//        message1.insert("lon",      "106.4750474577");
//        message1.insert("acc",      "0");

//        QJsonObject message2;
//        message2.insert("id",       "1");
//        message2.insert("timeStamp","1535507946210");
//        message2.insert("speed",    "5");
//        message2.insert("direction","125");
//        message2.insert("lat",      "29.5694305984");
//        message2.insert("lon",      "106.4750474577");
//        message2.insert("acc",      "0");

//        QJsonObject message3;
//        message3.insert("id",       "2");
//        message3.insert("timeStamp","1535507945116");
//        message3.insert("speed",    "5");
//        message3.insert("direction","-140");
//        message3.insert("lat",      "29.5701911572");
//        message3.insert("lon",      "106.4777135734");
//        message3.insert("acc",      "0");

//        QJsonObject message4;
//        message4.insert("id",       "2");
//        message4.insert("timeStamp","1535507946210");
//        message4.insert("speed",    "5");
//        message4.insert("direction","-140");
//        message4.insert("lat",      "29.5701911572");
//        message4.insert("lon",      "106.4777135734");
//        message4.insert("acc",      "0");


    /***********************************
     *
     * *********/
//        QJsonObject message1;
//        message1.insert("id",       "1");
//        message1.insert("timeStamp","1535507945116");
//        message1.insert("speed",    "5");
//        message1.insert("direction","125");
//        message1.insert("lat",      "29.5701911572");
//        message1.insert("lon",      "106.4777100000");
//        message1.insert("acc",      "0");

//        QJsonObject message2;
//        message2.insert("id",       "1");
//        message2.insert("timeStamp","1535507946210");
//        message2.insert("speed",    "5");
//        message2.insert("direction","125");
//        message2.insert("lat",      "29.5701911572");
//        message2.insert("lon",      "106.4777100000");
//        message2.insert("acc",      "0");

//        QJsonObject message3;
//        message3.insert("id",       "2");
//        message3.insert("timeStamp","1535507945116");
//        message3.insert("speed",    "5");
//        message3.insert("direction","-140");
//        message3.insert("lat",      "29.5701911572");
//        message3.insert("lon",      "106.4760130529");
//        message3.insert("acc",      "0");

//        QJsonObject message4;
//        message4.insert("id",       "2");
//        message4.insert("timeStamp","1535507946210");
//        message4.insert("speed",    "5");
//        message4.insert("direction","-140");
//        message4.insert("lat",      "29.5701911572");
//        message4.insert("lon",      "106.4760130529");
//        message4.insert("acc",      "0");

    QList<QJsonObject> messages;
    messages.append(message1);
    printf(QJsonDocument(message1).toJson()+ "\n");
    messages.append(message2);
    printf(QJsonDocument(message2).toJson()+ "\n");
    messages.append(message3);
    printf(QJsonDocument(message3).toJson()+ "\n");
    messages.append(message4);
    printf(QJsonDocument(message4).toJson()+ "\n");
    return messages;

}


int main()
{
    //QJsonObject jsonObject = getDistance(106.4763885622, 29.5684833914, 106.4763885622, 29.5681240136);
    //QJsonObject jsonObject = getDistance(106.4763885622, 29.5684833914, 106.4757801647, 29.5684833914);
   // printf(QJsonDocument(jsonObject).toJson()+ "\n");
   // computerResultByAverageFeatures(inputMessage());
    saveFile(inputMessage().at(0));
    saveFile(inputMessage().at(1));
    return 1;
}


