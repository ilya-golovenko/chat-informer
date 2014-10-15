//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>


// Sounds
enum
{
    SOUND_EVENT,
    SOUND_ERROR,

    SOUND_COUNT
};

class CSoundManager : public CManagerBase<CSoundManager>
{
public:
    CSoundManager() = default;
    ~CSoundManager() = default;

    virtual void Initialize();
    virtual void Finalize();

    void Play(int sound);

private:
    void CreateSounds();
};
