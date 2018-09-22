#pragma once
   
#include "ECS/EventManager.h"
#include "Events.h"

class TransformEntityEventSubscriber
    : public ECS::EventSubscriber<TransformEntityEvent>
{
public:
    virtual ~TransformEntityEventSubscriber() {}

    virtual void receive(ECS::EventManager* eventManager, const TransformEntityEvent& event) override
    {
        Ogre::SceneNode* node = event.entityNode;

        std::cout << "MoveEntity was emitted, translation = (" <<
            event.translation.x << ", " <<
            event.translation.y << ", " <<
            event.translation.z << 
            "), rotation = (" << 
            event.rotation.x << ", " <<
            event.rotation.y << ", " <<
            event.rotation.z << 
            ")" << std::endl;

        // Translate
        const Ogre::Vector3 translatedPosition = node->getPosition() + event.translation;
        node->setPosition(translatedPosition);

        // Rotate
        node->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(event.rotation.x)); 
        node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(event.rotation.y)); 
        node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(event.rotation.z));
    }
};

class TestEventSubscriber
    : public ECS::EventSubscriber<TestEvent>
{
public:
    virtual ~TestEventSubscriber() {}

    virtual void receive(ECS::EventManager* eventManager, const TestEvent& event) override
    {
        std::cout << "TestEvent was emitted with i = " << event.i << ", c = " << event.c << " " << std::endl;
    }
};

