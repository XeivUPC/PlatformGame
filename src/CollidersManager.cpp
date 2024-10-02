#include "CollidersManager.h"
#include "ColliderHandler.h"

void CollidersManager::AddSensor(ColliderHandler* sensor)
{
    sensors.push_back(sensor);
}

void CollidersManager::RemoveSensor(ColliderHandler* sensor)
{
    sensors.erase(std::remove(sensors.begin(), sensors.end(), sensor), sensors.end());
}

void CollidersManager::BeginContact(b2Contact* contact)
{
    for (auto sensor : sensors) {
        sensor->BeginContact(contact);
    }
}

void CollidersManager::EndContact(b2Contact* contact)
{
    for (auto sensor : sensors) {
        sensor->EndContact(contact);
    }
}
