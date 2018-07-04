#include "chatserver.h"
#include "ui_chatserver.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QBuffer>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>

static const int DEFAULT_PORT = 6789;
QHostAddress server_add;
int port;

ChatServer::ChatServer(QWidget *parent) :

static const int DEFAULT_PORT = 6789;
static const int DEFAULT_PORT =5555;
int port;
QHostAddress server_addr;
ChatServer::ChatServer(QMainWindow *parent) :
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
    socket = new QTcpSocket(this);
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()),this, SLOT(addConnection()));
    connect(ui ->editmes, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    connect(ui->btnsend,SIGNAL(clicked(bool)), this, SLOT(sendMessage()));
    server_addr = QHostAddress::LocalHost;
    port = DEFAULT_PORT;

    ui ->editport ->setText("5555");
    ui ->editport ->setEnabled(false);
    ui ->editIP ->setText("192.168.11.17 ");
    ui ->editIP ->setEnabled(false);
    server_addr = QHostAddress::AnyIPv4;
}

ChatServer::~ChatServer()
{
    server->close();
    socket->close();
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
    ui->lineEdit_Send->clear();
void ChatServer::on_btnlistent_clicked()
{
    port = ui->editport->text().toInt();
    bool b=server->listen(server_addr,port);
    if(b)
    {
        server->close();
        qDebug("server started");
        ui->btnlistent->setEnabled(true);
        //ui->btnStop->setEnabled(true);
    }
   else
    {
        qDebug("server can't start! check IP or Port");
    }
}
//void ChatServer::on_btnStop_clicked()
//{
//   if(server->isListening())
//   {
//       server->close();
//        ui->btnStart->setEnabled(true);
//        ui->btnStop->setEnabled(false);
//    }
//}
void ChatServer::addConnection()
{
    QTcpSocket* connection =server->nextPendingConnection();
    connections.append(connection);
    QBuffer *buffer= new QBuffer(this);
    buffer->open(QIODevice::ReadWrite);
    buffers.insert(connection, buffer);
    connect(connection, SIGNAL(disconnected()),this, SLOT(removeConnection()));
    connect(connection, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

// --> Compelate
//receive message -> compelate
void ChatServer::receive_message(){
    qDebug() << "Have a messge!";
    ui->textEdit_content->append("Client: "+socket->readAll());
}
void ChatServer::removeConnection()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QBuffer *buffer = buffers.take(socket);
    buffer->close();
    ui->btnlistent->setEnabled(true);
    buffer->deleteLater();

//add connect --> compelate
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
    qDebug() << "Socket close!";
    QMessageBox box;
    box.setText("Socket close!");
    box.exec();
    server->close();
    server->listen(server_add,port);
    connections.removeAll(socket);
    socket->deleteLater();
}
void ChatServer::receiveMessage()

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
{

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    QBuffer *buffer = buffers.value(socket);

    qint64 bytes = buffer->write(socket->readAll());

    buffer->seek(buffer->pos() - bytes);

    while (buffer->canReadLine())

    {
            QByteArray line = buffer->readLine();
            ui->editchat->append(line.simplified());

            foreach (QTcpSocket* connection, connections)

            {

                    connection->write(line);

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
void ChatServer::sendMessage(){
   bool a = socket->write(ui->editmes->text().toLatin1());
   if(a){
        qDebug() << "Send message is success!!!";
        ui->editchat->append("Client: " + ui->editmes->text());
    }
    else{
        qDebug() << "Send message is error!!!";
   }
    ui->editmes->clear();
}
