/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <ActiveQt/QAxWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupInfo;
    QWidget *horizontalLayoutWidget_11;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_10;
    QWidget *horizontalLayoutWidget_13;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_11;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *btn_show_tar;
    QPushButton *btn_clear_tar;
    QLabel *label_logo;
    QLabel *label;
    QLabel *label_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *NetworkInterface;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *NetInterface;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_local;
    QLabel *label_locIP;
    QGroupBox *UDP;
    QGroupBox *groupSend;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_ip;
    QLineEdit *editSendIP;
    QLabel *label_port;
    QLineEdit *editSendPort;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *editID;
    QLabel *label_4;
    QLineEdit *editWarning;
    QWidget *horizontalLayoutWidget_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLineEdit *editTime;
    QLabel *label_6;
    QLineEdit *editDistance;
    QWidget *horizontalLayoutWidget_8;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *but_send;
    QGroupBox *groupReceive;
    QGroupBox *groupListen;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelPort;
    QLineEdit *editLocalPort;
    QSpacerItem *horizontalSpacer;
    QPushButton *but_start;
    QGroupBox *groupShowMessage;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_textID;
    QLabel *label_id;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_7;
    QLabel *label_from;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_textSpeed;
    QLabel *label_speed;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_textAcc;
    QLabel *label_acc;
    QWidget *horizontalLayoutWidget_9;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_textLon;
    QLabel *label_lon;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_textLat;
    QLabel *label_Lat;
    QWidget *horizontalLayoutWidget_10;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_textDirection;
    QLabel *label_direction;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_textTimeStamp;
    QLabel *label_timeStamp;
    QGroupBox *Log;
    QTextBrowser *textLog;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QWidget *horizontalLayoutWidget_12;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_12;
    QLineEdit *timeEdit;
    QSpacerItem *horizontalSpacer_8;
    QLabel *label_14;
    QLineEdit *distanceEdit;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *pushButton;
    QGroupBox *Map;
    QGridLayout *gridLayout;
    QAxWidget *axWidget;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1200, 732);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupInfo = new QGroupBox(centralWidget);
        groupInfo->setObjectName(QStringLiteral("groupInfo"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupInfo->sizePolicy().hasHeightForWidth());
        groupInfo->setSizePolicy(sizePolicy);
        groupInfo->setMinimumSize(QSize(0, 60));
        horizontalLayoutWidget_11 = new QWidget(groupInfo);
        horizontalLayoutWidget_11->setObjectName(QStringLiteral("horizontalLayoutWidget_11"));
        horizontalLayoutWidget_11->setGeometry(QRect(10, 20, 731, 20));
        horizontalLayout_11 = new QHBoxLayout(horizontalLayoutWidget_11);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(horizontalLayoutWidget_11);
        label_10->setObjectName(QStringLiteral("label_10"));
        sizePolicy.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy);

        horizontalLayout_11->addWidget(label_10);

        horizontalLayoutWidget_13 = new QWidget(groupInfo);
        horizontalLayoutWidget_13->setObjectName(QStringLiteral("horizontalLayoutWidget_13"));
        horizontalLayoutWidget_13->setGeometry(QRect(10, 40, 731, 25));
        horizontalLayout_13 = new QHBoxLayout(horizontalLayoutWidget_13);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(horizontalLayoutWidget_13);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_13->addWidget(label_11);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_10);

        btn_show_tar = new QPushButton(horizontalLayoutWidget_13);
        btn_show_tar->setObjectName(QStringLiteral("btn_show_tar"));

        horizontalLayout_13->addWidget(btn_show_tar);

        btn_clear_tar = new QPushButton(horizontalLayoutWidget_13);
        btn_clear_tar->setObjectName(QStringLiteral("btn_clear_tar"));

        horizontalLayout_13->addWidget(btn_clear_tar);


        gridLayout_2->addWidget(groupInfo, 0, 2, 2, 2);

        label_logo = new QLabel(centralWidget);
        label_logo->setObjectName(QStringLiteral("label_logo"));
        label_logo->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_logo->sizePolicy().hasHeightForWidth());
        label_logo->setSizePolicy(sizePolicy1);
        label_logo->setMinimumSize(QSize(70, 70));

        gridLayout_2->addWidget(label_logo, 0, 0, 2, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(15);
        font.setBold(false);
        font.setWeight(50);
        label->setFont(font);

        gridLayout_2->addWidget(label, 0, 1, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(label_2, 1, 1, 1, 2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        NetworkInterface = new QGroupBox(centralWidget);
        NetworkInterface->setObjectName(QStringLiteral("NetworkInterface"));
        sizePolicy1.setHeightForWidth(NetworkInterface->sizePolicy().hasHeightForWidth());
        NetworkInterface->setSizePolicy(sizePolicy1);
        NetworkInterface->setMinimumSize(QSize(430, 55));
        horizontalLayoutWidget_4 = new QWidget(NetworkInterface);
        horizontalLayoutWidget_4->setObjectName(QStringLiteral("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(10, 20, 411, 31));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        NetInterface = new QComboBox(horizontalLayoutWidget_4);
        NetInterface->setObjectName(QStringLiteral("NetInterface"));

        horizontalLayout_4->addWidget(NetInterface);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        label_local = new QLabel(horizontalLayoutWidget_4);
        label_local->setObjectName(QStringLiteral("label_local"));

        horizontalLayout_4->addWidget(label_local);

        label_locIP = new QLabel(horizontalLayoutWidget_4);
        label_locIP->setObjectName(QStringLiteral("label_locIP"));

        horizontalLayout_4->addWidget(label_locIP);


        verticalLayout->addWidget(NetworkInterface);

        UDP = new QGroupBox(centralWidget);
        UDP->setObjectName(QStringLiteral("UDP"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(UDP->sizePolicy().hasHeightForWidth());
        UDP->setSizePolicy(sizePolicy3);
        UDP->setMinimumSize(QSize(430, 415));
        groupSend = new QGroupBox(UDP);
        groupSend->setObjectName(QStringLiteral("groupSend"));
        groupSend->setGeometry(QRect(10, 250, 421, 151));
        horizontalLayoutWidget = new QWidget(groupSend);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 20, 401, 31));
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

        horizontalLayoutWidget_5 = new QWidget(groupSend);
        horizontalLayoutWidget_5->setObjectName(QStringLiteral("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(10, 50, 401, 31));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_5);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_5->addWidget(label_3);

        editID = new QLineEdit(horizontalLayoutWidget_5);
        editID->setObjectName(QStringLiteral("editID"));

        horizontalLayout_5->addWidget(editID);

        label_4 = new QLabel(horizontalLayoutWidget_5);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_5->addWidget(label_4);

        editWarning = new QLineEdit(horizontalLayoutWidget_5);
        editWarning->setObjectName(QStringLiteral("editWarning"));

        horizontalLayout_5->addWidget(editWarning);

        horizontalLayoutWidget_6 = new QWidget(groupSend);
        horizontalLayoutWidget_6->setObjectName(QStringLiteral("horizontalLayoutWidget_6"));
        horizontalLayoutWidget_6->setGeometry(QRect(10, 80, 401, 31));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget_6);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(horizontalLayoutWidget_6);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_6->addWidget(label_5);

        editTime = new QLineEdit(horizontalLayoutWidget_6);
        editTime->setObjectName(QStringLiteral("editTime"));

        horizontalLayout_6->addWidget(editTime);

        label_6 = new QLabel(horizontalLayoutWidget_6);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_6->addWidget(label_6);

        editDistance = new QLineEdit(horizontalLayoutWidget_6);
        editDistance->setObjectName(QStringLiteral("editDistance"));

        horizontalLayout_6->addWidget(editDistance);

        horizontalLayoutWidget_8 = new QWidget(groupSend);
        horizontalLayoutWidget_8->setObjectName(QStringLiteral("horizontalLayoutWidget_8"));
        horizontalLayoutWidget_8->setGeometry(QRect(10, 120, 401, 25));
        horizontalLayout_8 = new QHBoxLayout(horizontalLayoutWidget_8);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        but_send = new QPushButton(horizontalLayoutWidget_8);
        but_send->setObjectName(QStringLiteral("but_send"));
        sizePolicy1.setHeightForWidth(but_send->sizePolicy().hasHeightForWidth());
        but_send->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(but_send);

        groupReceive = new QGroupBox(UDP);
        groupReceive->setObjectName(QStringLiteral("groupReceive"));
        groupReceive->setGeometry(QRect(10, 20, 421, 231));
        groupListen = new QGroupBox(groupReceive);
        groupListen->setObjectName(QStringLiteral("groupListen"));
        groupListen->setGeometry(QRect(10, 20, 401, 61));
        horizontalLayoutWidget_3 = new QWidget(groupListen);
        horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 20, 391, 31));
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
        groupShowMessage->setGeometry(QRect(9, 89, 411, 141));
        horizontalLayoutWidget_2 = new QWidget(groupShowMessage);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 20, 391, 22));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_textID = new QLabel(horizontalLayoutWidget_2);
        label_textID->setObjectName(QStringLiteral("label_textID"));

        horizontalLayout_2->addWidget(label_textID);

        label_id = new QLabel(horizontalLayoutWidget_2);
        label_id->setObjectName(QStringLiteral("label_id"));

        horizontalLayout_2->addWidget(label_id);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        label_7 = new QLabel(horizontalLayoutWidget_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_2->addWidget(label_7);

        label_from = new QLabel(horizontalLayoutWidget_2);
        label_from->setObjectName(QStringLiteral("label_from"));

        horizontalLayout_2->addWidget(label_from);

        horizontalLayoutWidget_7 = new QWidget(groupShowMessage);
        horizontalLayoutWidget_7->setObjectName(QStringLiteral("horizontalLayoutWidget_7"));
        horizontalLayoutWidget_7->setGeometry(QRect(10, 50, 391, 22));
        horizontalLayout_7 = new QHBoxLayout(horizontalLayoutWidget_7);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        label_textSpeed = new QLabel(horizontalLayoutWidget_7);
        label_textSpeed->setObjectName(QStringLiteral("label_textSpeed"));

        horizontalLayout_7->addWidget(label_textSpeed);

        label_speed = new QLabel(horizontalLayoutWidget_7);
        label_speed->setObjectName(QStringLiteral("label_speed"));

        horizontalLayout_7->addWidget(label_speed);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);

        label_textAcc = new QLabel(horizontalLayoutWidget_7);
        label_textAcc->setObjectName(QStringLiteral("label_textAcc"));

        horizontalLayout_7->addWidget(label_textAcc);

        label_acc = new QLabel(horizontalLayoutWidget_7);
        label_acc->setObjectName(QStringLiteral("label_acc"));

        horizontalLayout_7->addWidget(label_acc);

        horizontalLayoutWidget_9 = new QWidget(groupShowMessage);
        horizontalLayoutWidget_9->setObjectName(QStringLiteral("horizontalLayoutWidget_9"));
        horizontalLayoutWidget_9->setGeometry(QRect(10, 80, 391, 22));
        horizontalLayout_9 = new QHBoxLayout(horizontalLayoutWidget_9);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        label_textLon = new QLabel(horizontalLayoutWidget_9);
        label_textLon->setObjectName(QStringLiteral("label_textLon"));

        horizontalLayout_9->addWidget(label_textLon);

        label_lon = new QLabel(horizontalLayoutWidget_9);
        label_lon->setObjectName(QStringLiteral("label_lon"));

        horizontalLayout_9->addWidget(label_lon);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_7);

        label_textLat = new QLabel(horizontalLayoutWidget_9);
        label_textLat->setObjectName(QStringLiteral("label_textLat"));

        horizontalLayout_9->addWidget(label_textLat);

        label_Lat = new QLabel(horizontalLayoutWidget_9);
        label_Lat->setObjectName(QStringLiteral("label_Lat"));

        horizontalLayout_9->addWidget(label_Lat);

        horizontalLayoutWidget_10 = new QWidget(groupShowMessage);
        horizontalLayoutWidget_10->setObjectName(QStringLiteral("horizontalLayoutWidget_10"));
        horizontalLayoutWidget_10->setGeometry(QRect(10, 110, 391, 22));
        horizontalLayout_10 = new QHBoxLayout(horizontalLayoutWidget_10);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_textDirection = new QLabel(horizontalLayoutWidget_10);
        label_textDirection->setObjectName(QStringLiteral("label_textDirection"));

        horizontalLayout_10->addWidget(label_textDirection);

        label_direction = new QLabel(horizontalLayoutWidget_10);
        label_direction->setObjectName(QStringLiteral("label_direction"));

        horizontalLayout_10->addWidget(label_direction);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_4);

        label_textTimeStamp = new QLabel(horizontalLayoutWidget_10);
        label_textTimeStamp->setObjectName(QStringLiteral("label_textTimeStamp"));

        horizontalLayout_10->addWidget(label_textTimeStamp);

        label_timeStamp = new QLabel(horizontalLayoutWidget_10);
        label_timeStamp->setObjectName(QStringLiteral("label_timeStamp"));

        horizontalLayout_10->addWidget(label_timeStamp);

        Log = new QGroupBox(UDP);
        Log->setObjectName(QStringLiteral("Log"));
        Log->setEnabled(true);
        Log->setGeometry(QRect(0, 400, 441, 181));
        sizePolicy3.setHeightForWidth(Log->sizePolicy().hasHeightForWidth());
        Log->setSizePolicy(sizePolicy3);
        Log->setMinimumSize(QSize(400, 0));
        textLog = new QTextBrowser(Log);
        textLog->setObjectName(QStringLiteral("textLog"));
        textLog->setGeometry(QRect(10, 20, 411, 141));
        sizePolicy3.setHeightForWidth(textLog->sizePolicy().hasHeightForWidth());
        textLog->setSizePolicy(sizePolicy3);
        textLog->setMinimumSize(QSize(300, 0));
        textLog->setFocusPolicy(Qt::NoFocus);

        verticalLayout->addWidget(UDP);


        gridLayout_2->addLayout(verticalLayout, 2, 0, 3, 2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 40));
        horizontalLayoutWidget_12 = new QWidget(groupBox);
        horizontalLayoutWidget_12->setObjectName(QStringLiteral("horizontalLayoutWidget_12"));
        horizontalLayoutWidget_12->setGeometry(QRect(10, 10, 731, 25));
        horizontalLayout_12 = new QHBoxLayout(horizontalLayoutWidget_12);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, 0, 0, 0);
        label_12 = new QLabel(horizontalLayoutWidget_12);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_12->addWidget(label_12);

        timeEdit = new QLineEdit(horizontalLayoutWidget_12);
        timeEdit->setObjectName(QStringLiteral("timeEdit"));

        horizontalLayout_12->addWidget(timeEdit);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_8);

        label_14 = new QLabel(horizontalLayoutWidget_12);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_12->addWidget(label_14);

        distanceEdit = new QLineEdit(horizontalLayoutWidget_12);
        distanceEdit->setObjectName(QStringLiteral("distanceEdit"));

        horizontalLayout_12->addWidget(distanceEdit);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_9);

        pushButton = new QPushButton(horizontalLayoutWidget_12);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_12->addWidget(pushButton);


        verticalLayout_2->addWidget(groupBox);

        Map = new QGroupBox(centralWidget);
        Map->setObjectName(QStringLiteral("Map"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(Map->sizePolicy().hasHeightForWidth());
        Map->setSizePolicy(sizePolicy4);
        gridLayout = new QGridLayout(Map);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        axWidget = new QAxWidget(Map);
        axWidget->setObjectName(QStringLiteral("axWidget"));
        sizePolicy4.setHeightForWidth(axWidget->sizePolicy().hasHeightForWidth());
        axWidget->setProperty("sizePolicy", QVariant(sizePolicy4));

        gridLayout->addWidget(axWidget, 0, 0, 1, 1);


        verticalLayout_2->addWidget(Map);


        gridLayout_2->addLayout(verticalLayout_2, 2, 2, 1, 2);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        groupInfo->setTitle(QApplication::translate("MainWindow", "Info", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "\345\234\260\345\233\276\344\277\241\346\201\257\346\224\257\346\214\201\357\274\232\347\231\276\345\272\246\345\234\260\345\233\276", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "All rights reserved. Big Data and Smart Computing Lab of CQU.", nullptr));
        btn_show_tar->setText(QApplication::translate("MainWindow", "Show Trajectory", nullptr));
        btn_clear_tar->setText(QApplication::translate("MainWindow", "Clear Trajectory", nullptr));
        label_logo->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label->setText(QApplication::translate("MainWindow", "\351\207\215\345\272\206\345\244\247\345\255\246BDSC\345\256\236\351\252\214\345\256\244\350\275\246\350\276\206\347\242\260\346\222\236\351\242\204\350\255\246\347\263\273\347\273\237", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "2018-7-18 \347\211\210\346\234\254\357\274\2321.0", nullptr));
        NetworkInterface->setTitle(QApplication::translate("MainWindow", "\347\275\221\347\273\234\346\216\245\345\217\243\351\200\211\346\213\251", nullptr));
        label_local->setText(QApplication::translate("MainWindow", "\346\234\254\345\234\260IP:", nullptr));
        label_locIP->setText(QApplication::translate("MainWindow", "255.255.255.255", nullptr));
        UDP->setTitle(QApplication::translate("MainWindow", "UDP\351\200\232\344\277\241\346\265\213\350\257\225\346\250\241\345\235\227", nullptr));
        groupSend->setTitle(QApplication::translate("MainWindow", "\345\217\221\351\200\201\346\266\210\346\201\257", nullptr));
        label_ip->setText(QApplication::translate("MainWindow", "IP", nullptr));
        label_port->setText(QApplication::translate("MainWindow", "\347\253\257\345\217\243", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "ID", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\350\255\246\346\212\245", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\347\242\260\346\222\236\346\227\266\351\227\264", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "\347\242\260\346\222\236\350\267\235\347\246\273", nullptr));
        but_send->setText(QApplication::translate("MainWindow", "Send", nullptr));
        groupReceive->setTitle(QApplication::translate("MainWindow", "\346\216\245\346\224\266\346\266\210\346\201\257", nullptr));
        groupListen->setTitle(QApplication::translate("MainWindow", "\347\233\221\345\220\254", nullptr));
        labelPort->setText(QApplication::translate("MainWindow", "\347\253\257\345\217\243", nullptr));
        but_start->setText(QApplication::translate("MainWindow", "Start", nullptr));
        groupShowMessage->setTitle(QApplication::translate("MainWindow", "\346\230\276\347\244\272\346\266\210\346\201\257", nullptr));
        label_textID->setText(QApplication::translate("MainWindow", "ID", nullptr));
        label_id->setText(QApplication::translate("MainWindow", "MyID", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "From", nullptr));
        label_from->setText(QApplication::translate("MainWindow", "255.255.255.255", nullptr));
        label_textSpeed->setText(QApplication::translate("MainWindow", "\351\200\237\345\272\246", nullptr));
        label_speed->setText(QApplication::translate("MainWindow", "MySpeed", nullptr));
        label_textAcc->setText(QApplication::translate("MainWindow", "\345\212\240\351\200\237\345\272\246", nullptr));
        label_acc->setText(QApplication::translate("MainWindow", "00000000000000", nullptr));
        label_textLon->setText(QApplication::translate("MainWindow", "\347\273\217\345\272\246", nullptr));
        label_lon->setText(QApplication::translate("MainWindow", "0000000000", nullptr));
        label_textLat->setText(QApplication::translate("MainWindow", "\347\272\254\345\272\246", nullptr));
        label_Lat->setText(QApplication::translate("MainWindow", "00000000000000", nullptr));
        label_textDirection->setText(QApplication::translate("MainWindow", "\346\226\271\345\220\221", nullptr));
        label_direction->setText(QApplication::translate("MainWindow", "00000", nullptr));
        label_textTimeStamp->setText(QApplication::translate("MainWindow", "\346\227\266\351\227\264\346\210\263", nullptr));
        label_timeStamp->setText(QApplication::translate("MainWindow", "00000000000000", nullptr));
        Log->setTitle(QApplication::translate("MainWindow", "Log", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "\347\242\260\346\222\236\345\217\202\346\225\260\350\256\276\345\256\232", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "\347\242\260\346\222\236\346\227\266\351\227\264\351\230\210\345\200\274\357\274\232", nullptr));
        label_14->setText(QApplication::translate("MainWindow", "\347\242\260\346\222\236\345\214\272\345\237\237\345\244\247\345\260\217\357\274\232", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "Confirm", nullptr));
        Map->setTitle(QApplication::translate("MainWindow", "Map", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
