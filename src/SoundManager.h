#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_audio.h>

namespace Game
{
    enum SoundChannel
    {
        Ball = 0
    };

    class SoundManager
    {
    public:
        SoundManager();

        void playWallHit();

    protected:
        Mix_Chunk* wallHit;
        std::size_t flags;
    };
}
