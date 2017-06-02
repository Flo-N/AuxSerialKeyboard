#include "serialhandler.h"

SerialHandler::SerialHandler(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &SerialHandler::readData);
}
void SerialHandler::readData(){
    emit newSerialBufferReceived(serial->readAll());
}
QString SerialHandler::start(QString com, qint64 baudRate, qint8 dataBits, qint8 parity, qint8 stopBits, qint8 flowcontrowl, qint8 bufferSize)
{

    serial->setPortName(com);
    serial->setBaudRate(static_cast<QSerialPort::BaudRate>(baudRate));
    serial->setDataBits(static_cast<QSerialPort::DataBits>(dataBits));
    serial->setParity(static_cast<QSerialPort::Parity>(parity));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(stopBits));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(flowcontrowl));
    serial->setReadBufferSize(bufferSize); // 0 == default

    if (serial->open(QIODevice::ReadWrite)) {
       return "";
    } else {
        return serial->errorString();
    }

    qDebug("Serial startup ended");
}
bool SerialHandler::stop()
{
    if (serial->isOpen()){
        serial->close();
        return (true);
    } else
        return(false);
}
