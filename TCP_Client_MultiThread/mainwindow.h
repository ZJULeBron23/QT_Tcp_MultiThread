#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectServer_clicked();

    void on_disconnect_clicked();

    void on_sendData_clicked();
    void preTask();

private:
    Ui::MainWindow *ui;

signals:
    void startConnect(QString ip, unsigned short port);
    void sendData(QString sData);
    void disconnect();
};
#endif // MAINWINDOW_H
