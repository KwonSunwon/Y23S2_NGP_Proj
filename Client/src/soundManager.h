#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "stdafx.h"

class SoundManager
{
private:
    FMOD::System *soundSystem;
    FMOD::Sound *sound[5];
    FMOD::Channel *channel;
    bool isBgmPlay = false;

public:
    SoundManager();
    void soundPlay(int _type);
    void soundStop(int _type);
    bool getIsBgmPlay();
    void setIsBgmPlay(bool);
};

#endif // !__SOUND_MANAGER_H__