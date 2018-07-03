#include "chatserver.h"
#include "ui_chatserver.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>

static const int DEFAULT_PORT = 6789;
QHostAddress server_add;
int port;

ChatServer::ChatServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatServer)
{
    ui->setupUi(this);
    server = new QTcpServer(this);

    ui->lineEdit_Name->setText("Client");
    ui->lineEdit_Port->setEnabled(false);
    ui->lineEdit_Port->setText("6789");
    ui->pushButton_Listen->setText("Listen");
    ui->label_Notify->setText("Server is close!");
    ui->lineEdit_Name->setText("Server!");
    ui->lineEdit_Name->setEnabled(false);

    connect(server,SIGNAL(newConnection()),this,SLOT(add_connect()));

    server_add = QHostAddress::AnyIPv4;
    port = DEFAULT_PORT;
}

ChatServer::~ChatServer()
{
    delete ui;
}

//
void ChatServer::receive_message(){
    qDebug() << "New message";
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QBuffer *buffer = buffers.value(socket);
    qint64 bytes = buffer->write(socket->readAll());
    buffer->seek(buffer->pos() - bytes);
    while (buffer->canReadLine())
    {
        QByteArray line = buffer->readLine();
        foreach (QTcpSocket* connect, connections){
            connect->write(line);
        }
    }
}

// --> Compelate
void ChatServer::add_connect(){
    qDebug() << "New connnect!";
    QTcpSocket* connection = server->nextPendingConnection();
    connections.append(connection);
    QBuffer* buffer = new QBuffer(this);
    buffer->open(QIODevice::ReadWrite);
    buffers.insert(connection, buffer);
    //Event socket: signal and disconnect
    connect(connection,SIGNAL(readyRead()),this,SLOT(receive_message()));
    connect(connection,SIGNAL(disconnected()),this,SLOT(remove_connect()));
}

// --> Compelate
void ChatServer::remove_connect(){
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QBuffer* buffer = buffers.take(socket);
    buffer->close();
    buffer->deleteLater();
    connections.removeAll(socket);
    qDebug() << "Remove a connect!";
    socket->deleteLater();
}

// --> Compelate
void ChatServer::on_pushButton_Listen_clicked()
{
    QMessageBox box;
    if(ui->pushButton_Listen->text()=="Listen"){
        server->listen(server_add, port);
        if(server->isListening()){
            qDebug() << "Server is Listening!";
            ui->pushButton_Listen->setText("Sleep");
            ui->label_Notify->setText("Server is Listening...");
        }
        else{
            qDebug() << server->errorString();
            box.setText("Server is Error!");
            box.exec();
        }
    }
    else{
        server->close();
        if(server->isListening()){
            qDebug() << server->errorString();
            box.setText("Cann't close Server!");
            box.exec();
        }
        else{
            ui->label_Notify->setText("Server is close!");
            ui->pushButton_Listen->setText("Listen");
            qDebug() << "Server is close!";
        }
    }
}
