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
        //! [turnlights]
        mScnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
        //! [turnlights]

        //! [newlight]
        Light* light = mScnMgr->createLight("MainLight");
        SceneNode* lightNode = mScnMgr->getRootSceneNode()->createChildSceneNode();
        lightNode->attachObject(light);
        //! [newlight]

        //! [lightpos]
        lightNode->setPosition(20, 80, 50);
        //! [lightpos]

        //! [camera]
        SceneNode* camNode = mScnMgr->getRootSceneNode()->createChildSceneNode();

        // create the camera
        Camera* cam = mScnMgr->createCamera("myCam");
        cam->setNearClipDistance(5); // specific to this sample
        cam->setAutoAspectRatio(true);
        camNode->attachObject(cam);
        camNode->setPosition(0, 0, 140);

        // and tell it to render into the main window
        getRenderWindow()->addViewport(cam);
        //! [camera]

        //! [entity1]
        Entity* ogreEntity = mScnMgr->createEntity("ogrehead.mesh");
        //! [entity1]

        //! [entity1node]
        SceneNode* ogreNode = mScnMgr->getRootSceneNode()->createChildSceneNode();
        //! [entity1node]

        //! [entity1nodeattach]
        ogreNode->attachObject(ogreEntity);
        //! [entity1nodeattach]

        //! [cameramove]
        camNode->setPosition(0, 47, 222);
        //! [cameramove]

        //! [entity2]
        Entity* ogreEntity2 = mScnMgr->createEntity("ogrehead.mesh");
        SceneNode* ogreNode2 = mScnMgr->getRootSceneNode()->createChildSceneNode(Vector3(84, 48, 0));
        ogreNode2->attachObject(ogreEntity2);
        //! [entity2]

        //! [entity3]
        Entity* ogreEntity3 = mScnMgr->createEntity("ogrehead.mesh");
        SceneNode* ogreNode3 = mScnMgr->getRootSceneNode()->createChildSceneNode();
        ogreNode3->setPosition(0, 104, 0);
        ogreNode3->setScale(2, 1.2, 1);
        ogreNode3->attachObject(ogreEntity3);
        //! [entity3]

        //! [entity4]
        Entity* ogreEntity4 = mScnMgr->createEntity("ogrehead.mesh");
        SceneNode* ogreNode4 = mScnMgr->getRootSceneNode()->createChildSceneNode();
        ogreNode4->setPosition(-84, 48, 0);
        ogreNode4->roll(Degree(-90));
        ogreNode4->attachObject(ogreEntity4);
        //! [entity4]

        // -- tutorial section end --
        
        mEventManager = new ECS::EventManager(std::allocator<void>());

        TestEventSubscriber* sub = new TestEventSubscriber();
        mEventManager->connect<TestEvent>(sub);

        MoveEntityEventSubscriber* moveEntitySub = new MoveEntityEventSubscriber();
        mEventManager->connect<MoveEntityEvent>(moveEntitySub);

        mEventManager->event<TestEvent>({ 1, 'a' });

        Ogre::Vector3 translation = Ogre::Vector3(40.f, 0.f, -100.f);
        mEventManager->event<MoveEntityEvent>({ ogreNode4, translation });
    }

    static int counter = 0;
    bool BasicTutorial1::keyPressed(const KeyboardEvent& evt)
    {
        switch (evt.keysym.sym)
        {
        case SDLK_ESCAPE:
            getRoot()->queueEndRendering();
            break;
        case SDLK_SPACE:
            mEventManager->event<TestEvent>({ ++counter, static_cast<char>('a' + (counter % 26)) });
            break;
        default:
            break;
        }

        return true;
    }
    //! [fullsource]
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


