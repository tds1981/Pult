#include <iostream>
#include <fstream>
#include <QString>
#include <string>
#include <QDateTime>

struct Set{
    QString IP_Robot;
    int Port_IN;
    int Port_Out;
    int Timer;
    QString mess;
};

void Savelog(QString S) {
    std::ofstream out;                       // поток для записи
    out.open("LogFile.txt", std::ios::app); // окрываем файл для записи
    if (out.is_open())
    {
        QString d = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");
        out << d.toStdString()+"   "+S.toStdString() << std::endl;
     }
}

Set Seting() {
     std::ifstream SetFile("Setting.ini"); // окрываем файл для чтения
     std::string line;
     std::string s1;
     Set Buf;
     if (SetFile.is_open())
     {
         while (getline(SetFile, line))
         {
             s1=line.substr(0,line.find("="));
             if (s1=="IP_Robot") { Buf.IP_Robot=QString(line.substr(line.find("=")+1).c_str());}
             // c_str()- из string в char

             if (s1=="Port_IN")  { Buf.Port_IN=std::stoi(line.substr(line.find("=")+1)); }
             if (s1=="Port_Out")  { Buf.Port_Out=std::stoi(line.substr(line.find("=")+1)); }
             if (s1=="Timer")     { Buf.Timer=std::stoi(line.substr(line.find("=")+1)); }
         }// конец while
     SetFile.close();     // закрываем файл
     Buf.mess = "файл Setting.ini прочитан" ;
     } else Buf.mess = "файл Setting.ini не найден" ;
     return Buf;
} //конец  Seting()


