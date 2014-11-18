//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

#include <informer/managers/SoundManager.h>
#include <informer/managers/ManagerFactory.h>
#include <informer/common/Logging.h>


void CSoundManager::Initialize()
{
    LOG_COMP_INFO(CSoundManager, "initializing");

    CreateSounds();
}

void CSoundManager::Finalize()
{
    LOG_COMP_INFO(CSoundManager, "finalizing");

}

void CSoundManager::Play(int sound)
{
    ::PlaySound(L"error.wav", nullptr, SND_ASYNC | SND_NODEFAULT | SND_NOSTOP | SND_NOWAIT | SND_FILENAME);
}

void CSoundManager::CreateSounds()
{
}
