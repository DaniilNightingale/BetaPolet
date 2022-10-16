#include"server.h"
#include<fstream>
#include <QFile>
#include <QTextStream>
#include <thread>
void threadFunction(QFile file,QString str)
{
     // do smth
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
                QTextStream writeStream(&file);
                writeStream << str;

                file.flush();
                file.close();
                //_sleep(1);
        }
}

Server::Server()
{
    if(this->listen(QHostAddress::Any,2323))//QHostAddress("178.178.86.48")
    {
        qDebug()<<"start";
    }
    else
    {
        qDebug()<<"erro";
    }
    nextBlockSize=0;
}
void Server:: incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead,this,&Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected,socket,&QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug()<<"client connected"<<socketDescriptor;
}

void Server::slotReadyRead()
{
    socket=(QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok)
    {
       /* qDebug()<<"read...";
        QString str;
        in>>str;
        qDebug()<<str;
        SendToClient(str);*/
        for(;;)
        {
            if(nextBlockSize==0)
            {
                 qDebug()<<"nextBlockSize=0";
                if(socket->bytesAvailable()<2)
                {
                    qDebug()<<"Data<2, break";
                    break;
                }
                in>>nextBlockSize;
                 qDebug()<<"nextBlockSize="<<nextBlockSize;
            }
            if(socket->bytesAvailable()<nextBlockSize)
            {
                  qDebug()<<"Data not full, break=";
                break;
            }
            QString str;
            QTime time;
            in>>str;
            nextBlockSize=0;
            qDebug()<<str;
             QFile file("C:/Users/denso/Documents/server/path.txt");
           /* if(file.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                        QTextStream writeStream(&file);
                        writeStream << str;

                        file.flush();
                        file.close();
                        //_sleep(1);
                }*/
             std::thread thr([& in,& file,& str](){while(!(file.open(QIODevice::WriteOnly | QIODevice::Text)))
                 {}
                         QTextStream writeStream(&file);
                         writeStream << str;

                         file.flush();
                         file.close();
                         //_sleep(1);
                 });
            thr.join();
            SendToClient(str);
            break;
        }
    }
    else
    {
        qDebug()<<"DataStream error";
    }
}
      void Server::SendToClient(QString str)
        {
            Data.clear();
            QDataStream out(&Data,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_2);
            out<<quint16(0)<<QTime::currentTime()<<str;
            out.device()->seek(0);
            out<<quint16(Data.size() - sizeof(quint16));
            //socket->write(Data); для одного
            for(int i=0;i<Sockets.size();i++)
            {
              Sockets[i]->write(Data);
            }
        }

