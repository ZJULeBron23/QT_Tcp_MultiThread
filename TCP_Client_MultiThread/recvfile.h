#ifndef RECVFILE_H
#define RECVFILE_H
#include <QTcpSocket>
#include <QObject>

class RecvFile : public QObject
{
    Q_OBJECT
public:
    explicit RecvFile(QObject *parent = nullptr);

    void connectServer(QString ip, unsigned short port);
    void sendDataSlot(QString sData);
    void disconnectSlot();

signals:
    void connectOK();
    void msg(QByteArray msg);
    void gameOver();

private:
    QTcpSocket *m_tcpSocket = nullptr;
};

#endif // RECVFILE_H
