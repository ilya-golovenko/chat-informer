//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

#include <informer/managers/SoundManager.h>
#include <informer/managers/ManagerFactory.h>
#include <informer/common/Logging.h>


CSoundManager::CSoundManager()
{
}

void CSoundManager::Initialize()
{
    LOG_INFO("initializing");

    CreateSounds();
}

void CSoundManager::Finalize()
{
    LOG_INFO("finalizing");

}

void CSoundManager::Play(int sound)
{
    ::PlaySound(L"error.wav", NULL, SND_ASYNC | SND_NODEFAULT | SND_NOSTOP | SND_NOWAIT | SND_FILENAME);
}

void CSoundManager::CreateSounds()
{
}
