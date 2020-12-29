#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectBtnHit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectBtnHit(){
    sock = new QTcpSocket(this);


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
    //connect(sock, &QTcpSocket::disconnected, this, &MyWidget::socketDisconnect);
    //connect(sock, static_cast<void(QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error), this, &MyWidget::socketError);

    sock->connectToHost("127.0.0.1", 12345);
    connTimeoutTimer->start(3000);
}

void MainWindow::socketConnected(){
    connTimeoutTimer->stop();
    connTimeoutTimer->deleteLater();
    ui->textEdit->append("<b>Connected</b>");
    ui->radioGroup_2->setEnabled(true);
    ui->queueGroup->setEnabled(false);
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
        //ui->rooms->addItem(QString::fromStdString(text));
        break;
    }
     //       append(QString::fromUtf8(ba).trimmed());
    //ui->textEdit->setAlignment(Qt::AlignLeft);
}

int MainWindow::getCmd(std::string cmd){
    printf("co est2");
    int index = cmd.find(":");
    if (std::string(&cmd[0], &cmd[index])=="rooms"){
        return 1;
    }
    else return 0;
}
