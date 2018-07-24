#include "myudp.h"

/**
 * MyUDP的构造函数
 * 对QUdpSocket进行初始化
 * @brief MyUDP::MyUDP
 * @param parent
 */
MyUDP::MyUDP(QObject *parent) : QUdpSocket(parent)
{
    socket = new QUdpSocket();
}

/**
 * 本机作为接收端，绑定端口函数
 * @brief MyUDP::bindPort
 * @param addr  本地IP地址
 * @param port  本地接收端的端口号
 * @return 返回是否绑定成功
 */
bool MyUDP::bindPort(QHostAddress addr, qint16 port)
{
    socket->abort();
    bool isBinded = socket->bind(addr, port);
    if (isBinded)  //绑定成功时
    {
        //connect函数， 接收到readyRead() 信号， 使用槽函数 readyRead()进行处理
        //connect(发送端，信号，接收端，槽函数)
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }
    return isBinded;
}

/**
 * 本机作为发送端，通过UDP向指定的IP的端口号发送String字符串
 * @brief MyUDP::sendMessage
 * @param sender       接收端的IP地址
 * @param senderPort   接收端的端口号
 * @param string       发送的消息内容
 */
void MyUDP::sendMessage(QHostAddress sender, quint16 senderPort, QJsonObject result)
{  
    QByteArray Data = QJsonDocument(result).toJson();
    // Sends the datagram datagram
    // to the host address and at port.
    socket->writeDatagram(Data, sender, senderPort);
}

/**
 * 重写槽函数readyRead
 * @brief MyUDP::readyRead
 */
void MyUDP::readyRead()
{
    // when data comes in
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    // Receives a datagram no larger than maxSize bytes and stores it in data.
    // The sender's host address and port is stored in *address and *port
    // (unless the pointers are 0).

    //本机读取接收到的UDP报文
    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);

    QJsonObject message = QJsonDocument::fromJson(buffer).object();

    //发送新消息信号
    emit newMessage(sender.toString(),  message);
}
/**
 * 解绑函数
 * @brief MyUDP::unbindPort
 */
void MyUDP::unbindPort()
{
    disconnect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));  //关闭连接
    socket->close();  //关闭socket
}
