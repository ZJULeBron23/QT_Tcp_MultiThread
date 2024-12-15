#ifndef RECVFILE_H
#define RECVFILE_H
#include <QSslSocket>
#include <QObject>

class RecvFile : public QObject
{
    Q_OBJECT
public:
    explicit RecvFile();

    void connectServer(QString ip, unsigned short port);
    void sendDataSlot(QString sData);
    void disconnectSlot();

signals:
    void connectOK();
    void msg(QByteArray msg);
    void gameOver();

private:
    QSslSocket *m_sslSocket;
};

#endif // RECVFILE_H
