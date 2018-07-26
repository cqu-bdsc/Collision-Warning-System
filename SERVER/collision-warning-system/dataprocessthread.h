#ifndef DATAPROCESSTHREAD_H
#define DATAPROCESSTHREAD_H

#include <QJsonObject>
#include <QThread>
#include <QTimer>
#include <QQueue>
#include <QList>

class DataProcessThread : public QThread
{
    Q_OBJECT
public:
    DataProcessThread();
    ~DataProcessThread();
    /**
     * 是否可以开始计算
     * @brief isComputed
     * @return
     */
    QList<QJsonObject> isComputed();
    /**
     * 添加信息
     * @brief addMessage
     * @param message
     */
    bool addMessage(const QJsonObject &message);
    void ComputerResult(const QList<QJsonObject> &list);
    void run();

signals:
    void sendResult(const QJsonObject &result);

private:
    QTimer * timer;
    QQueue<QJsonObject> queueVehicleOne;
    QQueue<QJsonObject> queueVehicleTwo;
    int idOne,idTwo;        //存储两辆车的ID

    const int FREQUENCY = 200;   // timer的时间频率
    const int THRESHOLD = 1000;  // 碰撞时间的阈值
    const int UNITTIME = 10;     // 求轨迹的单位时间
private slots:
    void timeOutSlot();
};

#endif // DATAPROCESSTHREAD_H
