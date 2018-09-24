#include "Ball.h"

namespace Game
{

    Ball::Ball(
        Ogre::SceneManager* scnMgr,
        float radius = 1,
        const Ogre::String& material = "Examples/SphereMappedRustySteel",
        const Ogre::Vector3& velocity = Ogre::Vector3::ZERO
    )
        : mRadius(radius)
        , mMaterial(material)
        , mVelocity(velocity)
    {
        std::cout << " made ball " << std::endl;
        Ogre::Entity* ballEntity = scnMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
        ballEntity->setCastShadows(true);
        ballEntity->setMaterialName(material);

        mNode = scnMgr->getRootSceneNode()->createChildSceneNode();

        mNode->setScale(radius / 100.f, radius / 100.f, radius / 100.f);

        mNode->attachObject(ballEntity);
    }

    Ogre::SceneNode* Ball::getNode()
    {
        return mNode;
    }

    const Ogre::Vector3& Ball::getVelocity()
    {
        return mVelocity;
    }

    void Ball::setVelocity(const Ogre::Vector3& newVel)
    {
        mVelocity = newVel;
    }
}
