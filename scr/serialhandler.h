#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class SerialHandler : public QObject
{
    Q_OBJECT
    void readData();

    QSerialPort *serial;
public:
    explicit SerialHandler(QObject *parent = 0);

    bool stop();
    QString start(QString com, qint64 baud, qint8 dataBits, qint8 parity, qint8 stopBits, qint8 flowcontrowl, qint8 bufferSize);

signals:

    void newSerialBufferReceived(QByteArray);

public slots:


};

#endif // SERIALHANDLER_H
