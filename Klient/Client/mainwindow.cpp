#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectBtnHit);
    connect(ui->goButton, &QPushButton::clicked, this, &MainWindow::goBtnHit);
    connect(ui->muteButton, &QPushButton::clicked, this, &MainWindow::muteChange);
    connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::songUp);
    connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::songDown);
    connect(ui->hideButton, &QPushButton::clicked, this, &MainWindow::hideChange);
    connect(ui->loopButton, &QPushButton::clicked, this, &MainWindow::loopChange);
    connect(ui->randomButton, &QPushButton::clicked, this, &MainWindow::randomChange);
    connect(ui->skipButton, &QPushButton::clicked, this, &MainWindow::skip);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectBtnHit(){
    sock = new QTcpSocket(this);
    sock->socketOption(QAbstractSocket::KeepAliveOption);
    connTimeoutTimer = new QTimer(this);
    connTimeoutTimer->setSingleShot(true);
    connect(connTimeoutTimer, &QTimer::timeout, [&]{
        sock->abort();
        sock->deleteLater();
        connTimeoutTimer->deleteLater();
        ui->textEdit->append("<b>Connect timed out</b>");
    });
    connect(sock, &QTcpSocket::connected, this, &MainWindow::socketConnected);
    connect(sock, &QTcpSocket::readyRead, this, &MainWindow::socketRecive);
    connect(sock, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnect);
    connect(sock, static_cast<void(QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error), this, &MainWindow::socketError);
    std::string text = ui->textEdit->toPlainText().toStdString();
    sock->connectToHost(QString::fromStdString(text.substr(0, text.find(":"))), std::stoi(text.substr(text.find(":")+1)));
    connTimeoutTimer->start(3000);
}

void MainWindow::socketError(QTcpSocket::SocketError err){
    if(err == QTcpSocket::RemoteHostClosedError)
        return;
    ui->connectGroup->setEnabled(false);
    ui->radioGroup_2->setEnabled(false);
    QMessageBox::critical(this, "Error", sock->errorString());

    //ui->textEdit->append("<b>Socket error: "+sock->errorString()+"</b>");
}

void MainWindow::socketDisconnect(){
    ui->connectGroup->setEnabled(false);
    ui->radioGroup_2->setEnabled(false);
    QMessageBox::critical(this, "Error", "<b>Disconnected</b>");
    //ui->textEdit->append("<b>Disconnected</b>");
}

void MainWindow::goBtnHit(){
    if(ui->rooms->currentItem()){
        std::string s = ui->rooms->currentItem()->text().toStdString();
        currRoom = s;
        s = "room:"+s;
        ui->queueGroup->setEnabled(true);
        sock->write(s.c_str());
    }
}

void MainWindow::socketConnected(){
    connTimeoutTimer->stop();
    connTimeoutTimer->deleteLater();
    ui->textEdit->append("<b>Connected</b>");
    ui->radioGroup_2->setEnabled(true);
    ui->connectGroup->setEnabled(false);
    ui->queueGroup->setEnabled(false);
    udpsock = new QUdpSocket();
    udpsock->bind(sock->localAddress(), sock->localPort(), QAbstractSocket::ShareAddress);
    std::cout<<"tcp"<<sock->localAddress().toString().toStdString()<<sock->localPort()<<std::endl;
    std::cout<<"udp"<<udpsock->localAddress().toString().toStdString()<<udpsock->localPort()<<std::endl;
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    //format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);

    output = new QAudioOutput(format);
    //output->setBufferSize(256);
    device = output->start();
    connect(udpsock, &QUdpSocket::readyRead, this, &MainWindow::udpSocketRecive);

}

void MainWindow::udpSocketRecive(){
    while (udpsock->hasPendingDatagrams())
        {
            QByteArray data;
            data.resize(udpsock->pendingDatagramSize());
            udpsock->readDatagram(data.data(), data.size());
            device->write(data.data(), data.size());
        }
}

void MainWindow::socketRecive(){
    QByteArray ba = sock->readAll();
    QString qtext = QString::fromUtf8(ba).trimmed();
    std::string text = qtext.toStdString();
    std::cout<<text<<std::endl;
    int command = this->getCmd(text);
    switch(command){
    case czero:
        QMessageBox::critical(this, "Error", "<b>Bad request error.</b>");
        //ui->textEdit->append("Bad request");
        break;
    case crooms:
        //text = std::string(&text[6], &text[text.length()]);
        text = text.substr(6);
        while(true){
            std::size_t found = text.find(",");
            if(found!=std::string::npos){
                ui->rooms->addItem(QString::fromStdString(std::string(&text[0], &text[found])));
                text = std::string(&text[found+1], &text[text.length()]);
            }
            else {
                ui->rooms->addItem(QString::fromStdString(text));
                break;
            }
        }
        break;
    case cqueue:
    {
        text = std::string(&text[6], &text[text.length()]);
        ui->queue->clear();
        int ile = 0;
                while(true){
                    std::size_t found = text.find(",");
                    if(found!=std::string::npos){
                        std::size_t sred= text.find(";"), wykrz = text.find("!"), zap = text.find("?");
                        ui->queue->addItem(QString::fromStdString(std::string(&text[0], &text[sred])));
                        if(text.substr(sred+1, 3)=="yes"){
                            ui->queue->item(ile)->setTextColor(Qt::black);
                        }
                        else{
                            ui->queue->item(ile)->setTextColor(Qt::gray);
                        }
                        if(text.substr(wykrz+1, 3)=="yes"){
                            ui->queue->item(ile)->setBackgroundColor(Qt::green);
                        }
                        else{
                            ui->queue->item(ile)->setBackgroundColor(Qt::white);
                        }
                        if(std::string(&text[0], &text[sred])==text.substr(zap+1)){
                            ui->queue->item(ile)->setTextAlignment(Qt::AlignRight);
                        }
                        text = std::string(&text[found+1], &text[text.length()]);
                        ile++;
                    }
                    else {
                        std::size_t sred= text.find(";"), wykrz = text.find("!"), zap = text.find("?");
                        ui->queue->addItem(QString::fromStdString(std::string(&text[0], &text[sred])));
                        if(text.substr(sred+1, 3)=="yes"){
                            ui->queue->item(ile)->setTextColor(Qt::black);
                        }
                        else{
                            ui->queue->item(ile)->setTextColor(Qt::gray);
                        }
                        if(text.substr(wykrz+1, 3)=="yes"){
                            ui->queue->item(ile)->setBackgroundColor(Qt::green);
                        }
                        else{
                            ui->queue->item(ile)->setBackgroundColor(Qt::white);
                        }
                        if(std::string(&text[0], &text[sred])==text.substr(zap+1)){
                            ui->queue->item(ile)->setTextAlignment(Qt::AlignRight);
                        }
                        break;
                    }
                }
        break;
    }
    case ccurr:{
        std::size_t dwu= text.find(":");
        for(int i = 0; i < ui->queue->count(); i++){
            if(ui->queue->item(i)->text().toStdString()==text.substr(dwu+1)){
                ui->queue->item(i)->setTextAlignment(Qt::AlignRight);
            }
            else{
                ui->queue->item(i)->setTextAlignment(Qt::AlignLeft);
            }
        }
        break;
    }
    case chide:
        std::cout<<"hide"<<std::endl;
        for(int i = 0; i < ui->queue->count(); i++){
            if(ui->queue->item(i)->text().toStdString()==text.substr(5, text.find(";")-5)){
                if(text.substr(text.find(";")+1)=="yes"){
                    ui->queue->item(i)->setTextColor(Qt::black);
                }
                else{
                    ui->queue->item(i)->setTextColor(Qt::gray);
                }
            }
        }
        break;
    case cloop:
        std::cout<<"loop"<<std::endl;
        for(int i = 0; i < ui->queue->count(); i++){
            if(ui->queue->item(i)->text().toStdString()==text.substr(5, text.find(";")-5)){
                if(text.substr(text.find(";")+1)=="yes"){
                    ui->queue->item(i)->setBackgroundColor(Qt::green);
                }
                else{
                    ui->queue->item(i)->setBackgroundColor(Qt::white);
                }
            }
        }
        break;
    }
}

void MainWindow::songUp(){
    if(ui->queue->currentItem()){
        std::string s = ui->queue->currentItem()->text().toStdString();
        s = "up:"+currRoom+";"+s;
        sock->write(s.c_str());
    }
}

void MainWindow::songDown(){
    if(ui->queue->currentItem()){
        std::string s = ui->queue->currentItem()->text().toStdString();
        s = "down:"+currRoom+";"+s;
        sock->write(s.c_str());
    }
}

void MainWindow::muteChange(){
    std::string s = "mute:";
    sock->write(s.c_str());
}

void MainWindow::hideChange(){
    std::string s = ui->queue->currentItem()->text().toStdString();
    s = "hide:"+currRoom+";"+s;
    sock->write(s.c_str());
}

void MainWindow::loopChange(){
    if(ui->queue->currentItem()!=nullptr){
        std::string s = ui->queue->currentItem()->text().toStdString();
        s = "loop:"+currRoom+";"+s;
        sock->write(s.c_str());
    }
}

void MainWindow::randomChange(){
    std::string s = "random:"+currRoom+";";
    sock->write(s.c_str());
}

void MainWindow::skip(){
    std::string s = "skip:"+currRoom;
    sock->write(s.c_str());
}

int MainWindow::getCmd(std::string cmd){
    int index = cmd.find(":");
    if (std::string(&cmd[0], &cmd[index])=="rooms"){
        return crooms;
    }
    else if(std::string(&cmd[0], &cmd[index])=="queue"){
        return cqueue;
    }
    else if(std::string(&cmd[0], &cmd[index])=="hide"){
        return chide;
    }
    else if(std::string(&cmd[0], &cmd[index])=="loop"){
        return cloop;
    }
    else if(std::string(&cmd[0], &cmd[index])=="curr"){
        return ccurr;
    }
    else return czero;
}
