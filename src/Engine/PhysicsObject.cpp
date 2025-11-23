#include "PhysicsObject.h"

//TODO optimise colides
// 1. draw boxes around each object, so they fit perfectly
// 2. see if the boxes are overlapping
// 3. if they are find the centers of them and find the point S = middle between thoose two
// 4. sort the edges by how far they are from S 
// 5. check the edges like normal, but in sorted order
// okay, tbh i think that with how little edges each object is going to have,
// this is going to be actually slower than the normal O(n^2), but fuck you i went to Gródek like 3 times
// it changes you, man. it changes you.
bool PhysicsObject::colides(PhysicsObject* B)
{
    //checks if two PhysicsObjects' hitboxes are coliding
    auto A = this;
    int aSize = A->hitbox.size();
    int bSize = B->hitbox.size();
    if (aSize == 0 || bSize == 0) {
        return false;
    }

   
    for (int i = 0; i < aSize - 1; i++) {
        for (int j = 0; j < bSize - 1; j++) {
            if (intersects(A->hitbox[i], A->hitbox[i + 1], B->hitbox[j], B->hitbox[j + 1])) {
                return true;
            }
        }
        if (intersects(A->hitbox[i], A->hitbox[i + 1], B->hitbox[0], B->hitbox[bSize - 1])) {
            return true;
        }
    }

    for (int i = 0; i < bSize - 1; i++) {
        if (intersects(A->hitbox[aSize - 1], A->hitbox[0], B->hitbox[i], B->hitbox[i + 1])) {
            return true;
        }
    }

    return intersects(A->hitbox[aSize - 1], A->hitbox[0], B->hitbox[bSize-1], B->hitbox[0]);
}

bool PhysicsObject::intersects(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D) {
    //checks if the line segment AB intersects the line segment CD
    
    //assertions
    glm::vec2* arr[4] = {&A, &B, &C, &D };
    for (int i = 0; i < sizeof(arr) / sizeof(glm::vec2); i++) {
        assert(isfinite(arr[i]->x) && isfinite(arr[i]->y) && "All the points' coordinates must not be nan, or inf)");
    }


    float x1 = A.x, x2 = B.x, x3 = C.x, x4 = D.x, y1 = A.y, y2 = B.y, y3 = C.y, y4 = D.y;
    float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    //std::cout << A.x << " " << A.y << " " << B.x << " " << B.y << " " << C.x << " " << C.y << " " << D.x << " " << D.y << " " << std::endl;
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        
        return true;
    }
    printf("%d %d \n", uA, uB);
    return false;
}

bool PhysicsObject::intersects(std::pair<float, float> A, std::pair<float, float> B, std::pair<float, float> C, std::pair<float, float> D)
{
    this->intersects(glm::vec2(A.first, A.second), glm::vec2(B.first, B.second), glm::vec2(C.first, C.second), glm::vec2(D.first, D.second));
    return false;
}

bool PhysicsObject::intersects(std::pair<float, float>* A, std::pair<float, float>* B, std::pair<float, float>* C, std::pair<float, float>* D)
{
    this->intersects(glm::vec2(A->first, A->second), glm::vec2(B->first, B->second), glm::vec2(C->first, C->second), glm::vec2(D->first, D->second));
    return false;
}

bool PhysicsObject::intersects(std::pair<float*, float*> A, std::pair<float*, float*> B, std::pair<float*, float*> C, std::pair<float*, float*> D)
{
    this->intersects(glm::vec2(*A.first, *A.second), glm::vec2(*B.first, *B.second), glm::vec2(*C.first, *C.second), glm::vec2(*D.first, *D.second));
    return false;
}

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