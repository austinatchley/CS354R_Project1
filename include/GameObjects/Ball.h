#pragma once

#include <Ogre.h>
#include "GameObject.h"

namespace Game
{
    class Ball
        : public GameObject
    {
    public:
        Ball(float radius = 1, Ogre::String tex = "") {}
        ~Ball() {}

    private:
    };
}
