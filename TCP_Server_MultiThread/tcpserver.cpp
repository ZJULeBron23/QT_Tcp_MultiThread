#include "tcpserver.h"
#include <QSslSocket>
#include <QFile>
TcpServer::TcpServer(QObject *parent) : QTcpServer{ parent }
{
    QFile keyFile("://certificates/red_local.key");
    keyFile.open(QIODevice::ReadOnly);
    key = QSslKey(keyFile.readAll(), QSsl::Rsa);
    keyFile.close();

    QFile certFile("://certificates/red_local.pem");
    certFile.open(QIODevice::ReadOnly);
    cert = QSslCertificate(certFile.readAll());
    certFile.close();
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    QSslSocket *sslSocket = new QSslSocket(this);
    qDebug() << "entered incoming";

    // connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this,
    //         SLOT(sslErrors(QList<QSslError>)));
    sslSocket->setSocketDescriptor(socketDescriptor);
    sslSocket->setPrivateKey(key);
    sslSocket->setLocalCertificate(cert);

    sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    sslSocket->startServerEncryption();

    addPendingConnection(sslSocket);
    emit newClient();
}
