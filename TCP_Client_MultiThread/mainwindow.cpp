#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QThread>
#include <QDebug>
#include "recvfile.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("TCP - 客户端");
    qDebug() << "current main thread id:" << QThread::currentThread();
    QThread *subThread = new QThread();
    RecvFile *worker = new RecvFile();
    worker->moveToThread(subThread);

    connect(this, &MainWindow::startConnect, worker, &RecvFile::connectServer);
    connect(this, &MainWindow::sendData, worker, &RecvFile::sendDataSlot);
    connect(this, &MainWindow::discon, worker, &RecvFile::disconnectSlot);
    connect(worker, &RecvFile::connectOK, this,
            [=]() { ui->recvArea->append("恭喜, 连接服务器成功!!!"); });
    connect(worker, &RecvFile::msg, this,
            [=](QByteArray recvMsg) { ui->recvArea->append(recvMsg); });

    connect(worker, &RecvFile::gameOver, this, [=]() {
        // qDebug() << "resouce free";
        subThread->quit();
        subThread->wait();
        subThread->deleteLater();
        worker->deleteLater();

        ui->connectServer->setEnabled(true);
        ui->disconnect->setEnabled(false);
    });

    subThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectServer_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toInt();

    emit startConnect(ip, port);
    ui->connectServer->setEnabled(false);
    ui->disconnect->setEnabled(true);
}

void MainWindow::on_disconnect_clicked()
{
    emit discon();

    ui->connectServer->setEnabled(true);
    ui->disconnect->setEnabled(false);
}

void MainWindow::on_sendData_clicked()
{
    QString sendMsg = ui->sendArea->toPlainText();
    emit sendData(sendMsg);
}
