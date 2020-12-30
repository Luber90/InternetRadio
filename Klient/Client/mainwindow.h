#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QObject>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioFormat>
#include <QTimer>
#include <string>
#include <unistd.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    QTcpSocket * sock;
    QUdpSocket * udpsock;
    QTimer * connTimeoutTimer;
    void connectBtnHit();
    void goBtnHit();

    void socketConnected();
    void socketRecive();
    void udpSocketRecive();
    void socketDisconnect();
    void socketError(QTcpSocket::SocketError err);

    int getCmd(std::string cmd);
private:
    QAudioOutput *output;
    QIODevice *device;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
