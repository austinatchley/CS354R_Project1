#pragma once

#include <Ogre.h>
#include "GameObject.h"

namespace Game
{
    class Ball
        : public GameObject
    {
    public:
        Ball(float radius, Ogre::String tex);
        ~Ball();

    private:
    };
}
