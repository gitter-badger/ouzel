// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "AudioDS.h"
#include "SoundDS.h"
#include "core/Engine.h"
#include "core/windows/WindowWin.h"
#include "utils/Log.h"

namespace ouzel
{
    namespace audio
    {
        AudioDS::AudioDS():
            Audio(Driver::DIRECTSOUND)
        {
        }

        AudioDS::~AudioDS()
        {
            resourceDeleteSet.clear();
            resources.clear();

            if (listener3D) listener3D->Release();
            if (buffer) buffer->Release();
            if (directSound) directSound->Release();
        }

        bool AudioDS::init()
        {
            if (FAILED(DirectSoundCreate8(nullptr, &directSound, nullptr)))
            {
                Log(Log::Level::ERR) << "Failed to create DirectSound 8 instance";
                return false;
            }

            WindowWin* windowWin = static_cast<WindowWin*>(sharedEngine->getWindow());

            if (FAILED(directSound->SetCooperativeLevel(windowWin->getNativeWindow(), DSSCL_PRIORITY)))
            {
                Log(Log::Level::ERR) << "Failed to set cooperative level for DirectSound 8";
                return false;
            }

            DSBUFFERDESC bufferDesc;
            bufferDesc.dwSize = sizeof(bufferDesc);
            bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
            bufferDesc.dwBufferBytes = 0;
            bufferDesc.dwReserved = 0;
            bufferDesc.lpwfxFormat = nullptr;
            bufferDesc.guid3DAlgorithm = GUID_NULL;

            if (FAILED(directSound->CreateSoundBuffer(&bufferDesc, &buffer, nullptr)))
            {
                Log(Log::Level::ERR) << "Failed to create DirectSound buffer";
                return false;
            }

            WAVEFORMATEX waveFormat;
            waveFormat.wFormatTag = WAVE_FORMAT_PCM;
            waveFormat.nChannels = 2;
            waveFormat.nSamplesPerSec = 44100;
            waveFormat.wBitsPerSample = 16;
            waveFormat.nBlockAlign = waveFormat.nChannels * (waveFormat.wBitsPerSample / 8);
            waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
            waveFormat.cbSize = 0;

            if (FAILED(buffer->SetFormat(&waveFormat)))
            {
                Log(Log::Level::ERR) << "Failed to set DirectSound buffer format";
                return false;
            }

            if (FAILED(buffer->QueryInterface(IID_IDirectSound3DListener, reinterpret_cast<void**>(&listener3D))))
            {
                Log(Log::Level::ERR) << "Failed to get DirectSound listener";
                return false;
            }

            return Audio::init();
        }

        bool AudioDS::update()
        {
            Audio::update();

            std::lock_guard<std::mutex> lock(uploadMutex);

            if (dirty & DIRTY_LISTENER_POSITION)
            {
                if (FAILED(listener3D->SetPosition(listenerPosition.v[0], listenerPosition.v[1], listenerPosition.v[2], DS3D_IMMEDIATE)))
                {
                    Log(Log::Level::ERR) << "Failed to set DirectSound listener position";
                    return false;
                }
            }

            if (dirty & DIRTY_LISTENER_ROTATION)
            {
                Vector3 forwardVector = listenerRotation.getForwardVector();
                Vector3 upVector = listenerRotation.getUpVector();

                if (FAILED(listener3D->SetOrientation(forwardVector.v[0], forwardVector.v[1], forwardVector.v[2],
                                                      upVector.v[0], upVector.v[1], upVector.v[2],
                                                      DS3D_IMMEDIATE)))
                {
                    Log(Log::Level::ERR) << "Failed to set DirectSound listener position";
                    return false;
                }
            }

            dirty = 0;

            return true;
        }

        SoundResource* AudioDS::createSound()
        {
            SoundResource* sound = new SoundDS();
            resources.push_back(std::unique_ptr<SoundResource>(sound));
            return sound;
        }
    } // namespace audio
} // namespace ouzel