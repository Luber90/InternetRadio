#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <string>

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
    QTimer * connTimeoutTimer;
    void connectBtnHit();

    void socketConnected();
    void socketRecive();
    void socketDisconnect();
    void socketError(QTcpSocket::SocketError a);

    int getCmd(std::string cmd);
private:

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
