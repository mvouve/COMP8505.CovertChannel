#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->sendBtn, SIGNAL(released()), this, SLOT(send()));
    connect(ui->listenBtn, SIGNAL(released()), this, SLOT(listen()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::listen() {
    ui->listenPort->setDisabled(true);
    ui->listenBtn->setText("Stop");

    ServerDriver * sDriver = new ServerDriver(ui->listenPort->text().toInt());
    QThread * thread = new QThread;
    sDriver->moveToThread(thread);

    connect(sDriver, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread,  SIGNAL(started()),  sDriver, SLOT(process()));
    connect(sDriver, SIGNAL(finished()), thread, SLOT(quit()));
    connect(sDriver, SIGNAL(finished()), sDriver, SLOT(deleteLater()));
    connect(thread,  SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(sDriver, SIGNAL(readByte(char, int)), this, SLOT(writeCharacter(char, int)));

    // Button Work
    disconnect(ui->listenBtn, SIGNAL(released()), this, SLOT(listen()));
    connect(ui->listenBtn, SIGNAL(released()), sDriver, SLOT(deleteLater()));
    connect(ui->listenBtn, SIGNAL(released()), thread, SLOT(deleteLater()));
    connect(ui->listenBtn, SIGNAL(released()), thread, SLOT(quit()));
    connect(ui->listenBtn, SIGNAL(released()), this, SLOT(endListen()));

    thread->start();

}

void MainWindow::endListen() {
    ui->listenBtn->setText("Listen");
    ui->listenPort->setDisabled(false);
    connect(ui->listenBtn, SIGNAL(released()), this, SLOT(listen()));
    disconnect(ui->listenBtn, SIGNAL(released()), this, SLOT(endListen()));
}

void MainWindow::writeCharacter(char c, int pos) {
    ui->dstText->insertPlainText(QString(c));
}

void MainWindow::send() {
    /* Disable UI Elements */
    startSend();

    /*Create thread and start process */
    QThread * thread = new QThread;
    ClientDriver * cDriver = new ClientDriver(ui->srcText->toPlainText().toStdString(), ui->dstIP->text().toStdString(), ui->dstPort->text().toShort());
    cDriver->moveToThread(thread);
    connect(cDriver, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread,  SIGNAL(started()),  cDriver, SLOT(process()));
    connect(cDriver, SIGNAL(finished()), thread, SLOT(quit()));
    connect(cDriver, SIGNAL(finished()), cDriver, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // reset UI
    connect(cDriver, SIGNAL(finished()), this, SLOT(finishSend()));

    thread->start();
}

void MainWindow::errorString(QString err) {
    QErrorMessage msg(this);
    QErrorMessage::qtHandler()->showMessage(err);
}

void MainWindow::startSend() {
    ui->dstIP->setDisabled(true);
    ui->dstPort->setDisabled(true);
    ui->sendBtn->setDisabled(true);
    ui->srcText->setReadOnly(true);
}

void MainWindow::finishSend() {
    ui->dstIP->setDisabled(false);
    ui->dstPort->setDisabled(false);
    ui->sendBtn->setDisabled(false);
    ui->srcText->setReadOnly(false);
}

