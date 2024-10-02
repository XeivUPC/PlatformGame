#pragma once
#include <vector>
#include <box2d/box2d.h>

class Box2DSensorController;

class Box2DSensorsManager : public b2ContactListener {
public:
    void AddSensor(Box2DSensorController* sensor);
    void RemoveSensor(Box2DSensorController* sensor);
    

private:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact)override;
    std::vector<Box2DSensorController*> sensors;
};
