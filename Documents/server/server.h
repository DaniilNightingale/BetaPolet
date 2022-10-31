#ifndef SERVER_H
#define SERVER_H
#include<QTcpServer>
#include<QTcpSocket>
#include<QVector>
#include<QTime>
#include <QFile>
#include <QTextStream>
#include <thread>

class Server : public QTcpServer
{
    Q_OBJECT

public:
  Server();
  QTcpSocket *socket;

private:
  QVector <QTcpSocket*> Sockets; //для подключения нескольких сокетов
  QByteArray Data;               //для обмена данных
  void SendToClient(QString str);//метод отправки ответа назад к клиенту(срабатывает после получения посылки сервером)
  quint16 nextBlockSize;         //на тот случай если все данные не смогут уместиться в одной посылке, подсчитывает размер отправляемых данных

public slots:                    //сокет динамический, с использованием QT слотов и сигналов
     void incomingConnection(qintptr socketDescriptor);//метод для сопрежения сокетов клиента и сервера, устанавливает сообщение между ними, постоянно слушает порты
     void slotReadyRead();                             //там самая объемная часть кода, после сопрежения клиента и сервера отвечает за получение посылок с клиента, производит над ними операции, записывает в файл и вызывает метод для ответа клиенту назад
};

#endif // SERVER_H
