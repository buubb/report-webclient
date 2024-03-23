#include "widget.h"
#include "ui_widget.h"
#include "qabstractsocket.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QObject::connect(&socket_, &QAbstractSocket::connected,this,&Widget::doConnected);
    QObject::connect(&socket_, &QAbstractSocket::disconnected,this,&Widget::disConnected);
    QObject::connect(&socket_, &QAbstractSocket::readyRead,this,&Widget::doReadyRead);

    QObject::connect(&socket_2, &QAbstractSocket::connected,this,&Widget::doConnected);
    QObject::connect(&socket_2, &QAbstractSocket::disconnected,this,&Widget::disConnected);
    QObject::connect(&socket_2, &QAbstractSocket::readyRead,this,&Widget::doReadyRead);

    changeEnable();
}

Widget::~Widget()
{
    delete ui;
}
void Widget::doConnected(){
    changeEnable();
}
void Widget::disConnected(){
    changeEnable();
    ui->pteMessage->insertPlainText("Disconnected.\n\n");
}
void Widget::doReadyRead(){
    if(socket_.state() == QAbstractSocket::ConnectedState)
        ui->pteMessage->insertPlainText(socket_.readAll());
    if(socket_2.state() == QAbstractSocket::ConnectedState)
        ui->pteMessage->insertPlainText(socket_2.readAll());
}

void Widget::on_pbConnect_clicked()
{
    // tcp
    if(ui->cbTCP->isChecked() && !(ui->cbSSL->isChecked())){
        ui->pteMessage->insertPlainText("TCP connected. \n\n");
        socket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
    }else if(!(ui->cbTCP->isChecked()) && ui->cbSSL->isChecked()){ // ssl
        ui->pteMessage->insertPlainText("SSL connected. \n\n");
        socket_2.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort());
    }
    else{
        ui->pteMessage->insertPlainText("Select one connection. \n\n");
    }
    changeEnable();
}

void Widget::changeEnable(){
    ui->pbDisconnect->setEnabled(socket_.state() == QAbstractSocket::ConnectedState || socket_2.state() == QAbstractSocket::ConnectedState);
    ui->pbConnect->setDisabled((ui->cbTCP->isChecked() && socket_.state() == QAbstractSocket::ConnectedState) || (ui->cbSSL->isChecked() && socket_2.state() == QAbstractSocket::ConnectedState));
    ui->pbClear->setEnabled(!(ui->pteMessage->toPlainText().isEmpty()));
}


void Widget::on_pbDisconnect_clicked()
{
    socket_.close();
    socket_2.close();
    changeEnable();
}


void Widget::on_pbSend_clicked()
{
    if(socket_.state() == QAbstractSocket::ConnectedState)
        socket_.write(ui->pteSend->toPlainText().toUtf8());
    if(socket_2.state() == QAbstractSocket::ConnectedState)
        socket_2.write(ui->pteSend->toPlainText().toUtf8());
}


void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
    changeEnable();
}

