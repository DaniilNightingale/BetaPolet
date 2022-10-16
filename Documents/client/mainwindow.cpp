#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket= new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::slotReadyRead);
    connect(socket,&QTcpSocket::disconnected,socket,&QTcpSocket::deleteLater);
    nextBlockSize=0;
}


MainWindow::~MainWindow()
{
    delete ui;
}
QString addresses="127.0.0.1";

void MainWindow::on_pushButton_clicked()
{
    //addresses=ui->lineEdit_2->text();
    socket->connectToHost(QHostAddress(addresses),2323);// 127.0.0.1 2a00:1fa1:840d:f557:2dab:2e5a:311e:9d37

}
void MainWindow:: SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    str=QDateTime::currentDateTime().toString("zzzz");
    out<<quint16(0)<<QDateTime::currentDateTime().toString("ss.zz");//<<str;//поле сообщения
    out.device()->seek(0);
    out<<quint16(Data.size() - sizeof(quint16));

    socket->write(Data);
    _sleep(100);
    ui->lineEdit->clear();}


void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok)
    {
       /* QString str;
        in>>str;
        ui->textBrowser->append(str);*/
        for(;;)
        {
            if(nextBlockSize==0)
            {
                if(socket->bytesAvailable()<2)
                {
                    break;
                }
                in>>nextBlockSize;
            }
            if(socket->bytesAvailable()<nextBlockSize)
            {
                break;
            }
            QString str;
            QTime time;
            in>>time>>str;
            nextBlockSize=0;
            ui->textBrowser->append(time.toString("ss.zz")+" // "+ str);

        }
    }
    else
    {
      ui->textBrowser->append("read error");
    }
}


void MainWindow::on_pushButton_2_clicked()
{

    SendToServer(ui->lineEdit->text());

}


void MainWindow::on_lineEdit_returnPressed()
{

    SendToServer(ui->lineEdit->text());
}

