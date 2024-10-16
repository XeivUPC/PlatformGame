#include "ColliderHandler.h"


ColliderHandler::ColliderHandler()
{

}

ColliderHandler::~ColliderHandler()
{
    bodiesColliding.clear();
}

void ColliderHandler::SetSensor(b2Fixture* bodyToTrack)
{
    this->bodyToTrack = bodyToTrack;
}

bool ColliderHandler::IsBeingTriggered()
{
    return bodiesInside!=0;
}

void ColliderHandler::AcceptOnlyTriggers(bool acceptOnlyTriggers)
{
    onlyTriggers = acceptOnlyTriggers;
}

bool ColliderHandler::OnTriggerEnter()
{
    if (lastBodyEnter != nullptr) {
        lastBodyEnter = nullptr;
        return true;
    }
    return false;
}

bool ColliderHandler::OnTriggerExit()
{
    if (lastBodyExit != nullptr) {
        lastBodyExit = nullptr;
        return true;
    }
    return false;
}

void ColliderHandler::BeginContact(b2Contact* contact)
{
    // Get the two fixtures involved in the contact
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    // Check if the tracked body is involved
    if (fixtureA == bodyToTrack || fixtureB == bodyToTrack) {
        // Determine which fixture is the sensor (if any)
        if (!onlyTriggers || fixtureA->IsSensor() || fixtureB->IsSensor()) {
            bodiesInside++;
            b2Body* bodyA = fixtureA->GetBody();
            b2Body* bodyB = fixtureB->GetBody();
            lastBodyEnter = GetDifferentBody(bodyA, bodyB, bodyToTrack->GetBody());
            bodiesColliding.emplace(lastBodyEnter);
        }
    }
}

void ColliderHandler::EndContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    // Check if the tracked body is involved
    if (fixtureA == bodyToTrack || fixtureB == bodyToTrack) {
        // Determine which fixture is the sensor (if any)
        if (!onlyTriggers || fixtureA->IsSensor() || fixtureB->IsSensor()) {
            bodiesInside--;
            b2Body* bodyA = fixtureA->GetBody();
            b2Body* bodyB = fixtureB->GetBody();
            lastBodyExit = GetDifferentBody(bodyA, bodyB, bodyToTrack->GetBody());
            bodiesColliding.erase(lastBodyEnter);
        }
    }
}

std::set<b2Body*> ColliderHandler::GetBodiesColliding()
{
    return bodiesColliding;
}

b2Body* ColliderHandler::GetDifferentBody(b2Body* body1, b2Body* body2, b2Body* bodyToBeDifferentFrom)
{
    if (body1 != bodyToBeDifferentFrom)
        return body1;
    return body2;
}


