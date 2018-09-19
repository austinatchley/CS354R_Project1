#pragma once

#include <Ogre.h>
#include <OgreMath.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreApplicationContext.h>

#include <iostream>

#include "EventManager.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "Ball.h"

#define WALL_TEX "cube.mesh"
#define BALL_TEX "sphere.mesh"
#define OGRE_TEX "ogrehead.mesh"

using namespace Ogre;
using namespace OgreBites;

namespace Game
{
    class BasicTutorial1
            : public ApplicationContext
            , public InputListener
    {
    public:
        BasicTutorial1();
        virtual ~BasicTutorial1() {}

        void setup();
        bool keyPressed(const KeyboardEvent& evt);

        bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    private:
        Root* mRoot;
        SceneManager* mScnMgr;
        RTShader::ShaderGenerator* mShadergen;

        ECS::EventManager* mEventManager;

        // Game specific member vars
        SceneNode* mMainLightNode;

        SceneNode* mWallNode1;
        SceneNode* mWallNode2;
        SceneNode* mWallNode3;
        SceneNode* mWallNode4;
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
    
    class MoveEntityEventSubscriber
        : public ECS::EventSubscriber<MoveEntityEvent>
    {
    public:
        virtual ~MoveEntityEventSubscriber() {}

        virtual void receive(ECS::EventManager* eventManager, const MoveEntityEvent& event) override
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
} //namespace Game
