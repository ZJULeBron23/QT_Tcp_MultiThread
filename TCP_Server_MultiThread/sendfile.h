#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include <QTcpSocket>

class SendFile : public QObject
{
    Q_OBJECT
public:
    explicit SendFile(qintptr socket, QObject *parent = nullptr);

    void Working(QString msg);
    void initSlot();

signals:
    void done();
    void text(QByteArray msg);

private:
    qintptr m_socket;
    QTcpSocket *m_TcpSocket;
};

#endif // SENDFILE_H
