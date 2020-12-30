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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectBtnHit(){
    sock = new QTcpSocket(this);
    sock->socketOption(QAbstractSocket::KeepAliveOption);
    sock->bind(QAbstractSocket::ShareAddress);
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
    sock->connectToHost("127.0.0.1", 12345);
    connTimeoutTimer->start(3000);
}

void MainWindow::socketError(QTcpSocket::SocketError err){
    if(err == QTcpSocket::RemoteHostClosedError)
        return;
    QMessageBox::critical(this, "Error", sock->errorString());
    ui->textEdit->append("<b>Socket error: "+sock->errorString()+"</b>");
}

void MainWindow::socketDisconnect(){
    ui->textEdit->append("<b>Disconnected</b>");
}

void MainWindow::goBtnHit(){
    if(ui->rooms->currentItem()){
        std::string s = ui->rooms->currentItem()->text().toStdString();
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
    udpsock->bind(QHostAddress::LocalHost, sock->localPort(), QAbstractSocket::ShareAddress);
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);

    output = new QAudioOutput(format);
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
    int command = this->getCmd(text);
    switch(command){
    case 0:
        ui->textEdit->append("Bad request");
        break;
    case 1:
        text = std::string(&text[6], &text[text.length()]);
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
    case 2:
        text = std::string(&text[6], &text[text.length()]);
                while(true){
                    std::size_t found = text.find(",");
                    if(found!=std::string::npos){
                        ui->queue->addItem(QString::fromStdString(std::string(&text[0], &text[found])));
                        text = std::string(&text[found+1], &text[text.length()]);
                    }
                    else {
                        ui->queue->addItem(QString::fromStdString(text));
                        break;
                    }
                }
        break;
    }
}

int MainWindow::getCmd(std::string cmd){
    int index = cmd.find(":");
    if (std::string(&cmd[0], &cmd[index])=="rooms"){
        return 1;
    }
    else if(std::string(&cmd[0], &cmd[index])=="queue"){
        return 2;
    }
    else return 0;
}
