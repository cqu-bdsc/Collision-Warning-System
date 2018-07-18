/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *NetworkInterface;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *NetInterface;
    QLabel *label_local;
    QLabel *label_locIP;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *but_getIP;
    QGroupBox *UDP;
    QGroupBox *groupSend;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_ip;
    QLineEdit *editSendIP;
    QLabel *label_port;
    QLineEdit *editSendPort;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit_3;
    QPushButton *sendButton;
    QGroupBox *groupReceive;
    QGroupBox *groupListen;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelPort;
    QLineEdit *editLocalPort;
    QSpacerItem *horizontalSpacer;
    QPushButton *but_start;
    QGroupBox *groupShowMessage;
    QTextBrowser *textReceive;
    QGroupBox *Map;
    QGraphicsView *graphicsView;
    QGroupBox *Log;
    QTextBrowser *textLog;
    QGroupBox *groupInfo;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_logo;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1042, 677);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        NetworkInterface = new QGroupBox(centralWidget);
        NetworkInterface->setObjectName(QStringLiteral("NetworkInterface"));
        NetworkInterface->setGeometry(QRect(10, 90, 321, 61));
        horizontalLayoutWidget_4 = new QWidget(NetworkInterface);
        horizontalLayoutWidget_4->setObjectName(QStringLiteral("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(10, 20, 308, 31));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        NetInterface = new QComboBox(horizontalLayoutWidget_4);
        NetInterface->setObjectName(QStringLiteral("NetInterface"));

        horizontalLayout_4->addWidget(NetInterface);

        label_local = new QLabel(horizontalLayoutWidget_4);
        label_local->setObjectName(QStringLiteral("label_local"));

        horizontalLayout_4->addWidget(label_local);

        label_locIP = new QLabel(horizontalLayoutWidget_4);
        label_locIP->setObjectName(QStringLiteral("label_locIP"));

        horizontalLayout_4->addWidget(label_locIP);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        but_getIP = new QPushButton(horizontalLayoutWidget_4);
        but_getIP->setObjectName(QStringLiteral("but_getIP"));

        horizontalLayout_4->addWidget(but_getIP);

        UDP = new QGroupBox(centralWidget);
        UDP->setObjectName(QStringLiteral("UDP"));
        UDP->setGeometry(QRect(10, 160, 321, 381));
        groupSend = new QGroupBox(UDP);
        groupSend->setObjectName(QStringLiteral("groupSend"));
        groupSend->setGeometry(QRect(10, 280, 301, 91));
        horizontalLayoutWidget = new QWidget(groupSend);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 20, 281, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_ip = new QLabel(horizontalLayoutWidget);
        label_ip->setObjectName(QStringLiteral("label_ip"));

        horizontalLayout->addWidget(label_ip);

        editSendIP = new QLineEdit(horizontalLayoutWidget);
        editSendIP->setObjectName(QStringLiteral("editSendIP"));

        horizontalLayout->addWidget(editSendIP);

        label_port = new QLabel(horizontalLayoutWidget);
        label_port->setObjectName(QStringLiteral("label_port"));

        horizontalLayout->addWidget(label_port);

        editSendPort = new QLineEdit(horizontalLayoutWidget);
        editSendPort->setObjectName(QStringLiteral("editSendPort"));

        horizontalLayout->addWidget(editSendPort);

        horizontalLayoutWidget_2 = new QWidget(groupSend);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 50, 281, 31));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        lineEdit_3 = new QLineEdit(horizontalLayoutWidget_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_2->addWidget(lineEdit_3);

        sendButton = new QPushButton(horizontalLayoutWidget_2);
        sendButton->setObjectName(QStringLiteral("sendButton"));

        horizontalLayout_2->addWidget(sendButton);

        groupReceive = new QGroupBox(UDP);
        groupReceive->setObjectName(QStringLiteral("groupReceive"));
        groupReceive->setGeometry(QRect(10, 20, 301, 251));
        groupListen = new QGroupBox(groupReceive);
        groupListen->setObjectName(QStringLiteral("groupListen"));
        groupListen->setGeometry(QRect(10, 20, 281, 61));
        horizontalLayoutWidget_3 = new QWidget(groupListen);
        horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 20, 261, 31));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        labelPort = new QLabel(horizontalLayoutWidget_3);
        labelPort->setObjectName(QStringLiteral("labelPort"));

        horizontalLayout_3->addWidget(labelPort);

        editLocalPort = new QLineEdit(horizontalLayoutWidget_3);
        editLocalPort->setObjectName(QStringLiteral("editLocalPort"));

        horizontalLayout_3->addWidget(editLocalPort);

        horizontalSpacer = new QSpacerItem(88, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        but_start = new QPushButton(horizontalLayoutWidget_3);
        but_start->setObjectName(QStringLiteral("but_start"));

        horizontalLayout_3->addWidget(but_start);

        groupShowMessage = new QGroupBox(groupReceive);
        groupShowMessage->setObjectName(QStringLiteral("groupShowMessage"));
        groupShowMessage->setGeometry(QRect(9, 89, 281, 151));
        textReceive = new QTextBrowser(groupShowMessage);
        textReceive->setObjectName(QStringLiteral("textReceive"));
        textReceive->setEnabled(true);
        textReceive->setGeometry(QRect(10, 20, 261, 131));
        Map = new QGroupBox(centralWidget);
        Map->setObjectName(QStringLiteral("Map"));
        Map->setGeometry(QRect(340, 90, 691, 581));
        graphicsView = new QGraphicsView(Map);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(15, 21, 671, 551));
        Log = new QGroupBox(centralWidget);
        Log->setObjectName(QStringLiteral("Log"));
        Log->setGeometry(QRect(10, 550, 321, 121));
        textLog = new QTextBrowser(Log);
        textLog->setObjectName(QStringLiteral("textLog"));
        textLog->setGeometry(QRect(10, 20, 301, 91));
        groupInfo = new QGroupBox(centralWidget);
        groupInfo->setObjectName(QStringLiteral("groupInfo"));
        groupInfo->setGeometry(QRect(560, 10, 461, 81));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(110, 20, 411, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(16);
        font.setBold(false);
        font.setWeight(50);
        label->setFont(font);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(110, 60, 261, 16));
        label_logo = new QLabel(centralWidget);
        label_logo->setObjectName(QStringLiteral("label_logo"));
        label_logo->setGeometry(QRect(20, 10, 71, 71));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        NetworkInterface->setTitle(QApplication::translate("MainWindow", "\347\275\221\347\273\234\346\216\245\345\217\243\351\200\211\346\213\251", nullptr));
        label_local->setText(QApplication::translate("MainWindow", "\346\234\254\345\234\260IP:", nullptr));
        label_locIP->setText(QApplication::translate("MainWindow", "255.255.255.255", nullptr));
        but_getIP->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260", nullptr));
        UDP->setTitle(QApplication::translate("MainWindow", "UDP\351\200\232\344\277\241\346\265\213\350\257\225\346\250\241\345\235\227", nullptr));
        groupSend->setTitle(QApplication::translate("MainWindow", "\345\217\221\351\200\201\346\266\210\346\201\257", nullptr));
        label_ip->setText(QApplication::translate("MainWindow", "IP", nullptr));
        label_port->setText(QApplication::translate("MainWindow", "\347\253\257\345\217\243", nullptr));
        sendButton->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        groupReceive->setTitle(QApplication::translate("MainWindow", "\346\216\245\346\224\266\346\266\210\346\201\257", nullptr));
        groupListen->setTitle(QApplication::translate("MainWindow", "\347\233\221\345\220\254", nullptr));
        labelPort->setText(QApplication::translate("MainWindow", "\347\253\257\345\217\243", nullptr));
        but_start->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213", nullptr));
        groupShowMessage->setTitle(QApplication::translate("MainWindow", "\346\230\276\347\244\272\346\266\210\346\201\257", nullptr));
        Map->setTitle(QApplication::translate("MainWindow", "Map", nullptr));
        Log->setTitle(QApplication::translate("MainWindow", "Log", nullptr));
        groupInfo->setTitle(QApplication::translate("MainWindow", "Info", nullptr));
        label->setText(QApplication::translate("MainWindow", "\351\207\215\345\272\206\345\244\247\345\255\246BDSC\345\256\236\351\252\214\345\256\244\350\275\246\350\276\206\347\242\260\346\222\236\351\242\204\350\255\246\347\263\273\347\273\237", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "2018-7-18 \347\211\210\346\234\254\357\274\2321.0", nullptr));
        label_logo->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
