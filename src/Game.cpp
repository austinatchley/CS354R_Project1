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

//! [fullsource]
#include "Game.h"

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
        ApplicationContext::setup();
        addInputListener(this);

        // get a pointer to the already created root
        mRoot = getRoot();
        mScnMgr = mRoot->createSceneManager();

        // register our scene with the RTSS
        mShadergen = RTShader::ShaderGenerator::getSingletonPtr();
        mShadergen->addSceneManager(mScnMgr);

        mScnMgr->setAmbientLight(AMBIENT_LIGHT);

        Ogre::Light* light = mScnMgr->createLight("MainLight");
        light->setDiffuseColour(Ogre::ColourValue::White);
        light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

        Ogre::SceneNode* mainLightNode = mScnMgr->getRootSceneNode()->createChildSceneNode("MainLight");
        mainLightNode->attachObject(light);

        mainLightNode->setPosition(0, 200, 0);
        mainLightNode->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

        mCamNode = mScnMgr->getRootSceneNode()->createChildSceneNode("myCam");

        // create the camera
        Camera* cam = mScnMgr->createCamera("camera");
        cam->setNearClipDistance(0.1);
        cam->setAutoAspectRatio(true);
        cam->lookAt(Ogre::Vector3::ZERO);

        mCamNode->attachObject(cam);
        mCamNode->setPosition(0, 10, 200);

        // and tell it to render into the main window
        Ogre::Viewport* vp = getRenderWindow()->addViewport(cam);
        vp->setBackgroundColour(Ogre::ColourValue(0.1f, 0.1f, 0.15f));

        cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

        for (int i = 0; i < 2; ++i)
        {
            Entity* ballEntity = mScnMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
            ballEntity->setCastShadows(true);

            Ogre::SceneNode* ballNode = mScnMgr->getRootSceneNode()->createChildSceneNode();

            ballNode->setPosition(Ogre::Math::RangeRandom(-mWallSize, mWallSize), Ogre::Math::RangeRandom(-mWallSize, mWallSize), 0);
            ballNode->setScale(0.01f, 0.01f, 0.01f);

            ballNode->attachObject(ballEntity);

            mBalls.push_back(ballNode);
            mBallVel.push_back(Vector3(0.f, 0.f, 0.f));
        }

        static const std::unordered_map<String, Vector3> planeNameToAxis = {
            {"ground",  Vector3::UNIT_Y}, {"ceil",  Vector3::NEGATIVE_UNIT_Y},
            {"left",    Vector3::UNIT_X}, {"right", Vector3::NEGATIVE_UNIT_X},
            {"back",    Vector3::UNIT_Z}
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
            planeEntity->setMaterialName("Examples/Rockwall");

            SceneNode* planeNode = mScnMgr->getRootSceneNode()->createChildSceneNode(name);
            planeNode->attachObject(planeEntity);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        mEventManager.reset(new ECS::EventManager(std::allocator<void>()));

        TransformEntityEventSubscriber* moveEntitySub = new TransformEntityEventSubscriber();
        mEventManager->connect<TransformEntityEvent>(moveEntitySub);
    }

    bool Game::keyPressed(const KeyboardEvent& evt)
    {
        static const Real mag = 0.5f;
        static const Ogre::Vector3 leftVec  = Ogre::Vector3(-mag, 0.f, 0.f);
        static const Ogre::Vector3 rightVec = Ogre::Vector3(mag, 0.f, 0.f);
        static const Ogre::Vector3 forVec   = Ogre::Vector3(0.f, 0.f, -mag);
        static const Ogre::Vector3 backVec  = Ogre::Vector3(0.f, 0.f, mag);
        static const Ogre::Vector3 upVec    = Ogre::Vector3(0.f, mag, 0.f);
        static const Ogre::Vector3 downVec  = Ogre::Vector3(0.f, -mag, 0.f);

        //SceneNode* lightNode = mScnMgr->getSceneNode("MainLight");

        switch (evt.keysym.sym)
        {
        case SDLK_ESCAPE:
            getRoot()->queueEndRendering();
            break;

        case SDLK_SPACE:
            break;
         
        case 'w':
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, forVec * 10.f)));
            break;
        case 's':
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, backVec * 10.f)));
            break;

        case SDLK_PAGEUP:
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, upVec * 10.f)));
            break; 
        case SDLK_F12:
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, downVec * 10.f)));
            break;

       case 'a':
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, leftVec * 5.f)));
            break;
        case 'd':
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(mCamNode, rightVec * 5.f)));
            break; 


        case SDLK_UP:
            mEventManager->event<TransformEntityEvent>(*(new RotateEntityEvent(mCamNode, upVec)));
            break;
        case SDLK_DOWN:
            mEventManager->event<TransformEntityEvent>(*(new RotateEntityEvent(mCamNode, downVec)));
            break;


        case SDLK_LEFT:
            mEventManager->event<TransformEntityEvent>(*(new RotateEntityEvent(mCamNode, leftVec)));
            break;
        case SDLK_RIGHT:
            mEventManager->event<TransformEntityEvent>(*(new RotateEntityEvent(mCamNode, rightVec)));
            break; 

        default:
            break;
        }

        return true;
    }

    bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt)
    {
        Real dt = evt.timeSinceLastFrame;
        /*
        SceneNode* ballNode = mScnMgr->getSceneNode("ball");

        if (ballNode)
        {
            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(ballNode, Ogre::Vector3(50.f * dt * Ogre::Math::RangeRandom(-1.f, 1.f), 0.f, 0.f))));
        }
        */

        for (int i = 0; i < mBalls.size(); ++i)
        {
            auto ball = mBalls[i];
            mBallVel[i] += mGravity * dt;

            Vector3 translatedVec = ball->getPosition();
            translatedVec += mBallVel[i] * dt;

            std::cout << "ID: " << i << ", dt: " << dt << ", Translating " << translatedVec.y << " in the y" << std::endl;

            mEventManager->event<TransformEntityEvent>(*(new TranslateEntityEvent(ball, translatedVec)));
        }

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


