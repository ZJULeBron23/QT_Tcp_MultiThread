#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include <QTcpSocket>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslSocket>
#include <QTcpServer>

class SendFile : public QObject
{
    Q_OBJECT
public:
    explicit SendFile(QObject *parent = nullptr);

    void link(QTcpSocket *clientSocket);

signals:
    void done();
    void text(QByteArray msg);
    void Working(QString msg);

private:
    QSslKey key;
    QSslCertificate cert;
};

#endif // SENDFILE_H
