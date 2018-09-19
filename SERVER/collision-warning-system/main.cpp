#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QProcess>
#include <QSqlQuery>
#include <QDebug>
bool creatConnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("qtdb");
    db.setUserName("root");
    db.setPassword("great615");

    bool ok = db.open();//建立数据库连接
    if(!ok)
    {
        QMessageBox::critical(0,QObject::tr("连接数据库失败！！！"),db.lastError().text());
        return false;
    }
    else
    {
        QMessageBox::information(0,QObject::tr("Tips"),QObject::tr("连接数据库成功！！！"));
        QSqlQuery createMessage(db);
        /******************************
         * MySQL 似乎没有Long long
         * *********************/
        if(!createMessage.exec("CREATE TABLE message(id INTEGER PRIMARY KEY AUTO_INCREMENT, messageID int, speed float, direction float, acc double, lon double, lat double, timeStamp double, receiverStamp double, delay double)"))
        {
            QMessageBox::information(0,QObject::tr("Tips"), createMessage.lastError().text());
        } else {
            QMessageBox::information(0,QObject::tr("Tips"), QObject::tr("创建Message表成功！！！"));
        }

        QSqlQuery createResult(db);
        if(!createResult.exec("CREATE TABLE result(id INTEGER PRIMARY KEY AUTO_INCREMENT, resultID int, time int, distance double, warning boolean , sendStamp double, sended boolean)"))
        {
            QMessageBox::information(0,QObject::tr("Tips"), createResult.lastError().text());
        } else {
            QMessageBox::information(0,QObject::tr("Tips"), QObject::tr("创建Result表成功！！！"));
        }

        QSqlQuery createLog(db);
        if(!createLog.exec("CREATE TABLE log(id INTEGER PRIMARY KEY AUTO_INCREMENT,  timeStamp double, data text, context text)"))
        {
            QMessageBox::information(0,QObject::tr("Tips"), createLog.lastError().text());
        } else {
            QMessageBox::information(0,QObject::tr("Tips"), QObject::tr("创建Log表成功！！！"));
        }
        return true;
    }
}

/**应用程序入口
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //窗口类
    MainWindow w;
    QIcon icon(":/image/logo.png");

    if (!creatConnect()) return 1;

    //显示窗口
    w.setWindowTitle("碰撞预警系统");
    w.setWindowIcon(icon);
    w.show();

    return a.exec();
}

