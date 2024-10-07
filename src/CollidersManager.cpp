#include "CollidersManager.h"
#include "ColliderHandler.h"

CollidersManager::CollidersManager()
{

}

CollidersManager::~CollidersManager()
{
    sensors.clear();
}

void CollidersManager::AddSensor(ColliderHandler* sensor)
{
    sensors.emplace_back(sensor);
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

    /*class Sensor;
    const b2BodyUserData& userData = contact->GetFixtureA()->GetBody()->GetUserData();
    Sensor* sensorA = (Sensor*)userData.pointer;
    Sensor* sensorB = ....;*/
}

void CollidersManager::EndContact(b2Contact* contact)
{
    for (auto sensor : sensors) {
        sensor->EndContact(contact);
    }
}
