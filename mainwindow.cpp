#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QtNetwork>
#include "myfunction.cpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

   // connect(udpSocket, SIGNAL(bytesWritten()), SLOT(slotW()));

    ui->setupUi(this);

    Set s1=Seting();
    ui->LogList->addItem(s1.mess+"   IP = "+s1.IP_Robot);
    IProbot=QHostAddress(s1.IP_Robot);
    RobotPort=quint16(s1.Port_Out);

    udpSocket=new QUdpSocket(this);
    udpSocket->bind(RobotPort);
    connect(udpSocket, SIGNAL(readyRead()), SLOT(slotProcessDatagrams()));

    tmr = new QTimer(this); // Создаем объект класса QTimer и передаем адрес переменной
    tmr->setInterval(s1.Timer); // Задаем интервал таймера
    connect(tmr, SIGNAL(timeout()), this, SLOT(SendTimer())); // Подключаем сигнал таймера к нашему слоту
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotProcessDatagrams()
{
    QByteArray baDatagram;
    while(udpSocket->hasPendingDatagrams()) {
        //if (SendFlag) break;
        baDatagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(baDatagram.data(), baDatagram.size());
        QString str2 = baDatagram;
        ui->LogList->addItem(str2);
        ui->label->setText(str2);
        Savelog("Показания датчиков: "+str2);
    } ;

}


void MainWindow::on_pushButton_clicked()
{
    QString ls;
    ls = ui->lineEdit->text();
    ui->LogList->addItem(ls);

    QByteArray str1;
    str1 =ls.toUtf8();
    udpSocket->writeDatagram(str1, IProbot, 57514);

 }

void MainWindow::on_pushButton_2_clicked()
{
   SendUdp(0x01,qint8(ui->Shim->value()),"Вперёд  ", ui->lineEdit->text().toShort());
}

void MainWindow::on_pushButton_3_clicked()
{
    SendUdp(0x02,qint8(ui->Shim->value()),"Назад  ", ui->lineEdit->text().toShort());
}

void MainWindow::on_pushButton_4_clicked()
{
    SendUdp(0x03,qint8(ui->Shim->value()),"Вращение1 ", ui->lineEdit->text().toShort());
}

void MainWindow::on_pushButton_5_clicked()
{
    tmr->blockSignals(true);
    tmr->stop();
    Comand.resize(1);
    Comand[0]=0x07;
    while (!udpSocket->writeDatagram(Comand.data(), Comand.size(), IProbot, 57514))
    {       }
    ui->LogList->addItem("Cтоп  (Отправлено: "+Comand.toHex()+" )");
    Savelog("Cтоп  (Отправлено: "+Comand.toHex()+" )");
}

void MainWindow::on_pushButton_6_clicked()
{
    SendUdp(0x04,qint8(ui->Shim->value()),"Вращение2 ", ui->lineEdit->text().toShort());
}

void MainWindow::on_pushButton_7_clicked()
{
    Comand.resize(1);
    Comand[0]=0x7F;
   // udpSocket->writeDatagram(b, QHostAddress("192.168.1.6"), 57514);
    ui->pushButton_5->setEnabled(true);
}

void MainWindow::on_pushButton_8_clicked()
{
    SendUdp(0x05,qint8(ui->Shim->value()),"Вперёд стаб", ui->lineEdit->text().toShort());
}

void MainWindow::on_pushButton_9_clicked()
{
    SendUdp(0x06,qint8(ui->Shim->value()),"Назад стаб", ui->lineEdit->text().toShort());
}

void MainWindow::SendTimer()
{
    QByteArray b;
    b.resize(1);
    b[0]=0x7F;
    //if (Comand.size()>0) udpSocket->writeDatagram(Comand.data(),Comand.size(), IProbot, 57514);else
    udpSocket->writeDatagram(b, IProbot, 57514);
}

void MainWindow::SendUdp(qint8 Regim, qint8 Shim, QString str1,  unsigned short lim)
{
    tmr->stop();
  //  while(udpSocket->hasPendingDatagrams()) {

    //}
    Comand.resize(4);
    Comand[0]=Regim;
    Comand[1]=Shim;
    if (lim) { Comand[2]=lim; Comand[3]=(lim>>8); Comand[3]=Comand[3]+1; }
    else { Comand[2]=0xff;    Comand[3]=0x7f;}

    udpSocket->writeDatagram(Comand.data(), Comand.size(), IProbot, 57514);
   // udpSocket->writeDatagram(Comand, IProbot, 57514);
    QString s = QString::number(ui->Shim->value());
    ui->LogList->addItem(str1+" ШИМ: "+s+" (Отправлено: "+Comand.toHex()+" )");
    Savelog(str1+" ШИМ: "+s+" (Отправлено: "+Comand.toHex()+" )");
    if (ui->SensorFlag->checkState()) {
        tmr->blockSignals(false);
        tmr->start();
     }

}
