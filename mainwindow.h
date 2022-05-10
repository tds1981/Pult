#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtNetwork/QtNetwork>
#include <QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void slotProcessDatagrams();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void SendTimer();

    void SendUdp(qint8 Regim, qint8 Shim, QString str1, unsigned short lim);

private:
    Ui::MainWindow *ui;
    QUdpSocket* udpSocket;
    QTimer *tmr;
    QByteArray Comand;
    QHostAddress IProbot;
    quint16 RobotPort;
};

#endif // MAINWINDOW_H
