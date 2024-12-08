#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpserver.h"

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
    void on_startServer_clicked();

    void on_sendData_clicked();

signals:
    void start(QString msg);
    void init();

private:
    Ui::MainWindow *ui;
    TcpServer *m_Server;
};
#endif // MAINWINDOW_H
