#pragma once

#include <Ogre.h>

#include "EntityManager.h"
#include "ComponentManager.h"

#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>

namespace ECS
{
    class EventManager;
    class BaseEventSubscriber;

    typedef std::allocator<Entity> Allocator;
    using TypeIndex = std::type_index;

    using SubscriberPtrAllocator = std::allocator_traits<Allocator>::template rebind_alloc<BaseEventSubscriber*>; 
	using EntityPtrAllocator = std::allocator_traits<Allocator>::template rebind_alloc<Entity*>;

    using SubscriberPairAllocator = std::allocator_traits<Allocator>::template rebind_alloc<std::pair<const TypeIndex, std::vector<BaseEventSubscriber*, SubscriberPtrAllocator>>>;
	using EntityAllocator = std::allocator_traits<Allocator>::template rebind_alloc<Entity>;

    template<typename T>
    TypeIndex getTypeIndex()
    {
        return std::type_index(typeid(T));
    }

    /////////////////////////////////////////////////////////////////
    class BaseEventSubscriber
    {
    public:
        virtual ~BaseEventSubscriber() {}
    };

    /////////////////////////////////////////////////////////////////
    template <typename Event>
    class EventSubscriber
        : public BaseEventSubscriber
    {
    public:
        virtual ~EventSubscriber() {}

        virtual void receive(EventManager* manager, const Event& event) = 0;
    };

    /////////////////////////////////////////////////////////////////
    /*class EventHandler
    {
    	typedef void (Component::* Fptr)(void);
	    friend class EventManager;

    public:
    	EventHandler(Component* component, Fptr& callback)
            : mComponent(component)
            , mCallback(callback)
        {}
            
    private:
        template <class ...Types>
        void execute(Types... args)
        {
       	    (mComponent->*mCallback)(args...);
       	}

        Component* mComponent;
    	Fptr mCallback;
    };    
    */

    class EventManager
    {
    public:
        template <typename Event>
        void connect(EventSubscriber<Event>* subscriber)
        {
            auto index = getTypeIndex<Event>();
            auto it = mSubscribers.find(index); 

            if (it == mSubscribers.end())
            {
                std::vector<BaseEventSubscriber*, SubscriberPtrAllocator> subList(mEntAlloc);
                subList.push_back(subscriber);

                mSubscribers.insert({ index, subList });
            }
            else
            {
                it->second.push_back(subscriber);
            }
        }

        template <typename Event>
        void disconnect(EventSubscriber<Event>* subscriber)
        {
            auto index = getTypeIndex<Event>();
            auto it = mSubscribers.find(index);

            if (it != mSubscribers.end())
            {
                it->second.erase(subscriber);
                
                if (it->second.size() == 0)
                {
                    mSubscribers.erase(it);
                }
            }
        }

	    void update();

        template <typename Event>
        void event(Event&& event)
        {
            auto it = mSubscribers.find(getTypeIndex<Event>());

            if (it != mSubscribers.end())
            {
                const auto& subscribers = it->second;
                for (auto* base : subscribers)
                {
                    auto* sub = reinterpret_cast<EventSubscriber<Event>*>(base);
                    sub->receive(this, event);
                }
            }
        }

        template <typename Event>
        void broadcast(const Event& event);

        template <typename Result, typename Event>
        void eventResult(Result* res, EntityId id, const Event& event);
        
        template <typename Result, typename Event>
        void broadcastResult(Result* res, const Event& event);

        EventManager(Allocator alloc);
        ~EventManager();

    private:
        std::vector<Entity*, EntityPtrAllocator> mEntities;

        EntityAllocator mEntAlloc;

        std::unordered_map<TypeIndex
            , std::vector<BaseEventSubscriber*, SubscriberPtrAllocator>
            , std::hash<TypeIndex>
            , std::equal_to<TypeIndex>
            , SubscriberPairAllocator> mSubscribers;
    };
}
