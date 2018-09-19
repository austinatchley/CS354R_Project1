#pragma once

#include <Ogre.h>

#include "EntityManager.h"
#include "ComponentManager.h"

#include <unordered_map>
#include <functional>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>

namespace ECS
{
    class EventManager;
    class BaseEventSubscriber;
    class BaseEvent;

    typedef std::allocator<Entity> Allocator;
    using TypeIndex = std::type_index;

    using SubscriberPtrAllocator = std::allocator_traits<Allocator>::template rebind_alloc<BaseEventSubscriber*>; 
	using EntityPtrAllocator = std::allocator_traits<Allocator>::template rebind_alloc<Entity*>;

    using SubscriberPairAllocator = std::allocator_traits<Allocator>::template rebind_alloc<std::pair<const TypeIndex, std::vector<BaseEventSubscriber*, SubscriberPtrAllocator>>>;
	using EntityAllocator = std::allocator_traits<Allocator>::template rebind_alloc<Entity>;
    using SubscribersMap = std::unordered_map<TypeIndex
            , std::vector<BaseEventSubscriber*, SubscriberPtrAllocator>
            , std::hash<TypeIndex>
            , std::equal_to<TypeIndex>
            , SubscriberPairAllocator>;
    using EventQueue = std::unordered_map<BaseEventSubscriber*, unsigned>;

    template<typename T>
    TypeIndex getTypeIndex()
    {
        return std::type_index(typeid(T));
    }

    /////////////////////////////////////////////////////////////////
    class BaseEvent
    {
    public:
        typedef std::size_t Family;
        
        virtual ~BaseEvent();

    protected:
        static Family mFamilyCounter;
    };

	/*typedef Simple::Signal<void (const void*)> EventSignal;
    typedef std::shared_ptr<EventSignal> EventSignalPtr;
    typedef std::weak_ptr<EventSignal> EventSignalWeakPtr;
    */
    template <typename Derived>
    class Event
        : public BaseEvent
    {
    public:
        static Family family()
        {
            static Family family = mFamilyCounter++;
            return family;
        }
    };

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

        // After the data structure for subscribers is modified, this should take an ID
        // so that we can subscribe to multiple channels for the same event
        template <typename Event>
        void event(const Event& event)
        {
            auto it = mSubscribers.find(getTypeIndex<Event>());

            if (it != mSubscribers.end())
            {
                const auto& subscribers = it->second;
                for (BaseEventSubscriber* base : subscribers)
                {
                    auto* sub = reinterpret_cast<EventSubscriber<Event>*>(base);
                    const auto boundFunc = std::bind(&EventSubscriber<Event>::receive, sub, this, event);
                    mEvents.push_back(std::function<void ()>(boundFunc));
                }
            }
        }

        template <typename Event>
        void broadcast(Event& event)
        {
        }

        template <typename Result, typename Event>
        void eventResult(Result* res, EntityId id, const Event& event);
        
        template <typename Result, typename Event>
        void broadcastResult(Result* res, const Event& event);

        template <typename Event>
        void sendEvent(EventManager* eventManager, const Event& event, BaseEventSubscriber* base)
        {
            //auto* sub = reinterpret_cast<EventSubscriber<Event>*>(base);
            
            //sub->receive(eventManager, event);
        }

        EventManager(Allocator alloc);
        ~EventManager();

    private:
        std::vector<Entity*, EntityPtrAllocator> mEntities;

        EntityAllocator mEntAlloc;
        
        SubscribersMap mSubscribers;
        
        EventQueue mEventQueue;
        std::vector<std::function<void ()>> mEvents;
    };
}
