#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QAxObject>
#include <QMainWindow>
#include <QTextTable>
#include <QScrollBar>
#include <QNetworkInterface>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include "myudp.h"
#include "dataprocessthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//槽函数
private slots:
    /****************************
     *
     * UDP通信模块槽函数
     * on_sendButton_clicked   发送按钮单击事件的槽函数
     * onUdpSendMessage        使用UDP发送消息的槽函数
     * onUdpAppendMessage      UDP接收到消息的槽函数
     *
     ******************************/
    void on_but_start_clicked();
    void onUdpStopButtonClicked();
    void onUdpSendMessage();
    void onSendMessageq(const QJsonObject &result);
    void onUdpAppendMessage(const QString &from, const QJsonObject &message);

    //获取本机IP地址
    void on_but_getIP_clicked();

    void on_NetInterface_currentIndexChanged(int index);

    /*******************************
     * 数据处理
     *
     * ****************************/
    void showResult(const QJsonObject &result);


    /***********************************
     *
     * QT与JS进行通信
     * setRsu                       设置RSU的坐标位置信息
     * setCarOneNowPosition         设置第一辆车当前位置，形成轨迹
     * setCarTwoNowPosition         设置第二辆车当前位置，形成轨迹
     * *********************************/
    void setRsu(const double &timeCrash);
    void setCarOneNowPosition(const double &lon, const double &lat);
    void setCarTwoNowPosition(const double &lon, const double &lat);
//    void setCarOneFutureTrace();
//    void setCarTwoFutureTrace();

    void showLog(const QString &logInfo);
    void on_pushButton_clicked();

signals:
    void newMessage(const QJsonObject &message);            //有新的信息，添加到队列中
    void newLogInfo(const QString &logInfo);            //发送一些调试信息

private:
    Ui::MainWindow *ui;
    //对UI进行初始化，主要对IP地址等进行约束

    QAxObject * document;
    QAxObject * parentWindow;

    void initUI();

    //初始化Socket的连接性
    bool setupConnection();
    //获取本地IP地址
    void findLocalIP();


    QList<QNetworkInterface> interfaceList; //保存网卡接口的链表

    QHostAddress localAddr;     //本地IP地址
    quint16 udpListenPort;      //本地接收端的监听端口
    QHostAddress udpTargetAddr; //目标接收端的IP地址
    quint16 udpTargetPort;      //目标接收端的监听端口

    MyUDP *myudp = nullptr;   //MyUDP 对象

    QString messageUDP = "[UDP] ";

    /****************************
     * 处理数据模块
     * *************************/

    DataProcessThread *processThread = nullptr;

};
#endif // MAINWINDOW_H
