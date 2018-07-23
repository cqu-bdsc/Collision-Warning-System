#ifndef MYUDP_H
#define MYUDP_H


#include <QUdpSocket>
#include <QJsonObject>
#include <QJsonDocument>

/**
 * UDP通信模块的头文件，函数实现在myudp.cpp文件中
 * @brief The MyUDP class
 */
class MyUDP : public QUdpSocket
{
    Q_OBJECT

  public:
    explicit MyUDP(QObject *parent = nullptr);
    //绑定端口
    bool bindPort(QHostAddress addr, qint16 port);
    //去除绑定
    void unbindPort();

  //信号
  signals:
    //收到到新消息，参数为IP地址，与消息内容
    void newMessage(const QString &from, const QJsonObject &message);

  //槽函数
  public slots:
    //读取信息的槽函数
    void readyRead();
    //发送信息的槽函数
    //sender是Host地址，senderPort是接收端的端口号，string是发送的内容
    void sendMessage(QHostAddress sender, quint16 senderPort, QJsonObject result);

  private:
    //使用QUdpSocket 进行发送接收
    QUdpSocket *socket;
};

#endif // MYUDP_H
