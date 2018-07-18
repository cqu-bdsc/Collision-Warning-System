#include<QObject>
#include<QString>
#include<QtNetwork>
#ifndef UDPCOM_H
#define UDPCOM_H

class udpcom : public QObject
{
    Q_OBJECT

public slots:
    int sendMessage(QString message);

public:
    udpcom();
    ~udpcom();

private:
    QUdpSocket *sender;

};
#endif // UDPCOM_H
