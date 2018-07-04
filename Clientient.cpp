
#include "Clientient.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

static const int DEFAULT_PORT = 6789;
int port;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    time = new QTimer(this);

    //set Ip and Port
    ui->lineEdit_Name->setText("Client");
    ui->lineEdit_Port->setText("6789");
    ui->lineEdit_Port->setEnabled(false);

    //set text Notify and pushButton_connect
    qDebug() << "Not have connect to server!";
    ui->pushButton_Connect->setText("Connect");
    ui->textEdit_Content->setEnabled(false);

    if(!socket->isOpen()){
        qDebug() << "Socket not Open!";
    }
    connect(ui->lineEdit_Send,SIGNAL(returnPressed()),this,SLOT(send_message()));
    connect(ui->pushButton_Send,SIGNAL(clicked(bool)),this,SLOT(send_message()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(receive_message()));

    connect(time,SIGNAL(timeout()),this,SLOT(time_connect()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(socket_disconnect()));
    connect(socket,SIGNAL(connected()),this,SLOT(time_close()));
    port = DEFAULT_PORT;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --> Conpelate
void MainWindow::send_message(){
    QMessageBox box;
    if(!socket->isOpen()){
        qDebug()<< "Socket is not open, Cann't send message now";
        box.information(this,tr("The Title"),tr("Socket is close! Cann't send message now!"));
        ui->lineEdit_Send->clear();
    }
    else{
        QString msg = ui->lineEdit_Name->text() +": "+ ui->lineEdit_Send->text() + "\n";
        bool a = socket->write(msg.toLatin1());
        if(a){
            qDebug() << "Send message success!";
            ui->lineEdit_Send->clear();
        }
        else{
            qDebug() << socket->errorString();
            box.information(this,tr("The Title"),tr("Message error!"));
            ui->lineEdit_Send->clear();
        }
    }
}

// --> Compelate
void MainWindow::receive_message(){
    qDebug() << "Have a message!";
    ui->textEdit_Content->append(socket->readAll());
}

//connect/disconnect to server
void MainWindow::on_pushButton_Connect_clicked(){
    if(ui->pushButton_Connect->text() == "Connect"){
        socket->connectToHost("127.0.0.1",port);
        if(socket->isOpen()){
            qDebug() << "Connecting...!";
            ui->pushButton_Connect->setText("Disconncet");
            time->start(3000);
        }
    }
    else{
        socket->disconnectFromHost();
        qDebug() << "Disconnected to server!";
        ui->pushButton_Connect->setText("Connect");
    }
}

// --> Compelate
void MainWindow::socket_disconnect(){
    QMessageBox box;
    qDebug() << "Disconnect to server!";
    ui->pushButton_Connect->setText("Connect");
    box.information(this,tr("The Title"),tr("Disconnect to server!"));
}

// --> compelate
void MainWindow::on_toolButton_file_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"","/","");
    if(filename.isEmpty()){
        return;
    }
    ui->textEdit_Content->append(filename);
}

//time out connect to server
void MainWindow::time_connect(){
    QMessageBox box;
    box.information(this,tr("The Title"),tr("Server is busy!"));
    qDebug() << "Server Busy!";
    ui->pushButton_Connect->setText("Connect");
    time->stop();
    socket->close();
}

void MainWindow::time_close(){
    time->stop();
}
