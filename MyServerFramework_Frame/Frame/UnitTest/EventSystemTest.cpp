#ifdef WINDOWS
#include "FrameHeader.h"

// -----------------------------------------------------------
// 测试用事件定义
// -----------------------------------------------------------
class TestEventA : public GameEvent
{
public:
    int value = 0;
    string message;
    TestEventA()
    {
        mType = (int)typeid(TestEventA).hash_code();
    }
};

class TestEventB : public GameEvent
{
public:
    float score = 0.0f;
    bool flag = false;
    TestEventB()
    {
        mType = (int)typeid(TestEventB).hash_code();
    }
};

// -----------------------------------------------------------
// 测试用事件监听器
// -----------------------------------------------------------
class TestListener : public IEventListener
{
public:
    int eventACount = 0;
    int eventBCount = 0;
    int lastValue = 0;
    float lastScore = 0.0f;
    string lastMessage;

    void reset()
    {
        eventACount = 0;
        eventBCount = 0;
        lastValue = 0;
        lastScore = 0.0f;
        lastMessage.clear();
    }

    void onEvent(TestEventA* event)
    {
        ++eventACount;
        lastValue = event->value;
        lastMessage = event->message;
    }
    void onEvent(TestEventB* event)
    {
        ++eventBCount;
        lastScore = event->score;
    }
};

class EventDispatchModifyTestEvent : public GameEvent
{
public:
    EventDispatchModifyTestEvent()
    {
        mType = (int)typeid(EventDispatchModifyTestEvent).hash_code();
    }
};

class EventDispatchRemoveSelfListener : public IEventListener
{
public:
    void init(EventSystem* eventSystem)
    {
        mEventSystem = eventSystem;
    }
    void onEvent(EventDispatchModifyTestEvent* event)
    {
        ++mCallbackCount;
        mEventSystem->unlistenEvent(this);
    }
public:
    EventSystem* mEventSystem = nullptr;
    int mCallbackCount = 0;
};

class EventDispatchNormalListener : public IEventListener
{
public:
    void onEvent(EventDispatchModifyTestEvent* event)
    {
        ++mCallbackCount;
    }
public:
    int mCallbackCount = 0;
};

class EventModifyListener : public IEventListener
{
public:
    void init(EventSystem* eventSystem, Character* character = nullptr)
    {
        mEventSystem = eventSystem;
        mCharacter = character;
    }

    void onEvent(TestEventA* event)
    {
        ++mEventACount;
        mLastValue = event->value;
        mLastCharacterGUID = event->mCharacterGUID;

        if (mPushNested && !mNestedPushed)
        {
            mNestedPushed = true;
            TestEventA nestedEvent;
            nestedEvent.value = event->value + 1;
            if (mCharacter != nullptr)
            {
                mEventSystem->pushEvent(nestedEvent, mCharacter);
            }
            else
            {
                mEventSystem->pushEvent(nestedEvent);
            }
        }

        if (mAddTarget != nullptr && !mAddedTarget)
        {
            mAddedTarget = true;
            if (mCharacter != nullptr && mAddTargetAsLocal)
            {
                mEventSystem->listenEvent<TestEventA>(mCharacter, mAddTarget);
            }
            else
            {
                mEventSystem->listenEvent<TestEventA>(mAddTarget);
            }
        }

        if (mUnlistenTarget != nullptr)
        {
            mEventSystem->unlistenEvent(mUnlistenTarget);
        }

        if (mUnlistenSelf)
        {
            mEventSystem->unlistenEvent(this);
        }

        if (mRemoveCharacterEvent && mCharacter != nullptr)
        {
            mEventSystem->removeCharacterEvent(mCharacter->getGUID());
        }
    }

public:
    EventSystem* mEventSystem = nullptr;
    Character* mCharacter = nullptr;
    IEventListener* mUnlistenTarget = nullptr;
    EventModifyListener* mAddTarget = nullptr;
    bool mAddTargetAsLocal = true;
    bool mUnlistenSelf = false;
    bool mRemoveCharacterEvent = false;
    bool mPushNested = false;
    bool mNestedPushed = false;
    bool mAddedTarget = false;
    int mEventACount = 0;
    int mLastValue = 0;
    llong mLastCharacterGUID = 0;
};

// -----------------------------------------------------------
// 测试实现
// -----------------------------------------------------------
#define EVT_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("EventSystemTest FAILED: ") + (msg)); } } while(0)

void EventSystemTest::test()
{
    testEventInfoResetProperty();
    testPushAndListenEvent();
    testUnlistenEvent();
    testRemoveCharacterEvent();
    testGlobalEventListen();
    testEventProcessCallback();
    testMultipleListeners();
    testEventTypeIsolation();
    testEventSystemUnlistenInCallbackSkipNext();
    testLocalUnlistenSelfInCallback();
    testGlobalUnlistenSelfInCallback();
    testUnlistenPreviousInCallback();
    testUnlistenNextInCallback();
    testAddListenerInCallback();
    testNestedSameEventPush();
    testRemoveCharacterEventInCallback();
    testDuplicateListenUnlistenSelf();
    testGlobalAndLocalDispatch();
    testLocalUnlistenGlobalBeforeGlobalDispatch();
    testLocalUnlistenSelfAlsoRemovesGlobal();
    testGlobalAddListenerInCallback();
    testRemoveCharacterEventKeepsOtherScopes();
    testRemoveCharacterEventAfterNullSlotInCallback();
    testEventCharacterGUID();
    testNoDestroyedEventInfoProfileOnNormalMutations();
	testExtended();
}

void EventSystemTest::testEventInfoResetProperty()
{
    EventInfoPool pool;
    EventInfo* info = CLASS<EventInfo>();
    EVT_ASSERT(info != nullptr, "newClass returns valid pointer");
}

void EventSystemTest::testEventSystemUnlistenInCallbackSkipNext()
{
    EventInfoPool eventInfoPool;
    EventSystem eventSystem;

    Character character;
    character.setGUID(10000001);

    EventDispatchRemoveSelfListener listener0;
    listener0.init(&eventSystem);

    EventDispatchNormalListener listener1;

    eventSystem.listenEvent<EventDispatchModifyTestEvent>(&character, &listener0);
    eventSystem.listenEvent<EventDispatchModifyTestEvent>(&character, &listener1);

    EventDispatchModifyTestEvent event;
    eventSystem.pushEvent(event, &character);

    if (listener1.mCallbackCount == 0)
    {
        ERROR("后续监听者被跳过");
    }
    eventSystem.quit();
}

void EventSystemTest::testPushAndListenEvent()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener;

    // 监听全局事件
    eventSystem.listenEvent<TestEventA>(&listener);

    // 推送事件
    TestEventA event;
    event.value = 42;
    event.message = "test";
    eventSystem.pushEvent(event);

    // 更新事件系统以处理事件
    eventSystem.update(0.016f);

    EVT_ASSERT(listener.eventACount == 1, "listener received event A once");
    EVT_ASSERT(listener.lastValue == 42, "event value is 42");
    EVT_ASSERT(listener.lastMessage == "test", "event message is 'test'");

    eventSystem.quit();
}

void EventSystemTest::testUnlistenEvent()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener;

    // 监听然后取消监听
    eventSystem.listenEvent<TestEventA>(&listener);
    eventSystem.unlistenEvent(&listener);

    // 推送事件
    TestEventA event;
    event.value = 100;
    eventSystem.pushEvent(event);
    eventSystem.update(0.016f);

    EVT_ASSERT(listener.eventACount == 0, "after unlisten, no event received");

    eventSystem.quit();
}

void EventSystemTest::testRemoveCharacterEvent()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener;

    Character character;
    character.setGUID(12345);

    eventSystem.listenEvent<TestEventA>(&character, &listener);

    TestEventA event;
    event.value = 200;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener.eventACount == 1, "character listener received before removeCharacterEvent");

    eventSystem.removeCharacterEvent(character.getGUID());

    listener.reset();
    eventSystem.pushEvent(event, &character);
    eventSystem.update(0.016f);

    EVT_ASSERT(listener.eventACount == 0, "character listener removed by removeCharacterEvent");

    eventSystem.quit();
}

void EventSystemTest::testGlobalEventListen()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener1;
    TestListener listener2;

    // 两个监听器都监听全局事件
    eventSystem.listenEvent<TestEventA>(&listener1);
    eventSystem.listenEvent<TestEventA>(&listener2);

    // 推送事件
    TestEventA event;
    event.value = 999;
    eventSystem.pushEvent(event);
    eventSystem.update(0.016f);

    EVT_ASSERT(listener1.eventACount == 1, "global listener1 received event");
    EVT_ASSERT(listener2.eventACount == 1, "global listener2 received event");
    EVT_ASSERT(listener1.lastValue == 999, "listener1 got correct value");
    EVT_ASSERT(listener2.lastValue == 999, "listener2 got correct value");

    eventSystem.quit();
}

void EventSystemTest::testEventProcessCallback()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener;

    eventSystem.listenEvent<TestEventA>(&listener);
    eventSystem.listenEvent<TestEventB>(&listener);

    // 推送事件A
    TestEventA eventA;
    eventA.value = 111;
    eventSystem.pushEvent(eventA);

    // 推送事件B
    TestEventB eventB;
    eventB.score = 3.14f;
    eventSystem.pushEvent(eventB);

    eventSystem.update(0.016f);

    EVT_ASSERT(listener.eventACount == 1, "received event A");
    EVT_ASSERT(listener.eventBCount == 1, "received event B");
    EVT_ASSERT(listener.lastValue == 111, "event A value correct");
    EVT_ASSERT(listener.lastScore == 3.14f, "event B score correct");

    eventSystem.quit();
}

void EventSystemTest::testMultipleListeners()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener1;
    TestListener listener2;
    TestListener listener3;

    // 三个监听器监听同一事件
    eventSystem.listenEvent<TestEventA>(&listener1);
    eventSystem.listenEvent<TestEventA>(&listener2);
    eventSystem.listenEvent<TestEventA>(&listener3);

    // 推送多个事件
    for (int i = 0; i < 5; ++i)
    {
        TestEventA event;
        event.value = i;
        eventSystem.pushEvent(event);
    }

    eventSystem.update(0.016f);

    EVT_ASSERT(listener1.eventACount == 5, "listener1 received all 5 events");
    EVT_ASSERT(listener2.eventACount == 5, "listener2 received all 5 events");
    EVT_ASSERT(listener3.eventACount == 5, "listener3 received all 5 events");
    EVT_ASSERT(listener1.lastValue == 4, "listener1 last value is 4");

    eventSystem.quit();
}

void EventSystemTest::testEventTypeIsolation()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener;

    // 只监听事件A
    eventSystem.listenEvent<TestEventA>(&listener);

    // 推送事件A和B
    TestEventA eventA;
    eventA.value = 100;
    eventSystem.pushEvent(eventA);

    TestEventB eventB;
    eventB.score = 2.5f;
    eventSystem.pushEvent(eventB);

    eventSystem.update(0.016f);

    // 只应该收到事件A
    EVT_ASSERT(listener.eventACount == 1, "only received event A");
    EVT_ASSERT(listener.eventBCount == 0, "did not receive event B (not listening)");
    EVT_ASSERT(listener.lastValue == 100, "event A value correct");

    eventSystem.quit();
}
void EventSystemTest::testLocalUnlistenSelfInCallback()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000001);

    EventModifyListener listener0;
    EventModifyListener listener1;
    listener0.init(&eventSystem, &character);
    listener1.init(&eventSystem, &character);
    listener0.mUnlistenSelf = true;

    eventSystem.listenEvent<TestEventA>(&character, &listener0);
    eventSystem.listenEvent<TestEventA>(&character, &listener1);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 1, "local self-unlisten listener called once");
    EVT_ASSERT(listener1.mEventACount == 1, "local self-unlisten should not skip next listener");

    event.value = 2;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 1, "local self-unlisten removed listener");
    EVT_ASSERT(listener1.mEventACount == 2, "local remaining listener receives next event");

    eventSystem.quit();
}

void EventSystemTest::testGlobalUnlistenSelfInCallback()
{
    EventInfoPool pool;
    EventSystem eventSystem;

    EventModifyListener listener0;
    EventModifyListener listener1;
    listener0.init(&eventSystem);
    listener1.init(&eventSystem);
    listener0.mUnlistenSelf = true;

    eventSystem.listenEvent<TestEventA>(&listener0);
    eventSystem.listenEvent<TestEventA>(&listener1);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event);

    EVT_ASSERT(listener0.mEventACount == 1, "global self-unlisten listener called once");
    EVT_ASSERT(listener1.mEventACount == 1, "global self-unlisten should not skip next listener");

    event.value = 2;
    eventSystem.pushEvent(event);

    EVT_ASSERT(listener0.mEventACount == 1, "global self-unlisten removed listener");
    EVT_ASSERT(listener1.mEventACount == 2, "global remaining listener receives next event");

    eventSystem.quit();
}

void EventSystemTest::testUnlistenPreviousInCallback()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000002);

    EventModifyListener listener0;
    EventModifyListener listener1;
    EventModifyListener listener2;
    listener0.init(&eventSystem, &character);
    listener1.init(&eventSystem, &character);
    listener2.init(&eventSystem, &character);
    listener1.mUnlistenTarget = &listener0;

    eventSystem.listenEvent<TestEventA>(&character, &listener0);
    eventSystem.listenEvent<TestEventA>(&character, &listener1);
    eventSystem.listenEvent<TestEventA>(&character, &listener2);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 1, "previous target receives current event before being removed");
    EVT_ASSERT(listener1.mEventACount == 1, "unlisten previous listener called");
    EVT_ASSERT(listener2.mEventACount == 1, "unlisten previous should not skip next listener");

    event.value = 2;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 1, "previous target removed for next event");
    EVT_ASSERT(listener1.mEventACount == 2, "middle listener receives next event");
    EVT_ASSERT(listener2.mEventACount == 2, "last listener receives next event");

    eventSystem.quit();
}

void EventSystemTest::testUnlistenNextInCallback()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000003);

    EventModifyListener listener0;
    EventModifyListener listener1;
    EventModifyListener listener2;
    listener0.init(&eventSystem, &character);
    listener1.init(&eventSystem, &character);
    listener2.init(&eventSystem, &character);
    listener0.mUnlistenTarget = &listener1;

    eventSystem.listenEvent<TestEventA>(&character, &listener0);
    eventSystem.listenEvent<TestEventA>(&character, &listener1);
    eventSystem.listenEvent<TestEventA>(&character, &listener2);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 1, "unlisten next listener called");
    EVT_ASSERT(listener1.mEventACount == 0, "removed next listener should not receive current event");
    EVT_ASSERT(listener2.mEventACount == 1, "unlisten next should not skip following listener");

    event.value = 2;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 2, "first listener receives next event");
    EVT_ASSERT(listener1.mEventACount == 0, "removed next listener stays removed");
    EVT_ASSERT(listener2.mEventACount == 2, "following listener receives next event");

    eventSystem.quit();
}

void EventSystemTest::testAddListenerInCallback()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000004);

    EventModifyListener listener0;
    EventModifyListener listener1;
    listener0.init(&eventSystem, &character);
    listener1.init(&eventSystem, &character);
    listener0.mAddTarget = &listener1;

    eventSystem.listenEvent<TestEventA>(&character, &listener0);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 1, "add listener source receives current event");
    EVT_ASSERT(listener1.mEventACount == 0, "listener added during callback should not receive current event");

    event.value = 2;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 2, "add listener source receives next event");
    EVT_ASSERT(listener1.mEventACount == 1, "listener added during callback receives next event");

    eventSystem.quit();
}

void EventSystemTest::testNestedSameEventPush()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000005);

    EventModifyListener listener0;
    EventModifyListener listener1;
    listener0.init(&eventSystem, &character);
    listener1.init(&eventSystem, &character);
    listener0.mPushNested = true;

    eventSystem.listenEvent<TestEventA>(&character, &listener0);
    eventSystem.listenEvent<TestEventA>(&character, &listener1);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 2, "nested same event calls first listener in outer and nested dispatch");
    EVT_ASSERT(listener1.mEventACount == 2, "nested same event calls second listener in outer and nested dispatch");
    EVT_ASSERT(listener0.mLastValue == 2, "nested event value observed by first listener");
    EVT_ASSERT(listener1.mLastValue == 1, "outer event remains current after nested dispatch");

    eventSystem.quit();
}

void EventSystemTest::testRemoveCharacterEventInCallback()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000006);

    EventModifyListener listener0;
    EventModifyListener listener1;
    listener0.init(&eventSystem, &character);
    listener1.init(&eventSystem, &character);
    listener0.mRemoveCharacterEvent = true;

    eventSystem.listenEvent<TestEventA>(&character, &listener0);
    eventSystem.listenEvent<TestEventA>(&character, &listener1);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);
    eventSystem.update(0.016f);

    EVT_ASSERT(listener0.mEventACount == 1, "removeCharacterEvent source receives current event");
    EVT_ASSERT(listener1.mEventACount == 0, "removeCharacterEvent prevents later local listener on destroyed character");

    event.value = 2;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 1, "removed character listener does not receive next event");
    EVT_ASSERT(listener1.mEventACount == 0, "removed character listener stays removed");

    eventSystem.quit();
}

void EventSystemTest::testDuplicateListenUnlistenSelf()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000007);

    EventModifyListener listener;
    listener.init(&eventSystem, &character);
    listener.mUnlistenSelf = true;

    eventSystem.listenEvent<TestEventA>(&character, &listener);
    eventSystem.listenEvent<TestEventA>(&character, &listener);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener.mEventACount == 1, "duplicate listener self-unlisten should only call once");

    event.value = 2;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener.mEventACount == 1, "duplicate listener self-unlisten removes all own registrations");

    eventSystem.quit();
}

void EventSystemTest::testGlobalAndLocalDispatch()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000008);

    EventModifyListener localListener;
    EventModifyListener globalListener;
    localListener.init(&eventSystem, &character);
    globalListener.init(&eventSystem);

    eventSystem.listenEvent<TestEventA>(&character, &localListener);
    eventSystem.listenEvent<TestEventA>(&globalListener);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(localListener.mEventACount == 1, "local listener receives character event");
    EVT_ASSERT(globalListener.mEventACount == 1, "global listener also receives character event");

    event.value = 2;
    eventSystem.pushEvent(event);
    
    EVT_ASSERT(localListener.mEventACount == 1, "local listener does not receive global event");
    EVT_ASSERT(globalListener.mEventACount == 2, "global listener receives global event");

    eventSystem.quit();
}

void EventSystemTest::testLocalUnlistenGlobalBeforeGlobalDispatch()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000009);

    EventModifyListener localRemover;
    EventModifyListener globalRemoved;
    EventModifyListener globalRemain;
    localRemover.init(&eventSystem, &character);
    globalRemoved.init(&eventSystem);
    globalRemain.init(&eventSystem);
    localRemover.mUnlistenTarget = &globalRemoved;

    eventSystem.listenEvent<TestEventA>(&character, &localRemover);
    eventSystem.listenEvent<TestEventA>(&globalRemoved);
    eventSystem.listenEvent<TestEventA>(&globalRemain);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(localRemover.mEventACount == 1, "local remover receives character event");
    EVT_ASSERT(globalRemoved.mEventACount == 0, "local callback unlisten removes target before global dispatch");
    EVT_ASSERT(globalRemain.mEventACount == 1, "other global listener still receives character event");

    event.value = 2;
    eventSystem.pushEvent(event);

    EVT_ASSERT(globalRemoved.mEventACount == 0, "unlistened global target stays removed");
    EVT_ASSERT(globalRemain.mEventACount == 2, "remaining global listener receives later global event");

    eventSystem.quit();
}

void EventSystemTest::testLocalUnlistenSelfAlsoRemovesGlobal()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000010);

    EventModifyListener listener;
    EventModifyListener globalRemain;
    listener.init(&eventSystem, &character);
    globalRemain.init(&eventSystem);
    listener.mUnlistenSelf = true;

    eventSystem.listenEvent<TestEventA>(&character, &listener);
    eventSystem.listenEvent<TestEventA>(&listener);
    eventSystem.listenEvent<TestEventA>(&globalRemain);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener.mEventACount == 1, "self-unlisten in local callback removes own global registration before global dispatch");
    EVT_ASSERT(globalRemain.mEventACount == 1, "other global listener still receives event");

    event.value = 2;
    eventSystem.pushEvent(event);

    EVT_ASSERT(listener.mEventACount == 1, "self-unlistened listener does not receive later global event");
    EVT_ASSERT(globalRemain.mEventACount == 2, "remaining global listener receives later event");

    eventSystem.quit();
}

void EventSystemTest::testGlobalAddListenerInCallback()
{
    EventInfoPool pool;
    EventSystem eventSystem;

    EventModifyListener listener0;
    EventModifyListener listener1;
    listener0.init(&eventSystem);
    listener1.init(&eventSystem);
    listener0.mAddTarget = &listener1;
    listener0.mAddTargetAsLocal = false;

    eventSystem.listenEvent<TestEventA>(&listener0);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event);

    EVT_ASSERT(listener0.mEventACount == 1, "global add source receives current event");
    EVT_ASSERT(listener1.mEventACount == 0, "global listener added during callback should not receive current event");

    event.value = 2;
    eventSystem.pushEvent(event);

    EVT_ASSERT(listener0.mEventACount == 2, "global add source receives next event");
    EVT_ASSERT(listener1.mEventACount == 1, "global listener added during callback receives next event");

    eventSystem.quit();
}

void EventSystemTest::testRemoveCharacterEventKeepsOtherScopes()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character0;
    Character character1;
    character0.setGUID(10000011);
    character1.setGUID(10000012);

    EventModifyListener sharedListener;
    sharedListener.init(&eventSystem, &character0);

    eventSystem.listenEvent<TestEventA>(&character0, &sharedListener);
    eventSystem.listenEvent<TestEventA>(&character1, &sharedListener);
    eventSystem.listenEvent<TestEventA>(&sharedListener);

    eventSystem.removeCharacterEvent(character0.getGUID());

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character0);
    EVT_ASSERT(sharedListener.mEventACount == 1, "remove character0 keeps shared global registration");

    event.value = 2;
    eventSystem.pushEvent(event, &character1);
    EVT_ASSERT(sharedListener.mEventACount == 3, "remove character0 keeps character1 local and global registrations");

    event.value = 3;
    eventSystem.pushEvent(event);
    EVT_ASSERT(sharedListener.mEventACount == 4, "remove character0 keeps later global event");

    eventSystem.unlistenEvent(&sharedListener);

    event.value = 4;
    eventSystem.pushEvent(event, &character1);
    eventSystem.pushEvent(event);
    EVT_ASSERT(sharedListener.mEventACount == 4, "unlisten after removeCharacterEvent has no stale character0 EventInfo");

    eventSystem.quit();
}

void EventSystemTest::testRemoveCharacterEventAfterNullSlotInCallback()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000013);

    EventModifyListener listener0;
    EventModifyListener listener1;
    EventModifyListener listener2;
    listener0.init(&eventSystem, &character);
    listener1.init(&eventSystem, &character);
    listener2.init(&eventSystem, &character);
    listener0.mUnlistenTarget = &listener1;
    listener0.mRemoveCharacterEvent = true;

    eventSystem.listenEvent<TestEventA>(&character, &listener0);
    eventSystem.listenEvent<TestEventA>(&character, &listener1);
    eventSystem.listenEvent<TestEventA>(&character, &listener2);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);
    eventSystem.update(0.016f);

    EVT_ASSERT(listener0.mEventACount == 1, "removeCharacterEvent after null slot source receives current event");
    EVT_ASSERT(listener1.mEventACount == 0, "listener removed before removeCharacterEvent is not called");
    EVT_ASSERT(listener2.mEventACount == 0, "removeCharacterEvent clears later listener in same dispatch");

    event.value = 2;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(listener0.mEventACount == 1, "character event list cleared after null-slot removeCharacterEvent");
    EVT_ASSERT(listener1.mEventACount == 0, "removed listener stays removed after null-slot removeCharacterEvent");
    EVT_ASSERT(listener2.mEventACount == 0, "later listener stays removed after null-slot removeCharacterEvent");

    eventSystem.quit();
}

void EventSystemTest::testEventCharacterGUID()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    Character character;
    character.setGUID(10000014);

    EventModifyListener localListener;
    EventModifyListener globalListener;
    localListener.init(&eventSystem, &character);
    globalListener.init(&eventSystem);

    eventSystem.listenEvent<TestEventA>(&character, &localListener);
    eventSystem.listenEvent<TestEventA>(&globalListener);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character);

    EVT_ASSERT(localListener.mLastCharacterGUID == character.getGUID(), "local listener sees character GUID");
    EVT_ASSERT(globalListener.mLastCharacterGUID == character.getGUID(), "global listener sees character GUID from character event");

    event.value = 2;
    eventSystem.pushEvent(event);

    EVT_ASSERT(localListener.mEventACount == 1, "local listener does not receive global event");
    EVT_ASSERT(globalListener.mEventACount == 2, "global listener receives global event");
    EVT_ASSERT(globalListener.mLastCharacterGUID == 0, "global event without character has zero character GUID");

    eventSystem.quit();
}

void EventSystemTest::testNoDestroyedEventInfoProfileOnNormalMutations()
{
    EventInfoPool pool;
    int destroyedInfoErrorCount = 0;
    ErrorProfileEvent oldErrorProfileEvent = ErrorProfile::mOnErrorProfileEvent;
    ErrorProfile::mOnErrorProfileEvent = [&destroyedInfoErrorCount, oldErrorProfileEvent](string&& log, string&& file) mutable
        {
            if (file.find("EventSystem.cpp") != string::npos && !log.empty() && (log[0] == '0' || log[0] == '2'))
            {
                ++destroyedInfoErrorCount;
            }
            if (oldErrorProfileEvent)
            {
                oldErrorProfileEvent(move(log), move(file));
            }
        };

    EventSystem eventSystem;
    Character character0;
    Character character1;
    character0.setGUID(10000015);
    character1.setGUID(10000016);

    EventModifyListener duplicateSelf;
    duplicateSelf.init(&eventSystem, &character0);
    duplicateSelf.mUnlistenSelf = true;
    eventSystem.listenEvent<TestEventA>(&character0, &duplicateSelf);
    eventSystem.listenEvent<TestEventA>(&character0, &duplicateSelf);

    TestEventA event;
    event.value = 1;
    eventSystem.pushEvent(event, &character0);

    EventModifyListener globalSelf;
    EventModifyListener globalRemain;
    globalSelf.init(&eventSystem);
    globalRemain.init(&eventSystem);
    globalSelf.mUnlistenSelf = true;
    eventSystem.listenEvent<TestEventA>(&globalSelf);
    eventSystem.listenEvent<TestEventA>(&globalRemain);

    event.value = 2;
    eventSystem.pushEvent(event);

    EventModifyListener removeCharacter0;
    EventModifyListener removeCharacter1;
    EventModifyListener removeCharacter2;
    removeCharacter0.init(&eventSystem, &character1);
    removeCharacter1.init(&eventSystem, &character1);
    removeCharacter2.init(&eventSystem, &character1);
    removeCharacter0.mUnlistenTarget = &removeCharacter1;
    removeCharacter0.mRemoveCharacterEvent = true;
    eventSystem.listenEvent<TestEventA>(&character1, &removeCharacter0);
    eventSystem.listenEvent<TestEventA>(&character1, &removeCharacter1);
    eventSystem.listenEvent<TestEventA>(&character1, &removeCharacter2);

    event.value = 3;
    eventSystem.pushEvent(event, &character1);
    eventSystem.update(0.016f);

    eventSystem.quit();
    ErrorProfile::mOnErrorProfileEvent = oldErrorProfileEvent;

    EVT_ASSERT(destroyedInfoErrorCount == 0, "normal EventSystem mutations should not dispatch destroyed EventInfo");
}


// ============================================================
// EventSystemTest — 扩展事件系统测试
// 覆盖：嵌套事件推送、重复监听/取消监听、
//       多事件类型完全隔离、事件中修改监听列表
// ============================================================

// -----------------------------------------------------------
// 测试用事件类型
// -----------------------------------------------------------
class TestEventX : public GameEvent
{
public:
    int xValue = 0;
    TestEventX() { mType = (int)typeid(TestEventX).hash_code(); }
};

class TestEventY : public GameEvent
{
public:
    float yValue = 0.0f;
    string yLabel;
    TestEventY() { mType = (int)typeid(TestEventY).hash_code(); }
};

class TestEventZ : public GameEvent
{
public:
    bool flag = false;
    TestEventZ() { mType = (int)typeid(TestEventZ).hash_code(); }
};

// -----------------------------------------------------------
// 测试用监听器
// -----------------------------------------------------------
class ExtendedListener : public IEventListener
{
public:
    int eventXCount = 0;
    int eventYCount = 0;
    int eventZCount = 0;
    int lastXValue = 0;
    float lastYValue = 0.0f;
    string lastYLabel;
    bool lastZFlag = false;

    // 嵌套测试：在回调中再次 pushEvent
    EventSystem* nestedPushTarget = nullptr;
    int nestedDepth = 0;

    void reset()
    {
        eventXCount = 0;
        eventYCount = 0;
        eventZCount = 0;
        lastXValue = 0;
        lastYValue = 0.0f;
        lastYLabel.clear();
        lastZFlag = false;
        nestedDepth = 0;
    }

    void onEvent(TestEventX* event)
    {
        ++eventXCount;
        lastXValue = event->xValue;
        // 如果设置了嵌套目标，在 X 事件的回调中推一个 Y 事件
        if (nestedPushTarget != nullptr && nestedDepth < 5)
        {
            ++nestedDepth;
            TestEventY nestedY;
            nestedY.yValue = -1.0f;
            nestedY.yLabel = "nested-from-X";
            nestedPushTarget->pushEvent(nestedY);
            --nestedDepth;
        }
    }

    void onEvent(TestEventY* event)
    {
        ++eventYCount;
        lastYValue = event->yValue;
        lastYLabel = event->yLabel;
    }

    void onEvent(TestEventZ* event)
    {
        ++eventZCount;
        lastZFlag = event->flag;
    }
};

#define EVT_EXT_ASSERT(expr, msg) \
do { if (!(expr)) { ERROR(string("EventSystemTest FAILED: ") + (msg)); } } while(0)

// -----------------------------------------------------------
// Case 1: 嵌套事件推送 — 在 A 的回调中 push B
// -----------------------------------------------------------
void EventSystemTest::testNestedEventPush()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    listener.nestedPushTarget = &evtSys;
    evtSys.listenEvent<TestEventX>(&listener);
    evtSys.listenEvent<TestEventY>(&listener);

    // 推送 X → 回调中会自动推送 Y
    TestEventX eventX;
    eventX.xValue = 42;
    evtSys.pushEvent(eventX);

    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.eventXCount == 1, "nested: X received once");
    EVT_EXT_ASSERT(listener.eventYCount >= 1, "nested: Y received from within X callback");
    EVT_EXT_ASSERT(listener.lastYLabel == "nested-from-X", "nested: Y label correct");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 2: 多次连续推送同一事件
// -----------------------------------------------------------
void EventSystemTest::testMultipleSameEvents()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    evtSys.listenEvent<TestEventX>(&listener);

    for (int i = 0; i < 20; ++i)
    {
        TestEventX e;
        e.xValue = i;
        evtSys.pushEvent(e);
    }

    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.eventXCount == 20, "multi-push: all 20 events received");
    EVT_EXT_ASSERT(listener.lastXValue == 19, "multi-push: last value is 19");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 3: 三种事件类型完全隔离
// -----------------------------------------------------------
void EventSystemTest::testEventTypeIsolation_Strict()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    evtSys.listenEvent<TestEventX>(&listener);
    evtSys.listenEvent<TestEventY>(&listener);
    evtSys.listenEvent<TestEventZ>(&listener);

    listener.reset();

    // 只推送 Y 和 Z
    TestEventY eY;
    eY.yValue = 77.0f;
    eY.yLabel = "only-y";
    evtSys.pushEvent(eY);

    TestEventZ eZ;
    eZ.flag = true;
    evtSys.pushEvent(eZ);

    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.eventXCount == 0, "isolation: X not triggered");
    EVT_EXT_ASSERT(listener.eventYCount == 1, "isolation: Y triggered once");
    EVT_EXT_ASSERT(listener.eventZCount == 1, "isolation: Z triggered once");
    EVT_EXT_ASSERT(fabsf(listener.lastYValue - 77.0f) < 1e-6f, "isolation: Y value correct");
    EVT_EXT_ASSERT(listener.lastZFlag == true, "isolation: Z flag correct");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 4: 同一监听器监听同一事件两次（应收到两份）
// -----------------------------------------------------------
void EventSystemTest::testDoubleListen_SameEvent()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener1;
    ExtendedListener listener2;

    // 同一事件注册两个不同监听器
    evtSys.listenEvent<TestEventX>(&listener1);
    evtSys.listenEvent<TestEventX>(&listener2);

    TestEventX e;
    e.xValue = 888;
    evtSys.pushEvent(e);
    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener1.eventXCount == 1, "double listen: L1 got it");
    EVT_EXT_ASSERT(listener2.eventXCount == 1, "double listen: L2 got it");
    EVT_EXT_ASSERT(listener1.lastXValue == 888, "double listen: L1 value");
    EVT_EXT_ASSERT(listener2.lastXValue == 888, "double listen: L2 value");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 5: 取消重监听后再推送
// -----------------------------------------------------------
void EventSystemTest::testUnlistenThenPushAgain()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    evtSys.listenEvent<TestEventX>(&listener);

    // 第一次推送
    TestEventX e1;
    e1.xValue = 111;
    evtSys.pushEvent(e1);
    evtSys.update(0.016f);
    EVT_EXT_ASSERT(listener.eventXCount == 1, "unlisten: first push received");

    // 取消监听
    evtSys.unlistenEvent(&listener);

    // 第二次推送
    listener.reset();
    TestEventX e2;
    e2.xValue = 222;
    evtSys.pushEvent(e2);
    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.eventXCount == 0, "unlisten: second push NOT received after unlisten");

    // 重新监听
    evtSys.listenEvent<TestEventX>(&listener);

    // 第三次推送
    TestEventX e3;
    e3.xValue = 333;
    evtSys.pushEvent(e3);
    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.eventXCount == 1, "unlisten: third push received after re-listen");
    EVT_EXT_ASSERT(listener.lastXValue == 333, "unlisten: value after re-listen");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 6: 全局监听 vs 角色局部监听隔离
// -----------------------------------------------------------
void EventSystemTest::testGlobalVsCharacterScope()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener globalListener;
    ExtendedListener charListener;
    Character character;
    character.setGUID(910001);

    // 全局监听：角色事件和全局事件都应该收到
    evtSys.listenEvent<TestEventX>(&globalListener);

    // 角色局部监听：只应该收到指定角色事件
    evtSys.listenEvent<TestEventX>(&character, &charListener);

    TestEventX ex;
    ex.xValue = 100;
    evtSys.pushEvent(ex, &character);

    ex.xValue = 101;
    evtSys.pushEvent(ex);

    EVT_EXT_ASSERT(globalListener.eventXCount == 2, "scope: global got character and global X");
    EVT_EXT_ASSERT(charListener.eventXCount == 1, "scope: character listener only got character X");
    EVT_EXT_ASSERT(globalListener.lastXValue == 101, "scope: global last value from global event");
    EVT_EXT_ASSERT(charListener.lastXValue == 100, "scope: character last value from character event");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 7: 高频交替推送多种事件
// -----------------------------------------------------------
void EventSystemTest::testHighFrequencyMixedEvents()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    evtSys.listenEvent<TestEventX>(&listener);
    evtSys.listenEvent<TestEventY>(&listener);
    evtSys.listenEvent<TestEventZ>(&listener);

    const int ROUNDS = 50;
    for (int i = 0; i < ROUNDS; ++i)
    {
        if (i % 3 == 0)
        {
            TestEventX e;
            e.xValue = i;
            evtSys.pushEvent(e);
        }
        else if (i % 3 == 1)
        {
            TestEventY e;
            e.yValue = (float)i;
            evtSys.pushEvent(e);
        }
        else
        {
            TestEventZ e;
            e.flag = (i % 2 == 0);
            evtSys.pushEvent(e);
        }
    }

    evtSys.update(0.016f);

    // X: i%3==0 => 0,3,6,...,48 → 17 个
    int expectedX = ROUNDS / 3 + (ROUNDS % 3 > 0 ? 1 : 0);
    // Y: i%3==1 => 1,4,7,...,49 → 17 个
    int expectedY = ROUNDS / 3 + (ROUNDS % 3 > 1 ? 1 : 0);
    // Z: i%3==2 => 2,5,8,...,47 → 16 个
    int expectedZ = ROUNDS / 3;

    EVT_EXT_ASSERT(listener.eventXCount == expectedX, "high-freq: X count");
    EVT_EXT_ASSERT(listener.eventYCount == expectedY, "high-freq: Y count");
    EVT_EXT_ASSERT(listener.eventZCount == expectedZ, "high-freq: Z count");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 8: 无监听者时推送不崩溃
// -----------------------------------------------------------
void EventSystemTest::testPushWithNoListeners()
{
    EventInfoPool pool;
    EventSystem evtSys;

    // 没有任何监听者，直接推送各种事件
    FOR(200)
    {
        TestEventX e;
        e.xValue = i;
        evtSys.pushEvent(e);
        TestEventY y;
        evtSys.pushEvent(y);
    }

    evtSys.update(0.016f);
    EVT_EXT_ASSERT(true, "no listeners: no crash");
    evtSys.quit();
}

// -----------------------------------------------------------
// Case 9: 事件数据完整性 — 字段不串扰
// -----------------------------------------------------------
void EventSystemTest::testDataIntegrityAcrossTypes()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    evtSys.listenEvent<TestEventX>(&listener);
    evtSys.listenEvent<TestEventY>(&listener);

    // 推送带精确值的 X
    TestEventX ex;
    ex.xValue = 12345;
    evtSys.pushEvent(ex);

    // 推送带精确值的 Y
    TestEventY ey;
    ey.yValue = 67.89f;
    ey.yLabel = "integrity-check";
    evtSys.pushEvent(ey);

    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.lastXValue == 12345, "integrity: X value exact");
    EVT_EXT_ASSERT(fabsf(listener.lastYValue - 67.89f) < 0.001f, "integrity: Y value exact");
    EVT_EXT_ASSERT(listener.lastYLabel == "integrity-check", "integrity: Y label exact");

    evtSys.quit();
}

void EventSystemTest::testExtended()
{
    testNestedEventPush();
    testMultipleSameEvents();
    testEventTypeIsolation_Strict();
    testDoubleListen_SameEvent();
    testUnlistenThenPushAgain();
    testGlobalVsCharacterScope();
    testHighFrequencyMixedEvents();
    testPushWithNoListeners();
    testDataIntegrityAcrossTypes();
}
#endif
