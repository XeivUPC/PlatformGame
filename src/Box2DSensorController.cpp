#include "Box2DSensorController.h"

void Box2DSensorController::SetSensor(b2Fixture* bodyToTrack)
{
    this->bodyToTrack = bodyToTrack;
}

bool Box2DSensorController::IsBeingTriggered()
{
    return bodiesInside!=0;
}

void Box2DSensorController::AcceptOnlyTriggers(bool acceptOnlyTriggers)
{
    onlyTriggers = acceptOnlyTriggers;
}

bool Box2DSensorController::OnTriggerEnter()
{
    if (lastBodyEnter != nullptr) {
        lastBodyEnter = nullptr;
        return true;
    }
    return false;
}

bool Box2DSensorController::OnTriggerExit()
{
    if (lastBodyExit != nullptr) {
        lastBodyExit = nullptr;
        return true;
    }
    return false;
}

void Box2DSensorController::BeginContact(b2Contact* contact)
{
    // Get the two fixtures involved in the contact
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    // Get the bodies involved in the contact
    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    // Check if the tracked body is involved
    if (fixtureA == bodyToTrack || fixtureB == bodyToTrack) {
        // Determine which fixture is the sensor (if any)
        if (!onlyTriggers || fixtureA->IsSensor() || fixtureB->IsSensor()) {
            bodiesInside++;
            lastBodyEnter = GetDifferentBody(bodyA, bodyB, bodyToTrack->GetBody());
        }
    }
}

void Box2DSensorController::EndContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    // Check if the tracked body is involved
    if (fixtureA == bodyToTrack || fixtureB == bodyToTrack) {
        // Determine which fixture is the sensor (if any)
        if (!onlyTriggers || fixtureA->IsSensor() || fixtureB->IsSensor()) {
            bodiesInside--;
            lastBodyExit = GetDifferentBody(bodyA, bodyB, bodyToTrack->GetBody());
        }
    }
}

b2Body* Box2DSensorController::GetDifferentBody(b2Body* body1, b2Body* body2, b2Body* bodyToBeDifferentFrom)
{
    if (body1 != bodyToBeDifferentFrom)
        return body1;
    return body2;
}


