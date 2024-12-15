#include "recvfile.h"
#include <QDebug>
#include <QThread>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslConfiguration>
RecvFile::RecvFile()
{
    qDebug() << "constructor func " << QThread::currentThreadId();
}

void RecvFile::connectServer(QString ip, unsigned short port)
{
    qDebug() << "connectServer current sub thread id:"
             << QThread::currentThread();

    m_sslSocket = new QSslSocket();

    QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
    configuration.addCaCertificates("://certificates/red_ca.pem");

    // should not using the api !!!
    // QSslConfiguration::setDefaultConfiguration(configuration);

    m_sslSocket->setPrivateKey("://certificates/blue_local.key");
    m_sslSocket->setLocalCertificate("://certificates/blue_local.pem");

    m_sslSocket->setSslConfiguration(configuration);

    m_sslSocket->setPeerVerifyMode(QSslSocket::VerifyPeer);

    // 连接服务器
    m_sslSocket->connectToHostEncrypted(ip, port);
    // 5 seconds are not enough.
    if (m_sslSocket->waitForEncrypted(10000)) {
        m_sslSocket->write("Authentication Suceeded");
    } else {
        qDebug() << ("Unable to connect to server");
    }

    // // 检测是否和服务器是否连接成功了
    connect(m_sslSocket, &QSslSocket::connected, this, &RecvFile::connectOK);

    // 检测服务器是否回复了数据
    connect(m_sslSocket, &QSslSocket::readyRead, [=]() {
        // 接收服务器发送的数据
        QByteArray recvMsg = m_sslSocket->readAll();
        emit msg(recvMsg);
    });

    // // 检测服务器是否和客户端断开了连接
    connect(m_sslSocket, &QSslSocket::disconnected, this, [=]() {
        qDebug() << ("服务器已经断开了连接, ...");
        m_sslSocket->close();
        m_sslSocket->deleteLater();
        emit gameOver();
    });
}

void RecvFile::sendDataSlot(QString sData)
{
    sData = "Client Say: " + sData;
    // print data
    qDebug() << "client send data:" << sData;

    m_sslSocket->write(sData.toUtf8());
    emit msg(sData.toUtf8());
}

void RecvFile::disconnectSlot()
{
    m_sslSocket->close();
}
