#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QList>
#include <QHash>

class QTcpSocket;
class QBuffer;

#include <QTcpSocket>
#include <QBuffer>
class QBuffer;
class QTcpSocket;

namespace Ui {
    class ChatServer;
}

class ChatServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatServer(QMainWindow *parent = 0);
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

    void on_toolButton_file_clicked();


private:
    QTcpServer *server;
    QTcpSocket *socket;
    QList<QTcpSocket*> connections;
    QHash<QTcpSocket*, QBuffer*> buffers;
    Ui::ChatServer *ui;
private slots:
    void on_btnlistent_clicked();
    void addConnection();
    void removeConnection();
    void receiveMessage();
    void sendMessage();
};

#endif // CHATSERVER_H
