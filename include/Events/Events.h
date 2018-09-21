#pragma once

#include <Ogre.h>

#include "ECS/EventManager.h"

class TestEvent
    : public ECS::BaseEvent
{
public:
    TestEvent(int pi, char pc)
        : i(pi)
        , c(pc)
    {}
    ~TestEvent() {}

    int i;
    char c;
};

class MoveEntityEvent
    : public ECS::BaseEvent
{
public:
    MoveEntityEvent(
        Ogre::SceneNode* e,
        Ogre::Vector3 t,
        Ogre::Vector3 r)
        : entityNode(e)
        , translation(t)
        , rotation(r)
    {}
    ~MoveEntityEvent() {}

    Ogre::SceneNode* entityNode;
    Ogre::Vector3 translation;
    Ogre::Vector3 rotation;
};

class RotateEntityEvent
    : public MoveEntityEvent
{
public:
    RotateEntityEvent(Ogre::SceneNode* n, Ogre::Vector3 r)
        : MoveEntityEvent(n, Ogre::Vector3::ZERO, r)
    {
    }

private:
    MoveEntityEvent* mMoveEntityEvent;
};
