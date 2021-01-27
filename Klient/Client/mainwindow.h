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
#include <iostream>
#include <unistd.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
enum Cmd {czero, crooms, cqueue, chide, cloop};
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

    void songUp();
    void songDown();
    void muteChange();
    void hideChange();
    void loopChange();
    void randomChange();
    void skip();
    int getCmd(std::string cmd);
private:
    std::string currRoom;
    QAudioOutput *output;
    QIODevice *device;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
