#include "SoundManager.h"

namespace Game
{
    SoundManager::SoundManager()
    {
        SDL_Init(SDL_INIT_AUDIO);
        Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

        Mix_AllocateChannels(24);
        
        wallHit = Mix_LoadWAV("../media/sound/wallHit.wav");
    }

    void SoundManager::playWallHit()
    {
        if (!Mix_Playing(SoundChannel::Ball))
        {
            Mix_PlayChannel(SoundChannel::Ball, wallHit, 0);
        }
    }
}
