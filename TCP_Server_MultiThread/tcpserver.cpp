#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer{ parent }
{
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "recv client connection";
    emit newClient(socketDescriptor);
}
