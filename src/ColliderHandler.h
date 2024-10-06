#pragma once
#include <box2d/box2d.h>

class ColliderHandler
{
public:
    ColliderHandler();
    void SetSensor(b2Fixture* bodyToTrack);
    bool IsBeingTriggered();
    void AcceptOnlyTriggers(bool acceptOnlyTriggers);
    bool OnTriggerEnter();
    bool OnTriggerExit();
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    
private:
    b2Body* GetDifferentBody(b2Body* body1, b2Body* body2, b2Body* bodyToBeDifferentFrom);
    

    b2Fixture* bodyToTrack;
    int bodiesInside = 0;
    bool onlyTriggers = true;

    b2Body* lastBodyEnter;
    b2Body* lastBodyExit;
};