#include "sendfile.h"
#include <QThread>
#include <QDebug>
SendFile::SendFile(qintptr socket, QObject *parent) : QObject{ parent }
{
    m_socket = socket;
}

void SendFile::Working(QString msg)
{
    qDebug() << "Working current sub thread id :" << QThread::currentThread();
    msg = "Server Say:" + msg;

    m_TcpSocket->write(msg.toUtf8());
    emit text(msg.toUtf8());
}

void SendFile::initSlot()
{
    // initialization shall not be placed in constructor
    qDebug() << "initSlot current sub thread id:" << QThread::currentThread();
    m_TcpSocket = new QTcpSocket;
    m_TcpSocket->setSocketDescriptor(m_socket);

    // 客户端断开了连接
    connect(m_TcpSocket, &QTcpSocket::disconnected, this, [=]() {
        m_TcpSocket->close();
        m_TcpSocket->deleteLater();
        emit done();
        qDebug() << ("客户端已经断开了连接...");
    });

    // inspect the data from client
    connect(m_TcpSocket, &QTcpSocket::readyRead, this, [=]() {
        // 接收数据
        QString recvMsg = m_TcpSocket->readAll();
        // recvMsg = "Recving client msg:" + recvMsg;
        emit text(recvMsg.toUtf8());
    });
}
