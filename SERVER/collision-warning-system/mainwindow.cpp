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
    //绑定发送UDP报文事件
    connect(ui->but_send, SIGNAL(clicked()), this, SLOT(onUdpSendMessage()));

    connect(ui->but_start, SIGNAL(clicked()), this, SLOT(on_but_start_clicked()));
    connect(ui->but_getIP, SIGNAL(clicked()), this, SLOT(on_but_getIP_clicked()));

    connect(this, SIGNAL(newLogInfo(QString)), this, SLOT(showLog(QString)));
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
    QString webstr=QString("file:///C:/QtProject/ChongqingOfflineMap/offlinemap_demo/demo/1_0.html");//设置要打开的网页
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

}


/***********************************************
 * Network Interface 模块
 * 获取本地IP地址
 ************************************************/

/**
 * 刷新按钮单击事件响应槽函数
 * @brief MainWindow::on_but_getIP_clicked
 */
void MainWindow::on_but_getIP_clicked()
{
    findLocalIP();
}

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
    //but_start 按钮单击事件与本槽函数解除绑定
    disconnect(ui->but_start, SIGNAL(clicked()), this, SLOT(on_but_start_clicked()));

    //开始监听
    if(setupConnection()){
        ui->textLog->append(messageUDP + "listerning to "
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

    udpListenPort = ui->editLocalPort->text().toInt();
    isSuccess = myudp->bindPort(localAddr, udpListenPort);
    return isSuccess;
}

/**
 * 开始监听后，开始按钮变成停止按钮，停止按钮点击事件槽函数
 * @brief MainWindow::onUdpStopButtonClicked
 */
void MainWindow::onUdpStopButtonClicked(){
    disconnect(ui->but_start, SIGNAL(clicked()), this, SLOT(on_but_start_clicked()));
    ui->textLog->append(messageUDP+ "Stoped.");
    //解除槽函数绑定
    disconnect(myudp, SIGNAL(newMessage(QString, QJsonObject)), this, SLOT(onUdpAppendMessage(QString, QJsonObject)));
    ui->but_start->setText("Start");
    myudp->unbindPort();
    //重新绑定槽函数
    connect(ui->but_start, SIGNAL(clicked()), this, SLOT(on_but_start_clicked()));
}

/**
 * 将接收到的消息显示出来并将其发送到DataProcessThread
 * @brief MainWindow::onUdpAppendMessage
 * @param $from
 * @param message
 */
void MainWindow::onUdpAppendMessage(const QString &from, const QJsonObject &message){

    int         id               = message.find("id").value().toString().toInt();
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

}

/**
 * 通过udp发送消息槽函数
 * @brief MainWindow::onUdpSendMessage
 */
void MainWindow::onUdpSendMessage(){

    QString id = ui->editID->text();
    if(id.isEmpty()){
        id = "22233";
    }
    QString time = ui->editTime->text();
    if(time.isEmpty()){
        time = "22233";
    }
    QString distance = ui->editDistance->text();
    if(distance.isEmpty()){
        distance = "22233";
    }
    QString warning = ui->editWarning->text();
    if(warning.isEmpty()){
        warning = "";
    }

    QJsonObject message;
    message.insert("id", id);
    message.insert("time", time);
    message.insert("distance", distance);
    message.insert("warning", true);

    udpTargetAddr.setAddress(ui->editSendIP->text());
    udpTargetPort = ui->editSendPort->text().toInt();
    myudp->sendMessage(udpTargetAddr, udpTargetPort, message);

    ui->textLog->append("ME send:" + QString(QJsonDocument(message).toJson()));
    ui->editID->clear();
    ui->editTime->clear();
    ui->editDistance->clear();
    ui->editWarning->clear();

}

/**
 * 通过UDP发送QJsonObject result
 * @brief MainWindow::onUdpSendMessage
 * @param result
 */
void MainWindow::onSendMessageq(const QJsonObject &result){
//    udpTargetAddr.setAddress(ui->editSendIP->text());
//    udpTargetPort = ui->editSendPort->text().toInt();
    bool warning = result.find("warning").value().toBool();
    if(warning){
//        udpTargetAddr.setAddress("192.168.1.80");
//        udpTargetPort = 4040;
//        myudp->sendMessage(udpTargetAddr, udpTargetPort, result);

        udpTargetAddr.setAddress("192.168.43.111");
        udpTargetPort = 4040;
        myudp->sendMessage(udpTargetAddr, udpTargetPort, result);

        udpTargetAddr.setAddress("192.168.43.14");
        udpTargetPort = 4040;
        myudp->sendMessage(udpTargetAddr, udpTargetPort, result);

    }
}

/***************************************
 * 显示地图UI界面
 * QT与JS进行通信的模块
 * **************************************/

void MainWindow::setRsu(const double &rsuLon, const double &rsuLat, const double &timeCrash){
    document = this->ui->axWidget->querySubObject("Document");
    parentWindow = document->querySubObject("parentWindow");
    QString js = "shwoMap(new BMap.Point(";
    js.append(QString::number(rsuLon));
    js.append(",");
    js.append(QString::number(rsuLat));
    js.append(" ))");
    parentWindow->dynamicCall("execScript(QString,QString)", js,"JavaScript");

    /************
     * 启动处理线程
     * *********/
    QJsonObject rsuLocation;
    rsuLocation.insert("lon", QString::number(rsuLon));
    rsuLocation.insert("lat", QString::number(rsuLat));
    rsuLocation.insert("time", QString::number(timeCrash));
    if(processThread == nullptr){
        processThread = new DataProcessThread(rsuLocation);
        processThread->start();
        emit newLogInfo("processThread is started.");
        connect(this, SIGNAL(newMessage(QJsonObject)), processThread, SLOT(addMessage(QJsonObject)));
        connect(processThread, SIGNAL(newComputable(QList<QJsonObject>)), processThread, SLOT(ComputerResult(QList<QJsonObject>)));
        connect(processThread, SIGNAL(sendResult(QJsonObject)),this, SLOT(showResult(QJsonObject)));
        connect(processThread, SIGNAL(sendResult(QJsonObject)), this, SLOT(onSendMessageq(QJsonObject)));
        connect(processThread, SIGNAL(newLogInfo(QString)), this, SLOT(showLog(QString)));
        connect(processThread, SIGNAL(newVehicleOne(double,double)), this, SLOT(setCarOneNowPosition(double,double)));
        connect(processThread, SIGNAL(newVehicleTwo(double,double)), this, SLOT(setCarTwoNowPosition(double,double)));
    }

}

void MainWindow::setCarOneNowPosition(const double &lon, const double &lat){
    QString demand = "addGreendPoint(";
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

void MainWindow::on_pushButton_clicked(){
    double rsuLon = ui->lonEdit->text().toDouble();
    double rsuLat = ui->latEdit->text().toDouble();
    double timeCrash = ui->timeEdit->text().toDouble();
    setRsu(rsuLon, rsuLat, timeCrash);
}

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
    ui->textLog->append(message);
}
