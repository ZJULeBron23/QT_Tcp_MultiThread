#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

    // override incoming connection
protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void newClient(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
