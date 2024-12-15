#include "sendfile.h"
#include <QThread>
#include <QDebug>
#include <QSslConfiguration>
#include <QFile>

SendFile::SendFile(QObject *parent) : QTcpServer{ parent }
{
    qDebug() << "constructor current  thread id:" << QThread::currentThreadId();
    QFile keyFile("://certificates/red_local.key");
    keyFile.open(QIODevice::ReadOnly);
    key = QSslKey(keyFile.readAll(), QSsl::Rsa);
    keyFile.close();

    QFile certFile("://certificates/red_local.pem");
    certFile.open(QIODevice::ReadOnly);
    cert = QSslCertificate(certFile.readAll());
    certFile.close();
}

void SendFile::link()
{
    QTcpSocket *clientSocket = nextPendingConnection();
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

void SendFile::startListenSlot(uint16_t port)
{
    if (!isListening()) {
        if (!listen(QHostAddress::Any, port)) {
            qDebug() << "Error: Unable to start server!" << errorString();
        } else {
            qDebug() << "Server started on port" << port;
        }
    }

    connect(this, &SendFile::newConnection, this, &SendFile::link);
}

void SendFile::incomingConnection(qintptr socketDescriptor)
{
    QSslSocket *sslSocket = new QSslSocket(this);

    // connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this,
    //         SLOT(sslErrors(QList<QSslError>)));
    sslSocket->setSocketDescriptor(socketDescriptor);
    sslSocket->setPrivateKey(key);
    sslSocket->setLocalCertificate(cert);

    sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    sslSocket->startServerEncryption();

    addPendingConnection(sslSocket);
}
