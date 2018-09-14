#include "EntityManager.h"

namespace ECS
{
    bool Entity::valid() const
    {
        return mID != InvalidId;
    }

    void Entity::invalidate()
    {
        mID = InvalidId;
    }

    template <typename C, typename... Types>
    void Entity::addComponent(Types&&... args)
    {
    }

}
