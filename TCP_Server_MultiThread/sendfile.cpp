#include "sendfile.h"
#include <QThread>
#include <QDebug>
#include <QSslConfiguration>

SendFile::SendFile(QObject *parent) : QObject{ parent }
{
    qDebug() << "constructor current  thread id:" << QThread::currentThreadId();
}

void SendFile::link(QTcpSocket *clientSocket)
{
    connect(clientSocket, &QTcpSocket::readyRead, this, [=]() {
        // 接收数据
        QString recvMsg = clientSocket->readAll();
        emit text(recvMsg.toUtf8());
    });
    connect(clientSocket, &QTcpSocket::disconnected, this, [=]() {
        clientSocket->close();
        clientSocket->deleteLater();
        emit done();
        qDebug() << ("客户端已经断开了连接...");
    });
    connect(this, &SendFile::Working, clientSocket, [=](QString msg) {
        clientSocket->write(msg.toUtf8());
        qDebug() << "Working current sub thread id :"
                 << QThread::currentThread();
        msg = "Server Say:" + msg;

        emit text(msg.toUtf8());
    });
}
