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
        QSqlQuery query(db);
        if(!query.exec("CREATE TABLE message(id INTEGER PRIMARY KEY AUTO_INCREMENT, messageID int, speed float, direction float, acc double, lon double, lat double, timeStamp double)"))
        {
            QMessageBox::information(0,QObject::tr("Tips"), query.lastError().text());
        } else {
            QMessageBox::information(0,QObject::tr("Tips"), QObject::tr("创建Message表成功！！！"));
        }
//        query.exec("create table message(id INTEGER PRIMARY KEY autoincrement, messageID int, speed float, direction float, acc double, lon double, lat double, timeStamp double)"); //id自动增加
//        QSqlQuery query2;
//        query2.prepare("INSERT INTO message(messageID, speed, direction, acc, lon, lat, timeStamp)" "VALUES(:messageID, :speed, :direction, :acc, :lon, :lat, :timeStamp)");
//        query2.bindValue(":messageID", 1);
//        query2.bindValue(":timeStamp", 1);
//        query2.bindValue(":speed", 66);
//        query2.bindValue(":direction", 66);
//        query2.bindValue(":acc", 66);
//        query2.bindValue(":lon", 666);
//        query2.bindValue(":lat", 666);
//        if(!query2.exec())
//        {

//        } else {
//            QMessageBox::information(0,QObject::tr("Tips"), QObject::tr("创建Message表成功！！！"));
//        }
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

