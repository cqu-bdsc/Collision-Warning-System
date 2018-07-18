#include <QtNetwork>

QUdpSocket *receiver;

private slots:
    void readPendingDatagrams();

