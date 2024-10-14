#include "CollidersManager.h"
#include "ColliderHandler.h"

CollidersManager::CollidersManager()
{

}

CollidersManager::~CollidersManager()
{
   
}

void CollidersManager::BeginContact(b2Contact* contact)
{

    b2FixtureUserData dataA = contact->GetFixtureA()->GetUserData();
    ColliderHandler* sensorA = (ColliderHandler*)dataA.pointer;
    if (sensorA != nullptr)
        sensorA->BeginContact(contact);

    b2FixtureUserData dataB = contact->GetFixtureB()->GetUserData();
    ColliderHandler* sensorB = (ColliderHandler*)dataB.pointer;
    if (sensorB != nullptr)
        sensorB->BeginContact(contact);

}

void CollidersManager::EndContact(b2Contact* contact)
{

    b2FixtureUserData dataA = contact->GetFixtureA()->GetUserData();
    ColliderHandler* sensorA = (ColliderHandler*)dataA.pointer;
    if (sensorA != nullptr)
        sensorA->EndContact(contact);

    b2FixtureUserData dataB = contact->GetFixtureB()->GetUserData();
    ColliderHandler* sensorB = (ColliderHandler*)dataB.pointer;
    if (sensorB != nullptr)
        sensorB->EndContact(contact);
}
