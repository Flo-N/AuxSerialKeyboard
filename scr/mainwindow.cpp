#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    volumeHandler(this),
    serialHandler(this)
{
    ui->setupUi(this);

    connect(&serialHandler, &SerialHandler::newSerialBufferReceived, this, &MainWindow::saveBuffer);

    connect(this, &MainWindow::newBuffer, this, &MainWindow::setStatusBar);
    connect(this, &MainWindow::newBuffer, this, &MainWindow::triggerKey);

    ui->faderGui->setValue(255 * volumeHandler.getVolumeScalar());

    this->setWindowTitle("ArduinoAuxKeyboard");

    ui->startButton->setFocus();
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

void MainWindow::saveBuffer(QByteArray buffer){

    dataRX.append(buffer);

    emit newBuffer(dataRX);
}

void MainWindow::on_startButton_clicked()
{

//    serial->setPortName(ui->comLineEdit->text());
//    serial->setBaudRate(static_cast<QSerialPort::BaudRate>(ui->baudSpinBox->text().toInt()));
//    serial->setDataBits(static_cast<QSerialPort::DataBits>(ui->dataBitsSpinBox->text().toInt()));
//    serial->setParity(static_cast<QSerialPort::Parity>(0));
//    serial->setStopBits(static_cast<QSerialPort::StopBits>(ui->stopBitSpinBox->text().toInt()));
//    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(0));
//    serial->setReadBufferSize(0); // 0 == default

    QString start = serialHandler.start(ui->comLineEdit->text(), ui->baudSpinBox->text().toInt(), ui->dataBitsSpinBox->text().toInt(), 0, ui->stopBitSpinBox->text().toInt(), 0, 0);

    if (start == "") {
        setStatusBar("Serial started");
    } else {
        setStatusBar("Serial startup failed: " + start);
    }

    qDebug("Serial startup ended");
}

void MainWindow::on_stopButton_clicked()
{
    if(serialHandler.stop())
        setStatusBar("Serial stoped");
    else
        setStatusBar("No open connection");
}

void MainWindow::setStatusBar(QString text){
    ui->statusBar->showMessage(text);
}

void MainWindow::triggerKey(){
    while(true){
        qDebug() << "dataRX: " << dataRX;

        qDebug() << "indexOf !: " << dataRX.indexOf("!");
        if (dataRX.indexOf("!",0) == -1) break;

        qDebug() << "last indexOf !: " << dataRX.lastIndexOf("!");
        if(dataRX.lastIndexOf("!") == dataRX.indexOf("!")) break;

        dataRX = dataRX.remove(0,dataRX.indexOf("!",0)+1);
        qDebug() << "After ! remove: " << dataRX;

        char buttonID = dataRX[0];
        qDebug() << "Button ID: " << buttonID;

        switch (buttonID) {
        case 'a': {
            qDebug() << "Datalength" << dataRX.indexOf("!") - dataRX.indexOf(";") - 1;
            qint8 value = dataRX.mid(dataRX.indexOf(";")+1, 1 ).toInt();
            qDebug() << "Value" << value;
            break;
        }
        case 'b': {
            qDebug() << "Datalength" << dataRX.indexOf("!") - dataRX.indexOf(";") - 1;
            qint8 value = dataRX.mid(dataRX.indexOf(";")+1,1).toInt();
            qDebug() << "Value" << value;
            break;
        }
        case 'z':{
            qint8 dataLength = dataRX.indexOf("!") - dataRX.indexOf(";") - 1;
            qDebug() << "Datalength" << dataLength;
            qint16 value = dataRX.mid(dataRX.indexOf(";")+1, dataLength).toInt();
            qDebug() << "Value" << value;
            qDebug() << (float)value / (float)1023.0;
            volumeHandler.setVolumeScalar((float)value / (float)1023.0);
            ui->faderGui->setValue(value * 0.21994134897);
            break;
        }
        default:
            break;
        }
    }
    qDebug() << "while return";
}

void MainWindow::on_faderGui_valueChanged(int value)
{
    volumeHandler.setVolumeScalar((quint8)value);
}
