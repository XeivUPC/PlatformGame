#pragma once
#include <vector>
#include <box2d/box2d.h>

class ColliderHandler;

class CollidersManager : public b2ContactListener { //collisionmanager, collisiondector
public:
    CollidersManager();
    ~CollidersManager(); 

private:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact)override;
};
