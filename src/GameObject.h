#pragma once

#include <Ogre.h>

#include "EntityManager.h"

namespace Game
{
    class GameObject
        : public ECS::Entity
    {
    public:
        GameObject();
        ~GameObject();
    };
}
