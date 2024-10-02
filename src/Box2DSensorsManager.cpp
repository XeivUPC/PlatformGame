#include "Box2DSensorsManager.h"
#include "Box2DSensorController.h"

void Box2DSensorsManager::AddSensor(Box2DSensorController* sensor)
{
    sensors.push_back(sensor);
}

void Box2DSensorsManager::RemoveSensor(Box2DSensorController* sensor)
{
    sensors.erase(std::remove(sensors.begin(), sensors.end(), sensor), sensors.end());
}

void Box2DSensorsManager::BeginContact(b2Contact* contact)
{
    for (auto sensor : sensors) {
        sensor->BeginContact(contact);
    }
}

void Box2DSensorsManager::EndContact(b2Contact* contact)
{
    for (auto sensor : sensors) {
        sensor->EndContact(contact);
    }
}
