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

#include "SoundManager.h"

// Uncomment this line to apply gravity
//#define APPLY_GRAVITY

#define WALL_TEX "cube.mesh"
#define BALL_TEX "sphere.mesh"
#define OGRE_TEX "ogrehead.mesh"

#define NUM_BALLS 100
#define BALL_RADIUS 1.f

#define LIGHT_INTENSITY 0.2
#define AMBIENT_LIGHT Ogre::ColourValue(LIGHT_INTENSITY, LIGHT_INTENSITY, LIGHT_INTENSITY)

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

        Ogre::SceneNode* mCamNode;

        std::unique_ptr<ECS::EventManager> mEventManager;
        std::unique_ptr<SoundManager> mSoundManager;

        // Game specific member vars
        std::vector<Ogre::SceneNode*> mBalls;
        std::vector<Vector3> mBallVel;
        std::vector<Ogre::Plane> mWalls;
        std::vector<Vector3> mBallPos;

        const Real mWallSize = 20;
        const Vector3 mGravity = Vector3(0.f, -60.f, 0.f);
    };
} //namespace Game
