#include "volumehandler.h"

VolumeHandler::VolumeHandler(QObject *parent)
    : QObject(parent),
      endpointVolume(NULL)
{
    CoInitialize(NULL);
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
    IMMDevice *defaultDevice = NULL;

    deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();

    defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    defaultDevice->Release();
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
    endpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
}

void VolumeHandler::setVolumeScalar(const quint8 volume)
{
    if (volume == 0)
        endpointVolume->SetMasterVolumeLevelScalar(0, NULL);
    else
        endpointVolume->SetMasterVolumeLevelScalar((float)volume/255.0, NULL);
}
