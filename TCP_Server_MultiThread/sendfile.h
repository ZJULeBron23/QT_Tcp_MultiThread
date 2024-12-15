#ifndef SENDFILE_H
#define SENDFILE_H

#include <QSslKey>
#include <QSslCertificate>
#include <QSslSocket>
#include <QTcpServer>

class SendFile : public QTcpServer
{
    Q_OBJECT
public:
    explicit SendFile(QObject *parent = nullptr);

    void link();
    void startListenSlot(uint16_t port);

protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void done();
    void text(QByteArray msg);
    void Working(QString msg);

    void newClient();

private:
    QSslKey key;
    QSslCertificate cert;
};

#endif // SENDFILE_H
