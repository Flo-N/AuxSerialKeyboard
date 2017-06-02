#include "volumehandler.h"

VolumeHandler::VolumeHandler(QObject *parent)
    : QObject(parent),
      endpointVolume(NULL)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, VolumeHandler::filter);
    timer->start(2);

    CoInitialize(NULL);
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
    IMMDevice *defaultDevice = NULL;

    deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();

    defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    defaultDevice->Release();

    endpointVolume->GetMasterVolumeLevelScalar(&setVolume);
    qDebug() <<"Start Volume" << setVolume;
}

VolumeHandler::~VolumeHandler()
{
    endpointVolume->Release();
    CoUninitialize();
}

float VolumeHandler::getVolumeScalar() const
{
    float volume = 0.0;
    endpointVolume->GetMasterVolumeLevelScalar(&volume);
    return volume;
}

void VolumeHandler::setVolumeScalar(const float volume)
{
    setVolume = volume;
}

void VolumeHandler::setVolumeScalar(const quint8 volume)
{
    if (volume == 0)
        setVolume = 0;
    else
        setVolume = (float)volume/255.0;
}

float VolumeHandler::filter(){
    static float out = 0.0;
    const float p1 = 0.1;
    const float p2 = 1.0 - p1;

    out = setVolume * p1 + out * p2;

    endpointVolume->SetMasterVolumeLevelScalar(out, NULL);
    return out;
}
