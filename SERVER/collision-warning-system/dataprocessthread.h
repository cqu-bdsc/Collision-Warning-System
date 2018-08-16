#ifndef DATAPROCESSTHREAD_H
#define DATAPROCESSTHREAD_H
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include <QTimer>
#include <QQueue>
#include <QList>

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
    void ComputerResult(const QList<QJsonObject> &fourMessages);  //计算结果

signals:

    void newComputable(const QList<QJsonObject> &fourMessages);     //可以计算时，调用ComputerResult
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
