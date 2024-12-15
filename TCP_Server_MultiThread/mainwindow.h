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
    void on_startServer_clicked();

    void on_sendData_clicked();
    void PreTask();

signals:
    void start(QString msg);
    void init();

    void startListen(uint16_t port);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
