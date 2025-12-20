#include "PhysicsObject.h"


PhysicsObject::PhysicsObject(std::vector<glm::vec2> h) {
    // Hitbox MUST be a vector of points, IN THE CORRECT SEQUENCE.
    // So when you want to make a rectangular hitbox,
    // the vertices must go {top-left, top-right, bottom-right, bottom-left}.
    // Or the other way around is also fine.
    //this->hitbox = hitbox;
    for(int i = 0; i<sizeof(this->vertices)/sizeof(float); i+=3) {
        this->hitbox.push_back(std::make_pair( & this->vertices[i], &this->vertices[i + 1] ));
    }
}