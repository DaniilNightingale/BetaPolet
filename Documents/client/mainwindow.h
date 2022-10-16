#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include<QTime>
#include <QtGui>
#include <qobject.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:




private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    void SendToServer(QString str);
    quint16 nextBlockSize;
    QTimer *timer;

public slots:
    void sts(){
        timer = new QTimer(this);
                QObject::connect(timer, SIGNAL(timeout()), this, SLOT(on_pushButton_2_clicked()));

                timer->start(100); //ms
    }

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_returnPressed();

    void slotReadyRead();
};
#endif // MAINWINDOW_H
