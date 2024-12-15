#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QSslKey>
#include <QSslCertificate>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void newClient();

private:
    QSslKey key;
    QSslCertificate cert;
};

#endif // TCPSERVER_H
