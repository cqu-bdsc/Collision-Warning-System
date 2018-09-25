#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //调用初始化界面函数
    initUI();

    if(myudp == nullptr){
        myudp = new MyUDP;
    }

    connect(ui->but_start, SIGNAL(clicked()), this, SLOT(on_but_start_clicked()));
    connect(this, SIGNAL(newLogInfo(QString)), this, SLOT(showLog(QString)));
    connect(this, SIGNAL(newMessage(QJsonObject)), this, SLOT(addMessageToDB(QJsonObject)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

/************************************************
 * 初始化UI界面
 **************************************************/

void MainWindow::initUI(){
    //加载网页
    ui->axWidget->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));//注册组件ID
    ui->axWidget->setProperty("DisplayAlerts",false);//不显示警告信息
    ui->axWidget->setProperty("DisplayScrollBars",true);//不显示滚动条
    QString webstr=QString("file:///C:/offlineMap/offlinemap_demo/demo/1_0.html");//设置要打开的网页
    ui->axWidget->dynamicCall("Navigate(const QString&)",webstr);//显示网页

    ui->textLog->setText("显示LOG信息");

    //加载LOGO
    QString logoPath = ":/image/logo.png";
    QPixmap logo(logoPath);
    ui->label_logo->setPixmap(logo);
    ui->label_logo->resize(logo.width(),logo.height());

    /**
    * 使用正则表达式对IP地址和端口号做约束
    * @brief rule
    */
    QString rule = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    ui->editSendIP->setValidator(new QRegExpValidator(QRegExp("^" + rule + "\\." + rule + "\\." + rule + "\\." + rule + "$"), this));
    ui->editSendPort->setValidator(new QIntValidator(0, 65535, this));
    ui->editLocalPort->setValidator(new QIntValidator(0, 65535, this));

    /**
     * 初始化生成IP
     * @brief findLocalIP
     */
    findLocalIP();

    /***********************
     *  请勿注释，当调用JS时需要使用
     * ***********************/
    document = this->ui->axWidget->querySubObject("Document");
    parentWindow = document->querySubObject("parentWindow");

}

long long MainWindow::getTimeStamp(){
       QDateTime time = QDateTime::currentDateTime();   //获取当前时间
       long long timeStamp = time.toMSecsSinceEpoch();
       return timeStamp;
}


/***********************************************
 * Network Interface 模块
 * 获取本地IP地址
 ************************************************/

/**
 * 网络接口下拉框位置更改事件响应槽函数
 * @brief MainWindow::on_NetInterface_currentIndexChanged
 * @param index
*/
void MainWindow::on_NetInterface_currentIndexChanged(int index)
{
    for (int i = 0; i < interfaceList.at(index).addressEntries().size(); ++i)
    {
        if (interfaceList.at(index).addressEntries().at(i).ip().protocol() == QAbstractSocket::IPv4Protocol)
        {
            //将locIP显示出来
            ui->label_locIP->setText(interfaceList.at(index).addressEntries().at(i).ip().toString());
        }
    }
}

/**
 * 获取本地的网络接口
 * @brief MainWindow::findLocalIPs
 */
void MainWindow::findLocalIP()
{
    ui->NetInterface->clear();
    interfaceList.clear();
    QList<QNetworkInterface> listInterface = QNetworkInterface::allInterfaces();    
    for (int i = 0; i < listInterface.size(); ++i)
    {
        interfaceList.append(listInterface.at(i));
    }

    if (interfaceList.isEmpty())
    {
        // TODO wifilist is empty
    }
    else
    {
        for (int j = 0; j < interfaceList.size(); ++j)
        {
            ui->NetInterface->addItem(interfaceList.at(j).humanReadableName());
        }
    }
}


/*********************************************
 * UDP通信模块
 * ********************************************/

/**
 * UDP开始监听按钮单击事件触发槽函数
 * @brief MainWindow::on_but_start_clicked
 */
void MainWindow::on_but_start_clicked()
{
    if(udpStart){
        emit newLogInfo("UDP线程已启动。");
    } else{
        //but_start 按钮单击事件与本槽函数解除绑定
        disconnect(ui->but_start, SIGNAL(clicked()), this, SLOT(on_but_start_clicked()));

        //开始监听
        if(setupConnection()){
            ui->textLog->append(messageUDP + "listerning to "
                                + localAddr.toString()+ ":"+ QString::number(udpListenPort));
            addLogToDB(messageUDP + "listerning to "
                       + localAddr.toString()+ ":"+ QString::number(udpListenPort));
            //重新绑定槽函数
            connect(ui->but_start, SIGNAL(clicked()),this,SLOT(onUdpStopButtonClicked()));
            ui->but_start->setText("Stop");

            //绑定接收到消息的槽函数
            connect(myudp, SIGNAL(newMessage(QString, QJsonObject)), this, SLOT(onUdpAppendMessage(QString, QJsonObject)));
        } else{
            ui->textLog->append(messageUDP + "Failed to listen to:"
                                + localAddr.toString()+ ":"+ QString::number(udpListenPort));
            connect(ui->but_start, SIGNAL(clicked()), this, SLOT(on_but_start_clicked()));
        }
        udpStart = true;
    }
}

/**
 * 开始监听后，开始按钮变成停止按钮，停止按钮点击事件槽函数
 * @brief MainWindow::onUdpStopButtonClicked
 */
void MainWindow::onUdpStopButtonClicked(){
    if (udpStart){
        disconnect(ui->but_start, SIGNAL(clicked()), this, SLOT(onUdpStopButtonClicked()));
        ui->textLog->append(messageUDP+ "Stoped.");
        addLogToDB(messageUDP+ "Stoped.");
        //解除槽函数绑定
        disconnect(myudp, SIGNAL(newMessage(QString, QJsonObject)), this, SLOT(onUdpAppendMessage(QString, QJsonObject)));
        ui->but_start->setText("Start");
        myudp->unbindPort();
        //重新绑定槽函数
        connect(ui->but_start, SIGNAL(clicked()), this, SLOT(on_but_start_clicked()));
        udpStart = false;
    } else{
        emit newLogInfo("UDP线程已停止。");
    }

}


/**
 * 绑定端口号
 * @brief MainWindow::setupConnection
 * @return
 */
bool MainWindow::setupConnection(){
    bool isSuccess = false;
    //对localAddr 进行设置
    localAddr.setAddress(ui->label_locIP->text());

    if (ui->editLocalPort->text() != ""){
        udpListenPort = ui->editLocalPort->text().toInt();
    }
    isSuccess = myudp->bindPort(localAddr, udpListenPort);
    return isSuccess;
}

/********************************
 * 处理的线程的启动按钮与结束按钮
 * *****************************/
void MainWindow::on_pushButton_clicked(){
    if (threadStart){
        emit newLogInfo("处理线程已启动。");
    } else{

        disconnect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
        double THRESHOLD = ui->timeEdit->text().toDouble();
        double DISTANCE_THRESHOLD = ui->distanceEdit->text().toDouble();
        setRsu(THRESHOLD, DISTANCE_THRESHOLD);
        ui->pushButton->setText("Stop");
        connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_Stop_clicked()));

        connect(processThread, SIGNAL(newLogInfo(QString)), this, SLOT(showLog(QString)));
        /***在地图上标点*******/
        connect(processThread, SIGNAL(newVehicleOne(double,double)), this, SLOT(setCarOneNowPosition(double,double)));
        connect(processThread, SIGNAL(newVehicleTwo(double,double)), this, SLOT(setCarTwoNowPosition(double,double)));

        threadStart = true;
    }

}

void MainWindow::on_pushButton_Stop_clicked(){
    if (threadStart){
        disconnect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_Stop_clicked()));
        if(processThread != nullptr){
            processThread->exit();
            processThread = nullptr;
            ui->textLog->append(messageThread+"stoped.");
            addLogToDB(messageThread +"stoped.");
            disconnect(this, SIGNAL(newMessage(QJsonObject)), processThread, SLOT(addMessage(QJsonObject)));
            disconnect(processThread, SIGNAL(newComputable(QList<QJsonObject>)), processThread, SLOT(computerResult(QList<QJsonObject>)));
            disconnect(processThread, SIGNAL(sendResult(QJsonObject)),this, SLOT(showResult(QJsonObject)));
            disconnect(processThread, SIGNAL(sendResult(QJsonObject)), this, SLOT(onSendMessage(QJsonObject)));

            disconnect(processThread, SIGNAL(newLogInfo(QString)), this, SLOT(showLog(QString)));

            /***在地图上标点*******/
            disconnect(processThread, SIGNAL(newVehicleOne(double,double)), this, SLOT(setCarOneNowPosition(double,double)));
            disconnect(processThread, SIGNAL(newVehicleTwo(double,double)), this, SLOT(setCarTwoNowPosition(double,double)));
        }
        ui->pushButton->setText("Confirm");

        connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));

        threadStart = false;
    } else {
       emit newLogInfo("处理线程已停止。");
    }


}

void MainWindow::setRsu(const double &THRESHOLD, const double &DISTANCE_THRESHOLD){
    /************
     * 启动处理线程
     * *********/
    QJsonObject rsuLocation;
    rsuLocation.insert("THRESHOLD", QString::number(THRESHOLD));
    rsuLocation.insert("DISTANCE_THRESHOLD", QString::number(DISTANCE_THRESHOLD));
    if(processThread == nullptr){
        processThread = new DataProcessThread(rsuLocation);
        processThread->start();
        ui->textLog->append(messageThread+"processThread is started.");
        addLogToDB(messageThread+"started.");
        connect(this, SIGNAL(newMessage(QJsonObject)), processThread, SLOT(addMessage(QJsonObject)));
        connect(processThread, SIGNAL(newComputable(QList<QJsonObject>)), processThread, SLOT(computerResult(QList<QJsonObject>)));
        connect(processThread, SIGNAL(sendResult(QJsonObject)),this, SLOT(showResult(QJsonObject)));
        connect(processThread, SIGNAL(sendResult(QJsonObject)), this, SLOT(onSendMessage(QJsonObject)));

    }

}



/***********************************
 * 收到消息记录时间戳
 * 客户端发送过来的时间戳
 * 将接收到的消息显示出来并将其发送到DataProcessThread
 * @brief MainWindow::onUdpAppendMessage
 * @param $from
 * @param message
 */
void MainWindow::onUdpAppendMessage(const QString &from, const QJsonObject &message){

    QString type = message.find("type").value().toString();
    /*****************
     * QString.compare 当字符串相等时返回0
     * ****************/
    if (type.compare(TYPE_MESSAGE) == 0){
        int         id               = message.find("id").value().toString().toInt();
        emit newLogInfo(message.find("id").value().toString()+"发来一条普通消息");
        long long   timeStamp        = message.find("timeStamp").value().toString().toLongLong();
        float       direction        = message.find("direction").value().toString().toFloat();
        double      lat              = message.find("lat").value().toString().toDouble();
        double      lon              = message.find("lon").value().toString().toDouble();
        float       speed            = message.find("speed").value().toString().toFloat();
        double      acc              = message.find("acc").value().toString().toDouble();

        ui->label_from->setText(from);
        ui->label_id->setText(QString::number(id));
        ui->label_timeStamp->setText(QString::number(timeStamp));
        ui->label_speed->setText(QString::number(speed,10,10));
        ui->label_direction->setText(QString::number(direction));
        ui->label_Lat->setText(QString::number(lat,10,10));
        ui->label_lon->setText(QString::number(lon,10,10));
        ui->label_acc->setText(QString::number(acc,10,10));

        emit newMessage(message);

    } else if (type.compare(TYPE_TIME_SYNC_MESSAGE) == 0){
        long long  receiverTimeStamp = getTimeStamp();
        int         id               = message.find("id").value().toString().toInt();
        emit newLogInfo(message.find("id").value().toString()+"发来了时间同步请求");
        long long  timeStamp        = message.find("timeStamp").value().toString().toLongLong();

        QJsonObject result;
        result.insert("id", id);
        result.insert("type",TYPE_TIME_SYNC_RESULT);
        result.insert("timeStamp", timeStamp);
        result.insert("receiverTimeStamp", receiverTimeStamp);
        long long sendTimeStamp = getTimeStamp();
        result.insert("sendTimeStamp", sendTimeStamp);
        result.insert("time",ERROR_VALUE);
        result.insert("distance",ERROR_VALUE);
        result.insert("warning", false);

        udpTargetAddr.setAddress("192.168.1.80");
        udpTargetPort = 4040;
        myudp->sendMessage(udpTargetAddr, udpTargetPort, result);

    } else if(type.compare(TYPE_WIFI_TIME_SYNC_RESULT) == 0){
        long long sendTimeStamp = message.find("timeStamp").value().toString().toLongLong();
        long long timeStamp = message.find("acc").value().toString().toDouble();
        long long nowTimeStamp = getTimeStamp();
        long long delay = (nowTimeStamp - timeStamp) / 2;
        long long serverTimeStamp = sendTimeStamp + delay;
        this->baseTimeStamp = nowTimeStamp;
        this->serverTimeStamp = serverTimeStamp;
        emit newLogInfo("TIME_SYNC_SUCCESS");
        emit newLogInfo("sendTimeStamp: "+QString::number(sendTimeStamp));
        emit newLogInfo("TimeStamp: "+QString::number(timeStamp));

    } else if(type.compare(TYPE_WIFI_MESSAGE) == 0){
        int         messageId        = message.find("id").value().toString().toInt();
        emit newLogInfo(message.find("id").value().toString()+"发来一条WiFi消息");
        long long   timeStamp        = message.find("timeStamp").value().toString().toLongLong();
        float       direction        = message.find("direction").value().toString().toFloat();
        double      lat              = message.find("lat").value().toString().toDouble();
        double      lon              = message.find("lon").value().toString().toDouble();
        float       speed            = message.find("speed").value().toString().toFloat();
        double      acc              = message.find("acc").value().toString().toDouble();
        ui->label_from->setText(from);
        ui->label_id->setText(QString::number(messageId));
        ui->label_timeStamp->setText(QString::number(timeStamp));
        ui->label_speed->setText(QString::number(speed,10,10));
        ui->label_direction->setText(QString::number(direction));
        ui->label_Lat->setText(QString::number(lat,10,10));
        ui->label_lon->setText(QString::number(lon,10,10));
        ui->label_acc->setText(QString::number(acc,10,10));

        long long receiverStamp = getTimeStamp();

        if(this->baseTimeStamp != ERROR_VALUE && this->serverTimeStamp != ERROR_VALUE){
            receiverStamp = serverTimeStamp + receiverStamp  - baseTimeStamp;
            emit newLogInfo("Time Synced");
        }

        long long delay = receiverStamp - timeStamp;

        QSqlQuery query;
        query.prepare("INSERT INTO message(messageID, speed, direction, acc, lon, lat, timeStamp, receiverStamp, delay)" "VALUES(:messageID, :speed, :direction, :acc, :lon, :lat, :timeStamp, :receiverStamp, :delay)");
        query.bindValue(":messageID", messageId);
        query.bindValue(":timeStamp", timeStamp);
        query.bindValue(":speed", speed);
        query.bindValue(":direction", direction);
        query.bindValue(":acc", acc);
        query.bindValue(":lon", lon);
        query.bindValue(":lat", lat);
        query.bindValue(":receiverStamp", receiverStamp);
        query.bindValue(":delay", delay);
        if(!query.exec()){
            emit newLogInfo("插入Message表失败"+ query.lastError().text());
        } else {
            emit newLogInfo("插入Message表成功");
        }


    } else{
        emit newLogInfo("消息类型出错");
    }



}


/**
 * 通过UDP发送QJsonObject result
 * @brief MainWindow::onUdpSendMessage
 * @param result
 */
void MainWindow::onSendMessage(const QJsonObject &result){

    udpTargetAddr.setAddress("192.168.1.80");
    udpTargetPort = 4040;
    myudp->sendMessage(udpTargetAddr, udpTargetPort, result);
    addResultToDB(result, true);

//    bool isWarning = result.find("warning").value().toBool();
//    int id = result.find("id").value().toInt();
//    if(idOne == ERROR_VALUE){
//        idOne = id;
//        if(warningStatusOne ^ isWarning){
//            warningStatusOne = isWarning;
//            udpTargetAddr.setAddress("192.168.1.80");
//            udpTargetPort = 4040;
//            myudp->sendMessage(udpTargetAddr, udpTargetPort, result);
//            addResultToDB(result, true);
//        }
//    } else{ //idOne != ERROR_VALUE
//        if(idOne == id){   //id = idOne
//            if(warningStatusOne ^ isWarning){
//                warningStatusOne = isWarning;
//                udpTargetAddr.setAddress("192.168.1.80");
//                udpTargetPort = 4040;
//                myudp->sendMessage(udpTargetAddr, udpTargetPort, result);
//                addResultToDB(result, true);
//            }
//        }else{             //id != idOne and idOne != 666
//            if(idTwo == ERROR_VALUE){
//                idTwo = id;
//                if(warningStatusTwo ^ isWarning){
//                    warningStatusTwo = isWarning;
//                    udpTargetAddr.setAddress("192.168.1.80");
//                    udpTargetPort = 4040;
//                    myudp->sendMessage(udpTargetAddr, udpTargetPort, result);
//                    addResultToDB(result, true);
//                }
//            }else if(idTwo == id){
//                if(warningStatusTwo ^ isWarning){
//                    warningStatusTwo = isWarning;
//                    udpTargetAddr.setAddress("192.168.1.80");
//                    udpTargetPort = 4040;
//                    myudp->sendMessage(udpTargetAddr, udpTargetPort, result);
//                    addResultToDB(result, true);
//                }
//            } else{
//                emit newLogInfo("onSendMessage ERROR");
//            }
//        }
//    }

}

/***************************************
 * 显示地图UI界面
 * QT与JS进行通信的模块
 * **************************************/


void MainWindow::setCarOneNowPosition(const double &lon, const double &lat){
    QString demand = "addGreenPoint(";
    demand.append("new BMap.Point(");
    demand.append(QString::number(lon));
    demand.append(", ");
    demand.append(QString::number(lat));
    demand.append("))");
    parentWindow->dynamicCall("execScript(QString,QString)", demand,"JavaScript");

}

void MainWindow::setCarTwoNowPosition(const double &lon, const double &lat){
    QString demand = "addBulePoint(";
    demand.append("new BMap.Point(");
    demand.append(QString::number(lon));
    demand.append(", ");
    demand.append(QString::number(lat));
    demand.append("))");
    parentWindow->dynamicCall("execScript(QString,QString)", demand,"JavaScript");
}

/**
 * 显示轨迹槽函数
 * @brief MainWindow::on_btn_show_tar_clicked
 */
void MainWindow::on_btn_show_tar_clicked()
{
    QString demand = "showGreenTrance(); showBlueTrance();";
    parentWindow->dynamicCall("execScript(QString,QString)", demand,"JavaScript");
}

/**
 * 清除轨迹槽函数
 * @brief MainWindow::on_btn_clear_tar_clicked
 */
void MainWindow::on_btn_clear_tar_clicked()
{
    QString demand = "removeGreenTrance(); removeBlueTrance();";
    parentWindow->dynamicCall("execScript(QString,QString)", demand,"JavaScript");
}


/*****************************
 * UI显示部分
 * *************************/
void MainWindow::showResult(const QJsonObject &result){
    ui->editID->setText(QString::number(result.find("id").value().toInt()));
    ui->editDistance->setText(QString::number(result.find("distance").value().toDouble()));
    ui->editTime->setText(QString::number(result.find("time").value().toDouble()));
    ui->editWarning->setText(QString::number(result.find("warning").value().toBool()));
}

void MainWindow::showLog(const QString &logInfo){
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString message = currentDateTime.toString();
    message.append("  ");
    message.append(logInfo);
    //this->saveFile(message);
    ui->textLog->append(message);
}


/*****************************
 * 数据库存入部分
 * ***************************/
/**
 * 向数据库存入Message
 * @brief MainWindow::addMessageToDB
 * @param message
 * @return
 */
bool MainWindow::addMessageToDB(const QJsonObject &message){
    int messageId = message.find("id").value().toString().toInt();
    long long timeStamp = message.find("timeStamp").value().toString().toLongLong();
    float speed = message.find("speed").value().toString().toFloat();
    float direction = message.find("direction").value().toString().toFloat();
    double acc = message.find("acc").value().toString().toDouble();
    double lon = message.find("lon").value().toString().toDouble();
    double lat = message.find("lat").value().toString().toDouble();

    long long receiverStamp = getTimeStamp();

    long long delay = receiverStamp - timeStamp;

    emit newLogInfo("delay ="+ QString::number(delay));

    QSqlQuery query;
    query.prepare("INSERT INTO message(messageID, speed, direction, acc, lon, lat, timeStamp, receiverStamp, delay)" "VALUES(:messageID, :speed, :direction, :acc, :lon, :lat, :timeStamp, :receiverStamp, :delay)");
    query.bindValue(":messageID", messageId);
    query.bindValue(":timeStamp", timeStamp);
    query.bindValue(":speed", speed);
    query.bindValue(":direction", direction);
    query.bindValue(":acc", acc);
    query.bindValue(":lon", lon);
    query.bindValue(":lat", lat);
    query.bindValue(":receiverStamp", receiverStamp);
    query.bindValue(":delay", delay);
    if(!query.exec()){
        emit newLogInfo("插入Message表失败"+ query.lastError().text());
        return false;
    } else {
        emit newLogInfo("插入Message表成功");
        return true;
    }
}

/**
 * 将Result保存到数据库中，发送一个保存一个
 * @brief MainWindow::addResultToDB
 * @param result
 * @param sended
 * @return
 */
bool MainWindow::addResultToDB(const QJsonObject &result, bool sended){
    int resultID = result.find("id").value().toInt();
    int time = result.find("time").value().toInt();
    double distance = result.find("distance").value().toDouble();
    bool warning = result.find("warning").value().toBool();
    long long sendStamp = getTimeStamp();

    QSqlQuery query;
    query.prepare("INSERT INTO result(resultID, time, distance, warning, sendStamp, sended)" "VALUES(:resultID, :time, :distance, :warning, :sendStamp, :sended)");
    query.bindValue(":resultID", resultID);
    query.bindValue(":time", time);
    query.bindValue(":distance", distance);
    query.bindValue(":warning", warning);
    query.bindValue(":sendStamp", sendStamp);
    query.bindValue(":sended", sended);
    if(!query.exec()){
        emit newLogInfo("插入Result表失败"+ query.lastError().text());
        return false;
    } else {
        emit newLogInfo("插入Result表成功");
        return true;
    }
}

/**
 * 向数据库存入Log信息
 * @brief MainWindow::addLogToDB
 * @param logInfo
 * @return
 */
bool MainWindow::addLogToDB(const QString &logInfo){
    long long timeStamp = getTimeStamp();
    QString data = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QSqlQuery query;
    query.prepare("INSERT INTO log(timeStamp, data, context)" "VALUES(:timeStamp, :data, :context)");
    query.bindValue(":timeStamp", timeStamp);
    query.bindValue(":data", data);
    query.bindValue(":context",logInfo);
    if(!query.exec()){
        emit newLogInfo("插入Log表失败"+ query.lastError().text());
        return false;
    } else {
        emit newLogInfo("插入Log表成功");
        return true;
    }
}

/*************************************************
 *
 * 此函数为连接AP的PC端与Android进行同步
 * 将发送同步请求
 *
 * *************************************************/
void MainWindow::on_btn_time_sync_clicked()
{
    QJsonObject result;
    result.insert("id", ERROR_VALUE);
    result.insert("type","TYPE_WIFI_TIME_SYNC");
    result.insert("warning", false);
    result.insert("timeStamp",ERROR_VALUE);
    result.insert("receiverTimeStamp", ERROR_VALUE);
    result.insert("distance", ERROR_VALUE);
    result.insert("time", ERROR_VALUE);
    result.insert("sendTimeStamp", getTimeStamp());

    udpTargetAddr.setAddress("192.168.1.83");
    udpTargetPort = 4040;
    myudp->sendMessage(udpTargetAddr, udpTargetPort, result);
    addResultToDB(result, true);
}
