#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#pragma comment(lib, "winmm")

#include <QtSerialPort/QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    serial = new QSerialPort(this);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(this, &MainWindow::newSerialBufferReceived, this, &MainWindow::setStatusBar);
    connect(this, &MainWindow::newSerialBufferReceived, this, &MainWindow::triggerKey);

    // full volume:
    //waveOutSetVolume(NULL, 0xFFFF);

    // to mute:
    //waveOutSetVolume(NULL, 0);

    qDebug("init complete");
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Close");
    msgBox.setText("Do you realy want to close?");
    msgBox.setStandardButtons(QMessageBox::Close | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Cancel:
          event->ignore();
          break;
      case QMessageBox::Close:
          this->close();
          break;
      default:
          // should never be reached
          this->close();
          break;
    }
}

void MainWindow::readData(){

    dataRX.append(serial->readAll());

    if(dataRX.length() >= 4){
        newSerialBufferReceived(dataRX);
        dataRX.clear();
    }
}

void MainWindow::on_startButton_clicked()
{

    serial->setPortName("COM3");
    serial->setBaudRate(static_cast<QSerialPort::BaudRate>(ui->baudSpinBox->text().toInt()));
    serial->setDataBits(static_cast<QSerialPort::DataBits>(ui->dataBitsSpinBox->text().toInt()));
    serial->setParity(static_cast<QSerialPort::Parity>(0));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(0));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(0));
    serial->setReadBufferSize(0); // 0 == default

    if (serial->open(QIODevice::ReadWrite)) {

    } else {
        QMessageBox::critical(this, tr("QSerial error"), serial->errorString());
    }

    qDebug("Serial startup ended");
}

void MainWindow::on_stopButton_clicked()
{
    if (serial->isOpen()){
        serial->close();
        setStatusBar("Serial stoped");
    }
    qDebug() <<"Closed";
}

void MainWindow::setStatusBar(QString text){
    ui->statusBar->showMessage(text);
}

void MainWindow::triggerKey(QByteArray key){

    qDebug() << key;
    qint8 intKey = key.toDouble();
    qDebug() << intKey;
    switch (intKey) {
    case 5:
        qDebug() << "key = 5";
        break;
    default:
        break;
    }
}
