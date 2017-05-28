#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QString>

#include "volumehandler.h"

#include <windows.h>

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void readData();

private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

    void setStatusBar(QString);

    void closeEvent(QCloseEvent *event);

    void triggerKey();

    void on_faderGui_valueChanged(int value);

signals:

    void newSerialBufferReceived(QByteArray);

private:
    Ui::MainWindow *ui;

    QSerialPort *serial;

    QByteArray dataRX;

    VolumeHandler volumeHandler;
};

#endif // MAINWINDOW_H
