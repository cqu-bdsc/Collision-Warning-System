#ifndef DATAPROCESSTHREAD_H
#define DATAPROCESSTHREAD_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QThread>
#include <QTimer>
#include <QQueue>
#include <QList>
#include <QSqlQuery>
#include <QSqlError>
#include <qmath.h>
#include <QDateTime>

class DataProcessThread : public QThread
{
    Q_OBJECT
public:
    DataProcessThread(const QJsonObject &rsuLocation);
    ~DataProcessThread();
    /**
     * 是否可以开始计算
     * @brief isComputed
     * @return
     */
    QList<QJsonObject> isComputed();
    QString TYPE_RESULT = "TYPE_RESULT";
    int ERROR_VALUE = -666;
    /**
     * 计算两点间的距离
     * @brief nodeDistance
     * @param lon1
     * @param lat1
     * @param lon2
     * @param lat2
     * @return
     */
    QJsonObject getDistance(double lon1, double lat1, double lon2, double lat2);
    double getDistance(QJsonObject nodeOne, QJsonObject nodeTwo);
    double getDistanceDouble(double lon1, double lat1, double lon2, double lat2);
    bool isSolved(double a, double b, double c);
    bool solveTime(double a, double b, double c);
    QList<QJsonObject> Trajectory(double v, double a, double dir, double vlat, double vlon);
    QList<QJsonObject> Judgment(int id1, int id2, double lat1,double lon1,double lat2,double lon2,QList<QJsonObject> tra1,QList<QJsonObject> tra2);
    QJsonObject Trajectory(double unittime, double v, double a, double dir, double vlat, double vlon);
    long long getTimeStamp();
    bool addResultToDB(const QJsonObject &result, bool sended);      //将信息添加到数据库中
    void run();

private slots:
    void timeOutSlot();
    /**
     * 添加信息
     * @brief addMessage
     * @param message
     */
    void setRsuLocation(const QJsonObject &rsuLocation);  //设置RSU位置
    bool addMessage(const QJsonObject &message);          //将信息添加到队列中

//    void ComputerResult(const QList<QJsonObject> &messages);
    void computerResult(const QList<QJsonObject> &messages);  //本次实验采用的算法

    void computerResultByAverageFeatures(const QList<QJsonObject> &message); //使用平均的特征以及物理特性
    void computerResultByDiscretePoints(const QList<QJsonObject> &message);  //使用离散点
    void computerResultByLinearRegression(const QList<QJsonObject> &location); //使用线性回归的方法



signals:

    void newComputable(const QList<QJsonObject> &twoMessages);
    void newComputableByAverageFeatures(const QList<QJsonObject> &fourMessages);     //可以计算时，调用ComputerResult
    void newComputableByLinearRegression(const QList<QJsonObject> &location);
    void sendResult(const QJsonObject result);             //有计算结果时，先将结果发送的UI
    void newLogInfo(const QString &logInfo);
    void newVehicleOne(const double &lon, const double &lat);
    void newVehicleTwo(const double &lon, const double &lat);

private:
    QJsonObject rsuLocation;

    QTimer * timer;
    QQueue<QJsonObject> queueVehicleOne;
    QQueue<QJsonObject> queueVehicleTwo;

//    const int FREQUENCY = 1000;   // timer的时间频率
//    const int THRESHOLD = 1000;  // 碰撞时间的阈值
//    const int UNITTIME = 10;     // 求轨迹的单位时间

    /************************************
     *  方案二中的默认参数
     * *****************************/
    const int FREQUENCY = 200;    //timer的时间频率（单位：毫秒）
    double THRESHOLD = 5;   //碰撞时间的阈值（单位：秒）
    double DISTANCE_THRESHOLD = 5; //碰撞区域的直径（单位：米）
    const int UNITTIME = 1;     //轨迹的单位间隔时间（单位：毫秒）
    const double DIR = 10;        //方向的阈值（单位：度）
    const double Velocity = 10;   //速度的阈值（单位：m/ms）
    const double Distance = 10;   //距离的阈值（单位：m）
    const double VTIME = 200;     //行驶时间的阈值（单位：毫秒）
    const double Acceleration = 10; //加速度的阈值（单位：毫秒）
    const double LAT= 0.0000001; //纬度阈值（单位：度）
    const double LON= 0.0000001; //经度阈值（单位：度）
    const double lat_THRESHOLD = 0.001;//纬度的精度
    const double lon_THRESHOLD = 0.01;//经度的精度

};

#endif // DATAPROCESSTHREAD_H
