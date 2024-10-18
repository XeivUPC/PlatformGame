#pragma once
#include <vector>
#include <box2d/box2d.h>

class CollisionSensor;

class CollisionsManager : public b2ContactListener { //collisionmanager, collisiondector
public:
    CollisionsManager();
    ~CollisionsManager(); 

private:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact)override;
};
