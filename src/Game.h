#pragma once

#include <Ogre.h>
#include <OgreMath.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreApplicationContext.h>

#include <iostream>

#include "ECS/EventManager.h"
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Ball.h"
#include "Events/Events.h"
#include "Events/EventSubscribers.h"

#define WALL_TEX "cube.mesh"
#define BALL_TEX "sphere.mesh"
#define OGRE_TEX "ogrehead.mesh"

#define AMBIENT_LIGHT Ogre::ColourValue(0.5, 0.5, 0.5)
//#define AMBIENT_LIGHT Ogre::ColourValue(0.f, 0.f, 0.f)

using namespace Ogre;
using namespace OgreBites;

namespace Game
{
    class Game
            : public ApplicationContext
            , public InputListener
    {
    public:
        Game();
        virtual ~Game();

        void setup();
        bool keyPressed(const KeyboardEvent& evt);

        bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    private:
        Root* mRoot;
        SceneManager* mScnMgr;
        RTShader::ShaderGenerator* mShadergen;

        std::unique_ptr<ECS::EventManager> mEventManager;

        // Game specific member vars
        std::vector<Ogre::SceneNode*> mBalls;
        std::vector<Vector3> mBallVel;
        Ogre::SceneNode* mCamNode;

        const Real mWallSize = 20;
        const Vector3 mGravity = Vector3(0.f, -.098f, 0.f);
    };
} //namespace Game
