#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QList>
#include <QHash>

class QTcpSocket;
class QBuffer;

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
    void add_connect();
    void remove_connect();
    void setIcon(QString &a);
    int checkMessage(QString &a);

    void on_pushButton_Listen_clicked();

    void on_toolButton_file_clicked();

    void on_pushButton_SendFile_clicked();

private:
    QTcpSocket *socket;
    QTcpServer *server;
    QList<QTcpSocket*> connections;
    QHash<QTcpSocket*, QBuffer*> buffers;



    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
