#include "recvfile.h"
#include <QDebug>
#include <QThread>

RecvFile::RecvFile(QObject *parent) : QObject{ parent }
{
}

void RecvFile::connectServer(QString ip, unsigned short port)
{
    qDebug() << "current sub thread id:" << QThread::currentThread();
    m_tcpSocket = (new QTcpSocket);
    // 连接服务器
    m_tcpSocket->connectToHost(QHostAddress(ip), port);

    // // 检测是否和服务器是否连接成功了
    connect(m_tcpSocket, &QTcpSocket::connected, this, &RecvFile::connectOK);

    // 检测服务器是否回复了数据
    connect(m_tcpSocket, &QTcpSocket::readyRead, [=]() {
        // 接收服务器发送的数据
        QByteArray recvMsg = m_tcpSocket->readAll();
        emit msg(recvMsg);
    });

    // // 检测服务器是否和客户端断开了连接
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, [=]() {
        qDebug() << ("服务器已经断开了连接, ...");
        m_tcpSocket->close();
        m_tcpSocket->deleteLater();
        emit gameOver();
    });
}

void RecvFile::sendDataSlot(QString sData)
{
    if (m_tcpSocket == nullptr) {
        qCritical() << "m_tcpSocket is nullptr";
        return;
    }

    sData = "Client Say: " + sData;

    m_tcpSocket->write(sData.toUtf8());
    emit msg(sData.toUtf8());
}

void RecvFile::disconnectSlot()
{
    if (m_tcpSocket == nullptr) {
        qCritical() << "m_tcpSocket is nullptr";
        return;
    }
    m_tcpSocket->close();
}
