#ifndef VOLUMEHANDLER_H
#define VOLUMEHANDLER_H

#include <QObject>
#include <QTimer>

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include <QDebug>

class VolumeHandler : public QObject
{
    Q_OBJECT
public:
    explicit VolumeHandler(QObject *parent = 0);
    ~VolumeHandler();

    float getVolumeScalar() const;
    void setVolumeScalar(const float volume);
    void setVolumeScalar(const quint8 volume);

private:
    IAudioEndpointVolume *endpointVolume;

signals:

public slots:


};

#endif // VOLUMEHANDLER_H
