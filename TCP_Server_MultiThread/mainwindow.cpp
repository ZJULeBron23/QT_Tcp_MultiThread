#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QThread>
#include "sendfile.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_Server(new TcpServer(this))

{
    ui->setupUi(this);
    setWindowTitle("TCP - Server");
    qDebug() << "current main thread id:" << QThread::currentThread();

    connect(m_Server, &TcpServer::newClient, this, [=]() {
        qDebug() << "entered newClient";
        // cope subthread related logic
        QThread *subThread = new QThread();
        SendFile *sendFile = new SendFile();
        sendFile->moveToThread(subThread);

        QTcpSocket *clientSocket = m_Server->nextPendingConnection();

        connect(m_Server, &TcpServer::newConnection, this,
                [=]() { emit socketPass(clientSocket); });

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

        connect(this, &MainWindow::socketPass, sendFile, &SendFile::link);

        // only subthread could cope ui object
        subThread->start();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startServer_clicked()
{
    ui->startServer->setEnabled(false);
    uint16_t u16Port = ui->port->text().toUInt();

    m_Server->listen(QHostAddress("127.0.0.1"), u16Port);
}

void MainWindow::on_sendData_clicked()
{
    QString sendMsg = ui->sendArea->toPlainText();
    emit start(sendMsg);
}
