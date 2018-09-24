/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/


Copyright (c) 2000-2013 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/

#include "Game.h"

#include "Util/Util.h"

using namespace Ogre;
using namespace OgreBites;

namespace Game
{
    Game::Game()
        : ApplicationContext("Project1")
    {
    }

    Game::~Game()
    {
    }

    void Game::setup()
    {
        //////////////////////////////////////////////////////////////////
        // Application Setup
        ApplicationContext::setup();
        addInputListener(this);

        // get a pointer to the already created root
        mRoot = getRoot();
        mScnMgr = mRoot->createSceneManager();

        // register our scene with the RTSS
        mShadergen = RTShader::ShaderGenerator::getSingletonPtr();
        mShadergen->addSceneManager(mScnMgr);

        //////////////////////////////////////////////////////////////////
        // Lighting
        mScnMgr->setAmbientLight(AMBIENT_LIGHT);

        Ogre::Light* light = mScnMgr->createLight("MainLight");
        light->setDiffuseColour(Ogre::ColourValue::White);
        light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
        light->setType(Light::LT_SPOTLIGHT);
        light->setSpotlightRange(Degree(0), Degree(90));
        light->setAttenuation(mWallSize * 8.f, 1.f, 4.5f / (mWallSize * 8.f), 75.f / (mWallSize * mWallSize * 64.f));
        Ogre::SceneNode* mainLightNode = mScnMgr->getRootSceneNode()->createChildSceneNode("MainLight");
        mainLightNode->attachObject(light);
        mainLightNode->setPosition(0, mWallSize - 1.f, 0);
        mainLightNode->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

        Light* pointLight = mScnMgr->createLight("PointLight");
        pointLight->setType(Light::LT_POINT);
        pointLight->setDiffuseColour(0.85, 0.1, 0.1);
        pointLight->setSpecularColour(0.1, 0.1, 0.1);
        pointLight->setAttenuation(100, 1.0, 0.045, 0.0075);
        SceneNode* pointLightNode = mScnMgr->getRootSceneNode()->createChildSceneNode();
        pointLightNode->attachObject(pointLight);
        pointLightNode->setPosition(Vector3(0, mWallSize - 1.f, -mWallSize + 1.f));

        //////////////////////////////////////////////////////////////////
        // Camera
        mCamNode = mScnMgr->getRootSceneNode()->createChildSceneNode("camera");

        // create the camera
        Camera* cam = mScnMgr->createCamera("camera");
        cam->setNearClipDistance(0.1);
        cam->setAutoAspectRatio(true);
        cam->lookAt(Ogre::Vector3::ZERO);

        mCamNode->attachObject(cam);
        mCamNode->setPosition(0, 5, 80);

        // and tell it to render into the main window
        Ogre::Viewport* vp = getRenderWindow()->addViewport(cam);
        vp->setBackgroundColour(Ogre::ColourValue(0.1f, 0.1f, 0.15f));

        cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

        //////////////////////////////////////////////////////////////////
        // Balls
        for (int i = 0; i < NUM_BALLS; ++i)
        {
            Entity* ballEntity = mScnMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
            ballEntity->setCastShadows(true);
            ballEntity->setMaterialName("Examples/SphereMappedRustySteel");

            Ogre::SceneNode* ballNode = mScnMgr->getRootSceneNode()->createChildSceneNode();

            ballNode->setScale(BALL_RADIUS / 100.f, BALL_RADIUS / 100.f, BALL_RADIUS / 100.f);

            ballNode->attachObject(ballEntity);

            mBalls.push_back(ballNode);
            mBallPos.push_back(
                Vector3(
                    Math::RangeRandom(-mWallSize, mWallSize), 
                    Math::RangeRandom(-mWallSize, mWallSize),
                    Math::RangeRandom(-mWallSize, mWallSize)
                )
            );

            mBallVel.push_back(
                Vector3(
                    Math::RangeRandom(-20.0, 20.0),
                    Math::RangeRandom(-20.0, 20.0),
                    Math::RangeRandom(-20.0, 20.0)
                )
            );
        }

        //////////////////////////////////////////////////////////////////
        // Planes
        static const std::unordered_map<String, Vector3> planeNameToAxis = {
            {"left",    Vector3::UNIT_X}, {"right", Vector3::NEGATIVE_UNIT_X},
            {"ground",  Vector3::UNIT_Y}, {"ceil",  Vector3::NEGATIVE_UNIT_Y},
            {"back",    Vector3::UNIT_Z}, {"front", Vector3::NEGATIVE_UNIT_Z}
        };

        for (const auto entry : planeNameToAxis)
        {
            auto name = entry.first;
            auto norm = entry.second;

            Plane plane(norm, -mWallSize);
            MeshManager::getSingleton().createPlane(name,
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                plane,
                mWallSize * 2.f, mWallSize * 2.f, 20, 20,
                true,
                1, 5, 5,
                norm.perpendicular()); 

            Entity* planeEntity = mScnMgr->createEntity(name);
            planeEntity->setCastShadows(false);
            planeEntity->setMaterialName("Examples/BeachStones");

            SceneNode* planeNode = mScnMgr->getRootSceneNode()->createChildSceneNode(name);
            planeNode->attachObject(planeEntity);

            mWalls.push_back(plane);
        }

        //////////////////////////////////////////////////////////////////
        // Event Manager
        mEventManager.reset(new ECS::EventManager(std::allocator<void>()));

        TransformEntitySubscriber* moveEntitySub = new TransformEntitySubscriber();
        mEventManager->connect<TransformEntityEvent>(moveEntitySub);

        //////////////////////////////////////////////////////////////////
        // Sound Manager
        mSoundManager.reset(new SoundManager());
        mEventManager->connect<PlaySoundEvent>(mSoundManager.get());
    }

    bool Game::keyPressed(const KeyboardEvent& evt)
    {
        static const Real mag = 0.5f;
        static const Ogre::Vector3 rightVec = Ogre::Vector3(mag, 0.f, 0.f);
        static const Ogre::Vector3 leftVec  = Ogre::Vector3(-mag, 0.f, 0.f);
        static const Ogre::Vector3 upVec    = Ogre::Vector3(0.f, mag, 0.f);
        static const Ogre::Vector3 downVec  = Ogre::Vector3(0.f, -mag, 0.f);
        static const Ogre::Vector3 backVec  = Ogre::Vector3(0.f, 0.f, mag);
        static const Ogre::Vector3 forVec   = Ogre::Vector3(0.f, 0.f, -mag);

        switch (evt.keysym.sym)
        {
        case OgreBites::SDLK_ESCAPE: // Exit the game
            getRoot()->queueEndRendering();
            break;

        case 'w':
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, forVec * 5.f)));
            break;
        case 's':
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, backVec * 5.f)));
            break;
        case 'a':
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, leftVec * 5.f)));
            break;
        case 'd':
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, rightVec * 5.f)));
            break; 

        case OgreBites::SDLK_PAGEUP:
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, upVec * 5.f)));
            break; 
        case OgreBites::SDLK_PAGEDOWN: // For some reason the 'End' key seems to trigger this keybinding
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, downVec * 5.f)));
            break;

        case OgreBites::SDLK_UP:
            mEventManager->event<TransformEntityEvent>(*(new RotateEntityEvent(mCamNode, rightVec * 0.25f)));
            break;
        case OgreBites::SDLK_DOWN:
            mEventManager->event<TransformEntityEvent>(*(new RotateEntityEvent(mCamNode, leftVec * 0.25f)));
            break;
        case OgreBites::SDLK_LEFT:
            mEventManager->event<TransformEntityEvent>(*(new RotateEntityEvent(mCamNode, upVec * 0.25f)));
            break;
        case OgreBites::SDLK_RIGHT:
            mEventManager->event<TransformEntityEvent>(*(new RotateEntityEvent(mCamNode, downVec * 0.25f)));
            break; 

        default:
            return false;
        }

        return true;
    }

    bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt)
    {
        const Real dt = evt.timeSinceLastFrame;

        // Check each ball for collisions
        for (int i = 0; i < mBalls.size(); ++i)
        {
            const auto& prevPos = mBallPos[i];
            Vector3 deltaPos = Vector3::ZERO;

            // Check each wall to see if our ball has collided with it
            for (int j = 0; j < mWalls.size(); ++j)
            {
                if (mWalls[j].getDistance(prevPos) <= BALL_RADIUS
                        && mBallVel[i].dotProduct(mWalls[j].normal) < 0) 
                {
                    const auto& norm = mWalls[j].normal;

                    mBallVel[i] -= 2 * norm * (norm.dotProduct(mBallVel[i]));
                    deltaPos += norm * dt; // Lift the ball off the plane slightly so it doesn't get stuck

                    // Play the wall hit sound
                    mEventManager->event<PlaySoundEvent>(*(new PlaySoundEvent(Util::Sound::Ball)));
                }
            }

#ifdef APPLY_GRAVITY
            mBallVel[i] += mGravity * dt;
#endif

            deltaPos += mBallVel[i] * dt;
            const Vector3 newPos = prevPos + deltaPos;

            const auto& ball = mBalls[i];
            ball->setPosition(newPos);

            mBallPos[i] = newPos;
        }

        // Tell the EventManager to update and dispatch the events in the queue
        mEventManager->update();

        return true;
    }

} // namespace Game

int main(int argc, char **argv)
{
    try
    {
        Game::Game app;
        app.initApp();
        app.getRoot()->startRendering();
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}


