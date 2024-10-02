#pragma once
#include <vector>
#include <box2d/box2d.h>

class ColliderHandler;

class CollidersManager : public b2ContactListener {
public:
    void AddSensor(ColliderHandler* sensor);
    void RemoveSensor(ColliderHandler* sensor);
    

private:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact)override;
    std::vector<ColliderHandler*> sensors;
};
