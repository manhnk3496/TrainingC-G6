#include "mainwindow.h"
#include "ui_mainwindow.h"

//Library add
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>
#include <QTextStream>
#include <QFile>


static const int DEFAULT_PORT = 6789;
int port;
QHostAddress server_add;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    socket = new QTcpSocket(this);
    //set Ip and Port
    ui->lineEdit_Ip->setText("192.168.12.51");
    ui->lineEdit_Ip->setEnabled(false);
    ui->lineEdit_Port->setText("6789");
    ui->lineEdit_Part->setEnabled(false);
    ui->lineEdit_Port->setEnabled(false);

    //set text contentChat, pushButton_linten,
    ui->textEdit_content->setEnabled(true);
    ui->pushButton_Listen->setText("Listen");
    qDebug() << "No connect!";

    connect(ui->pushButton_Send,SIGNAL(clicked(bool)),this,SLOT(send_message()));
    connect(ui->lineEdit_Send,SIGNAL(returnPressed()),this,SLOT(send_message()));
    connect(server,SIGNAL(newConnection()),this,SLOT(add_connect()));
    //set address and port
    server_add = QHostAddress::AnyIPv4;
    port = DEFAULT_PORT;
}

MainWindow::~MainWindow()
{
    server->close();
    socket->close();
    delete ui;
}

//send message -> compelate
void MainWindow::send_message(){
    QMessageBox box;
    if(!socket->isOpen()){
        ui->lineEdit_Send->clear();
        qDebug()<< "Socket is not open, Cann't send message now";
        box.information(this,tr("The Title"),tr("Socket is close! Cann't send message now!"));
    }
    else{
        ui->textEdit_content->acceptRichText();
        QString show = ui->lineEdit_Send->text();
        setIcon(show);
        ui->textEdit_content->append("Server: " + show);
        QString msg ="ch" + ui->lineEdit_Send->text()+"\n";
        bool a = socket->write(msg.toUtf8(),msg.length());
        setIcon(msg);
        if(a){
            qDebug() << "Send message success!";
            ui->lineEdit_Send->clear();
        }
        else{
            qDebug() << socket->errorString();
            box.information(this,tr("The Title"),tr("Message error!"));
        }
    }
}

//receive message -> compelate
void MainWindow::receive_message(){
    qDebug() << "Have a messge!";
    QByteArray data = socket->readAll();
    QString text = QString::fromUtf8(data);
    ui->textEdit_content->acceptRichText();
    setIcon(text);
    if(checkMessage(text)==1){
        qDebug() << "Have o new file";
        ui->textEdit_content->append("Have a new file");
        QString filename = "newFile.txt";
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << text << endl;
        }
    }
    else{
        ui->textEdit_content->append(text);
    }
}

//add connect --> compelate
void MainWindow::add_connect(){
    socket = server->nextPendingConnection();
    qDebug() << "Have new connect";
    connect(socket,SIGNAL(readyRead()),this,SLOT(receive_message()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(remove_connect()));
    socket->write("aaHello Client!");
    server->close();
}

//remove connect --> compelate
void MainWindow::remove_connect(){
    if(socket->isOpen()){
        socket->close();
    }
    qDebug() << "Socket close!";
    ui->textEdit_content->append("Socket is close!");
}

//--> Compelate
void MainWindow::on_pushButton_Listen_clicked()
{
    QMessageBox box;
    server->close();
    if(socket->isOpen()){
        socket->close();
    }
    if(ui->pushButton_Listen->text() == "Listen"){
        server->listen(server_add, port);
        if(server->isListening()){
            qDebug() << "Server is listening...";
            ui->pushButton_Listen->setText("Sleep");
        }
        else{
            box.information(this,tr("The Title"),tr("Open Server error!"));
            qDebug() << "Server listen is error!";
        }
    }
    else{
        server->close();
        if(server->isListening()){
            qDebug() << "Close server is false.";
        }
        else{
            qDebug() << "Server is Close.";
            ui->textEdit_content->append("Socket is close!");
            ui->pushButton_Listen->setText("Listen");
        }
    }
}

//box file --> Compelate
void MainWindow::on_toolButton_file_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"","/","");
    if(filename.isEmpty()){
        return;
    }
    ui->lineEdit_Part->setText(filename);
}

void MainWindow::on_pushButton_SendFile_clicked()
{
    QFile file(ui->lineEdit_Part->text());
    if(!file.open(QIODevice::ReadOnly)){
        return ;
    }
    QTextStream in(&file);
    QString m = "fi";
    while(!in.atEnd()){
        m+=in.readLine();
    }
    socket->write(m.toUtf8());
}

void MainWindow::setIcon(QString &text){
    text.replace("(cuoi)","<img src='/home/nguyen.khac.manh/Desktop/Icon1.png'>");
    text.replace("(khoc)","<img src='/home/nguyen.khac.manh/Desktop/Icon2.png'>");
    text.replace("(vui)","<img src='/home/nguyen.khac.manh/Desktop/Icon3.png'>");
    text.replace("(buon)","<img src='/home/nguyen.khac.manh/Desktop/Icon4.png'>");
    text.replace("(buon2)","<img src='/home/nguyen.khac.manh/Desktop/Icon5.png'>");
    text.replace("(buon3)","<img src='/home/nguyen.khac.manh/Desktop/Icon6.png'>");
}

int MainWindow::checkMessage(QString &a){
    QString s, ma = a.mid(0,2);
    int i = a.length();
    for(int j=2; j < i; j++){
        s+=a[j];
    }
    a=s;
    if(ma=="fi") return 1;
    else return 0;
}
