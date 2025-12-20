#include "PhysicsObject.h"


PhysicsObject::PhysicsObject(float x, float y, float width, float height):
    WorldObject(x, y, width, height)
{
    // Hitbox MUST be a vector of points, IN THE CORRECT SEQUENCE.
    // So when you want to make a rectangular hitbox,
    // the vertices must go {top-left, top-right, bottom-right, bottom-left}.
    //this->hitbox = hitbox;
    for(int i = 0; i<sizeof(this->verticesVectors)/sizeof(float); i+=3) {
        this->hitbox = &(this->verticesVectors);
    }
}