#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QList>
#include <QHash>

class QTcpSocket;
class QBuffer;


namespace Ui {
class ChatServer;
}

class ChatServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatServer(QWidget *parent = 0);
    ~ChatServer();

private slots:
    void receive_message();
    void add_connect();
    void remove_connect();

    void on_pushButton_Listen_clicked();

private:
    QTcpServer *server;
    QList<QTcpSocket*> connections;
    QHash<QTcpSocket*, QBuffer*> buffers;

    Ui::ChatServer *ui;
};

#endif // CHATSERVER_H
