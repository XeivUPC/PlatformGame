#include <box2d/box2d.h>

class Box2DSensorController : public b2ContactListener
{
public:

    void SetSensor(b2Body* bodyToTrack);
    bool IsBeingTriggered();

    bool OnTriggerEnter(b2Body& bodyToEnter);
    bool OnTriggerExit(b2Body& bodyToExit);
    
private:
    b2Body* GetDifferentBody(b2Body* body1, b2Body* body2, b2Body* bodyToBeDifferentFrom);
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

    b2Body* bodyToTrack;
    int bodiesInside = 0;

    
    b2Body* lastBodyEnter;
    b2Body* lastBodyExit;
};