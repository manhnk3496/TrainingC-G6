#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void send_message();
    void receive_message();
    void socket_disconnect();
    void time_connect();
    void time_close();

    void on_pushButton_Connect_clicked();

    void on_toolButton_file_clicked();

private:
    QTcpSocket *socket;
    QTimer *time;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
