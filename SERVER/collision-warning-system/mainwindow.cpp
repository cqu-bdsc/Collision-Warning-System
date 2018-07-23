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
    //connect(ui->editMessage, SIGNAL(returnPressed()), this, SLOT(onUdpSendMessage()));

    connect(ui->but_start, SIGNAL(clicked()), this, SLOT(on_but_start_clicked()));
    connect(ui->but_getIP, SIGNAL(clicked()), this, SLOT(on_but_getIP_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/************************************************
 * 初始化UI界面
 **************************************************/

void MainWindow::initUI(){
    //加载LOGO
    QString logoPath = ":/image/logo.png";
    QPixmap logo(logoPath);
    ui->label_logo->setPixmap(logo);
    ui->label_logo->resize(logo.width(),logo.height());

   //tableFormat.setBorder(0);

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
        ui->but_start->setText("停止");

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

    ui->textLog->append(messageUDP + "Stoped.");
    //解除槽函数绑定
    disconnect(myudp, SIGNAL(newMessage(QString, QJsonObject)), this, SLOT(onUdpAppendMessage(QString, QJsonObject)));

    ui->but_start->setText("开始");
    myudp->unbindPort();
    //重新绑定槽函数
    connect(ui->but_start, SIGNAL(clicked()), this, SLOT(on_but_start_clicked()));
}

/**
 * 将接收到的消息显示出来
 * @brief MainWindow::onUdpAppendMessage
 * @param $from
 * @param message
 */
void MainWindow::onUdpAppendMessage(const QString &from, const QJsonObject &message){

//    if(from == "System"){
//        QColor color = ui->textReceive->textColor();
//        ui->textReceive->setTextColor(Qt::gray);
//        ui->textReceive->append(message);
//        ui->textReceive->setTextColor(color);
//    } else{
//        ui->textReceive->append("<"+ from + ">   "+ message);
//    }
//    QScrollBar *bar = ui->textReceive->verticalScrollBar();
//    bar->setValue(bar->maximum());

    int    id           = message.find("id").value().toInt();
    int    timeStamp    = message.find("timeStamp").value().toInt();
    int    direction    = message.find("direction").value().toInt();
    double    lat          = message.find("lat").value().toDouble();
    double    lon          = message.find("lon").value().toDouble();
    double    speed        = message.find("speed").value().toDouble();
    double    acc          = message.find("acc").value().toDouble();

    ui->label_from->setText(from);
    ui->label_id->setText(QString::number(id));
    ui->label_timeStamp->setText(QString::number(timeStamp));
    ui->label_speed->setText(QString::number(speed));
    ui->label_direction->setText(QString::number(direction));
    ui->label_Lat->setText(QString::number(lat));
    ui->label_lon->setText(QString::number(lon));
    ui->label_acc->setText(QString::number(acc));
}

/**
 * 通过udp发送消息槽函数
 * @brief MainWindow::onUdpSendMessage
 */
void MainWindow::onUdpSendMessage(){
  //  QString text = ui->editMessage->text();
  //  if(text.isEmpty()){
  //      return;
  //  }

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
        warning = "22233";
    }

    QJsonObject message;
    message.insert("id", id);
    message.insert("time", time);
    message.insert("distance", distance);
    message.insert("warning", warning);

    udpTargetAddr.setAddress(ui->editSendIP->text());
    udpTargetPort = ui->editSendPort->text().toInt();
    myudp->sendMessage(udpTargetAddr, udpTargetPort, message);

    //onUdpAppendMessage("ME", text);
    //ui->editMessage->clear();

    ui->textLog->append("ME send:" + QString(QJsonDocument(message).toJson()));
    ui->editID->clear();
    ui->editTime->clear();
    ui->editDistance->clear();
    ui->editWarning->clear();

}
