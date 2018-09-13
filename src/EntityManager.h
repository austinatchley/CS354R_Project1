#pragma once

namespace ECS
{
    using EntityId = unsigned long;

    class Entity
    {
        friend class EntityManager;
        static const EntityId Invalid = 0;

    public:
        Entity() = default;
        Entity(EntityId id)
            : mID(id)
        {}

        bool valid() const;
        void invalidate();

        //Component management
        template <typename C, typename... Types>
        void addComponent(Types&&... args);

        template <typename C>
        void removeComponent();

        template <typename C>
        bool hasComponent() const;

        // Operators
        operator bool() const
        {
            return valid();
        }

        bool operator==(const Entity& other)
        {
            return mID == other.mID;
        }

        bool operator!=(const Entity& other)
        {
            return !(*this == other);
        }

        bool operator<(const Entity& other)
        {
            return other.mID < mID;
        }

    private:
        const EntityId mID = Invalid;
    };

    class EntityManager
    {
    
    public:
        static EntityManager* getInstance()
        {
            if (!mInstance)
            {
                mInstance = new EntityManager();
            }

            return mInstance;
        }

        // Creates a new Entity and assigns an ID to it
        EntityId create();

        // Destroys a Entity and invalidates its ID 
        void destroy(EntityId entityId);

        // Returns the Entity with entityId
        Entity* get(EntityId entityId);

        // Returns the EntityId associated with this entity
        EntityId getID(Entity* entity);

    private:
        static EntityManager* mInstance;
        
        using EntityTable = std::unordered_map<EntityId, Entity*>;
        EntityTable mEntityTable;
    };
}
