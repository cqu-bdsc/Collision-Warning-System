#include <QCoreApplication>
#include <QJsonObject>
#include <QList>
#include <QJsonDocument>
QList<QJsonObject> ComputerResult(const QList<QJsonObject> &list){
    int THRESHOLD = 1000;
    QList<QJsonObject> result;
    QJsonObject RVehicleOne,RVehicleTwo;
    QList<QList<double>> tra1,tra2;
    //提取两辆车的id
    int id1=list.at(0).find("id").value().toInt();
    int id2=list.at(2).find("id").value().toInt();

    //提取两辆车的速度
    double v11=list.at(0).find("speed").value().toDouble();
    double v12=list.at(1).find("speed").value().toDouble();
    double v21=list.at(2).find("speed").value().toDouble();
    double v22=list.at(3).find("speed").value().toDouble();
    //求两辆车的平均速度
    double v1=(v11+v12)/2;
    double v2=(v21+v22)/2;

    //提取两辆车的加速度
    double acc11=list.at(0).find("acc").value().toDouble();
    double acc12=list.at(1).find("acc").value().toDouble();
    double acc21=list.at(2).find("acc").value().toDouble();
    double acc22=list.at(3).find("acc").value().toDouble();
    //求两辆车的平均加速度
    double acc1=(acc11+acc12)/2;
    double acc2=(acc21+acc22)/2;

    //提取两辆车的lat
    double lat11=list.at(0).find("lat").value().toDouble();
    double lat12=list.at(1).find("lat").value().toDouble();
    double lat21=list.at(2).find("lat").value().toDouble();
    double lat22=list.at(3).find("lat").value().toDouble();
    //求两辆车的平均lat（NTU)
    double lat1=(lat11+lat12)/2;
    double lat1_NTU=lat1*100000;
    double lat2=(lat21+lat22)/2;
    double lat2_NTU=lat2*100000;

    //提取两辆车的lon
    double lon11=list.at(0).find("lon").value().toDouble();
    double lon12=list.at(1).find("lon").value().toDouble();
    double lon21=list.at(2).find("lon").value().toDouble();
    double lon22=list.at(3).find("lon").value().toDouble();
    //求两辆车的平均lon（NTU)
    double lon1=(lon11+lon12)/2;
    double lon1_NTU=lon1*100000;
    double lon2=(lon21+lon22)/2;
    double lon2_NTU=lon2*100000;

    //提取RSU的lat(NTU)
    double Rlat=list.at(4).find("lat").value().toDouble();
    double Rlat_NTU=Rlat*100000;

    //提取RSU的lon(NTU)
    double Rlon=list.at(4).find("lon").value().toDouble();
    double Rlon_NTU=Rlon*100000;

    //求两车与路口的距离
    double dist1=sqrt(pow(lat1_NTU-Rlat_NTU,2)+pow(lon1_NTU-Rlon_NTU,2));
    double dist2=sqrt(pow(lat2_NTU-Rlat_NTU,2)+pow(lon2_NTU-Rlon_NTU,2));

    //求两车可能的碰撞时间
    double t1=(-v1+sqrt(pow(v1,2)+2*acc1*dist1))/acc1;
    double t2=(-v2+sqrt(pow(v2,2)+2*acc2*dist2))/acc2;

    if (abs(t1-t2)<THRESHOLD){//碰撞
        RVehicleOne.insert("id",id1);
        RVehicleOne.insert("lat",lat1);
        RVehicleOne.insert("lon",lon1);
        RVehicleOne.insert("warning",true);
        RVehicleOne.insert("time",t1);
        RVehicleOne.insert("distance",dist1);
        //求车1的碰撞轨迹
       // tra1=Trajectory(UNITTIME,v1,acc1,Rlat,Rlon,lat1,lon1);
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
    result.append(RVehicleOne);
    result.append(RVehicleTwo);
    return result;

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QJsonObject jo1,jo2,jo3,jo4;
    jo1.insert("id",1);
    jo1.insert("speed",5);
    jo1.insert("acc",1);
    jo1.insert("lat",29.5701584952);
    jo1.insert("lon",106.477724302);
    jo2.insert("id",1);
    jo2.insert("speed",5);
    jo2.insert("acc",1);
    jo2.insert("lat",29.5701584952);
    jo2.insert("lon",106.477724302);
    jo3.insert("id",2);
    jo3.insert("speed",5);
    jo3.insert("acc",1);
    jo3.insert("lat",29.5698552055);
    jo3.insert("lon",106.4744144563);
    jo4.insert("id",2);
    jo4.insert("speed",5);
    jo4.insert("acc",1);
    jo4.insert("lat",29.5698552055);
    jo4.insert("lon",106.4744144563);
    QJsonObject rsu;
    rsu.insert("lon",106.4763992910);
    rsu.insert("lat",29.5684880575);
    QList<QJsonObject> list;
    list.append(jo1);
    list.append(jo2);
    list.append(jo3);
    list.append(jo4);
    list.append(rsu);
    QList<QJsonObject> result = ComputerResult(list);
    QJsonObject object = result.at(0);
    QJsonObject object2 = result.at(1);
    printf(QJsonDocument(object).toJson()+ "\n");
    printf(QJsonDocument(object2).toJson());
    return a.exec();
}


