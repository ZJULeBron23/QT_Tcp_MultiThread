#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QThread>
#include "sendfile.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    setWindowTitle("TCP - Server");
    qDebug() << "current main thread id:" << QThread::currentThread();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startServer_clicked()
{
    // each time, start server clicked, conduct thread and task thread launch.
    PreTask();
    ui->startServer->setEnabled(false);
    uint16_t u16Port = ui->port->text().toUInt();
    emit startListen(u16Port);
}

void MainWindow::on_sendData_clicked()
{
    QString sendMsg = ui->sendArea->toPlainText();
    emit start(sendMsg);
}

void MainWindow::PreTask()
{
    // cope subthread related logic
    QThread *subThread = new QThread();
    SendFile *sendFile = new SendFile();
    sendFile->moveToThread(subThread);

    connect(this, &MainWindow::start, sendFile, &SendFile::Working);
    connect(sendFile, &SendFile::done, this, [=]() {
        qDebug() << "sever resource delete";
        // delete resources
        subThread->quit();
        subThread->wait();
        subThread->deleteLater();
        sendFile->deleteLater();
        ui->startServer->setEnabled(true);
    });
    connect(sendFile, &SendFile::text, this,
            [=](QByteArray msg) { ui->recvArea->append(msg); });

    connect(this, &MainWindow::startListen, sendFile,
            &SendFile::startListenSlot);

    // only subthread could cope ui object
    subThread->start();
}
