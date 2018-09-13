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
    bool isSolved(double a, double b, double c);
    bool solveTime(double a, double b, double c);
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
    bool addMessageToDB(const QJsonObject &message);      //将信息添加到数据库中

//    void ComputerResult(const QList<QJsonObject> &messages);
    void computerResult(const QList<QJsonObject> &messages);
    void computerResultByAverageFeatures(const QList<QJsonObject> &message); //使用平均的特征以及物理特性
    void computerResultByLinearRegression(const QList<QJsonObject> &location); //使用线性回归的方法



signals:

    void newComputableByAverageFeatures(const QList<QJsonObject> &fourMessages);     //可以计算时，调用ComputerResult
    void newComputableByLinearRegression(const QList<QJsonObject> &location);
    void sendResult(const QJsonObject &result);             //有计算结果时，先将结果发送的UI
    void newLogInfo(const QString &logInfo);
    void newVehicleOne(const double &lon, const double &lat);
    void newVehicleTwo(const double &lon, const double &lat);

private:
    QJsonObject rsuLocation;

    QTimer * timer;
    QQueue<QJsonObject> queueVehicleOne;
    QQueue<QJsonObject> queueVehicleTwo;

    const int FREQUENCY = 1000;   // timer的时间频率
    const int THRESHOLD = 1000;  // 碰撞时间的阈值
    const int UNITTIME = 10;     // 求轨迹的单位时间

};

#endif // DATAPROCESSTHREAD_H
