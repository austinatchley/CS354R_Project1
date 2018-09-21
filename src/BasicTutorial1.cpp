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
#include "BasicTutorial1.h"

using namespace Ogre;
using namespace OgreBites;

namespace Game
{
    BasicTutorial1::BasicTutorial1()
        : ApplicationContext("OgreTutorialApp")
    {
    }

    BasicTutorial1::~BasicTutorial1()
    {
    }

    void BasicTutorial1::setup()
    {
        // do not forget to call the base first
        ApplicationContext::setup();
        addInputListener(this);

        // get a pointer to the already created root
        mRoot = getRoot();
        mScnMgr = mRoot->createSceneManager();

        // register our scene with the RTSS
        mShadergen = RTShader::ShaderGenerator::getSingletonPtr();
        mShadergen->addSceneManager(mScnMgr);

        // -- tutorial section start --
        mScnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

        Light* light = mScnMgr->createLight("MainLight");
        mMainLightNode = mScnMgr->getRootSceneNode()->createChildSceneNode();
        mMainLightNode->attachObject(light);

        mMainLightNode->setPosition(0, 100, 0);
        mMainLightNode->setOrientation(Ogre::Quaternion(0.707f, 0.f, 0.f, -0.707f));

        SceneNode* camNode = mScnMgr->getRootSceneNode()->createChildSceneNode();

        // create the camera
        Camera* cam = mScnMgr->createCamera("myCam");
        cam->setNearClipDistance(5); // specific to this sample
        cam->setAutoAspectRatio(true);
        camNode->attachObject(cam);
        camNode->setPosition(0, 0, 140);

        // and tell it to render into the main window
        getRenderWindow()->addViewport(cam);

        Entity* wallEntity1 = mScnMgr->createEntity(OGRE_TEX);
        mWallNode1 = mScnMgr->getRootSceneNode()->createChildSceneNode();
        mWallNode1->attachObject(wallEntity1);

        camNode->setPosition(0, 47, 222);

        Entity* wallEntity2 = mScnMgr->createEntity(OGRE_TEX);
        mWallNode2 = mScnMgr->getRootSceneNode()->createChildSceneNode(Vector3(84, 48, 0));
        mWallNode2->attachObject(wallEntity2);

        Entity* wallEntity3 = mScnMgr->createEntity(OGRE_TEX);
        mWallNode3 = mScnMgr->getRootSceneNode()->createChildSceneNode();
        mWallNode3->setPosition(0, 104, 0);
        mWallNode3->setScale(2, 1.2, 1);
        mWallNode3->attachObject(wallEntity3);

        Entity* wallEntity4 = mScnMgr->createEntity(OGRE_TEX);
        mWallNode4 = mScnMgr->getRootSceneNode()->createChildSceneNode();
        mWallNode4->setPosition(-84, 48, 0);
        mWallNode4->roll(Degree(-90));
        mWallNode4->attachObject(wallEntity4);

        std::cout << "\n\nEND SETUP()\n\n" << std::endl;
        // -- tutorial section end --
        
        mEventManager.reset(new ECS::EventManager(std::allocator<void>()));

        TestEventSubscriber* sub = new TestEventSubscriber();
        mEventManager->connect<TestEvent>(sub);

        MoveEntityEventSubscriber* moveEntitySub = new MoveEntityEventSubscriber();
        mEventManager->connect<MoveEntityEvent>(moveEntitySub);

        TestEvent *e = new TestEvent(1, 'a');
        mEventManager->event<TestEvent>(*e);

        const Ogre::Vector3 translation(40.f, 0.f, -100.f);
        MoveEntityEvent *me = new MoveEntityEvent(mWallNode4, translation, Ogre::Vector3::ZERO);
        mEventManager->event<MoveEntityEvent>(*me);
    }

    bool BasicTutorial1::keyPressed(const KeyboardEvent& evt)
    {
        Ogre::Vector3 leftVec = Ogre::Vector3(0.f, 0.f, 0.1f);
        Ogre::Vector3 rightVec = Ogre::Vector3(0.f, 0.f, -0.1f);

        switch (evt.keysym.sym)
        {
        case SDLK_ESCAPE:
            getRoot()->queueEndRendering();
            break;

        case SDLK_SPACE:
            break;

        case SDLK_LEFT:
            mEventManager->event<MoveEntityEvent>(*(new RotateEntityEvent(mWallNode1, leftVec)));
            mEventManager->event<MoveEntityEvent>(*(new RotateEntityEvent(mWallNode2, leftVec)));
            mEventManager->event<MoveEntityEvent>(*(new RotateEntityEvent(mWallNode3, leftVec)));
            mEventManager->event<MoveEntityEvent>(*(new RotateEntityEvent(mWallNode4, leftVec)));
            break;
        case SDLK_RIGHT:
            mEventManager->event<MoveEntityEvent>(*(new RotateEntityEvent(mWallNode1, rightVec)));
            mEventManager->event<MoveEntityEvent>(*(new RotateEntityEvent(mWallNode2, rightVec)));
            mEventManager->event<MoveEntityEvent>(*(new RotateEntityEvent(mWallNode3, rightVec)));
            mEventManager->event<MoveEntityEvent>(*(new RotateEntityEvent(mWallNode4, rightVec)));
            break;

        default:
            break;
        }

        return true;
    }

    bool BasicTutorial1::frameRenderingQueued(const Ogre::FrameEvent& evt)
    {
        mEventManager->update();

        return true;
    }

} // namespace Game

int main(int argc, char **argv)
{
    try
    {
        Game::BasicTutorial1 app;
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


