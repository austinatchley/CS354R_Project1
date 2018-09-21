#pragma once

#include <Ogre.h>

#include "ECS/EntityManager.h"

namespace Game
{
    class GameObject
        : public ECS::Entity
    {
    public:
        GameObject() {}
        ~GameObject() {}
    };
}
