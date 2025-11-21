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
    if (A->hitbox.size() == 0 || B->hitbox.size() == 0) {
        return false;
    }
    int aSize = A->hitbox.size();
    int bSize = B->hitbox.size();
    for (int i = 0; i < aSize - 1; i++) {
        for (int j = 0; j < bSize - 1; j++) {
            if (intersects(A->hitbox[i], A->hitbox[i + 1], B->hitbox[j], B->hitbox[j + 1])) {
                return true;
            }
        }
    }
    return intersects(A->hitbox[aSize - 1], A->hitbox[0], B->hitbox[bSize], B->hitbox[0]);
}

bool PhysicsObject::intersects(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D) {
    //checks if the line segment AB intersects the line segment CD
    
    //assertions
    glm::vec2 arr[4] = { A, B, C, D };
    for (int i = 0; i < sizeof(arr) / sizeof(glm::vec2); i++) {
        assert(isfinite(arr[i].x) && isfinite(arr[i].y) && "All the points' coordinates can't be nan, or inf)");
    }
    delete arr;

    //make sure A is to the left of B and C to the left of D
    if (A.x > B.x) {
        std::swap(A, B);
    }
    if (C.x > D.x) {
        std::swap(C, D);
    }
    //make sure A is to the left of C
    if (A.x < C.x) {
        std::swap(A, C);
        std::swap(B, D);
    }

    if (B.x < C.x || std::max(A.y, B.y) < std::min(C.y, D.y)) { return false; }


    //if both are vertical lines
    if (A.x == B.x && C.x == D.x) {
        //if they are on a diffrent x
        if (A.x != C.x) {
            return false;
        }

        //changing the names of points, so that the first line has A higher than B, the second has C higher than D
        //and so that A is higher than C. 
        if (A.y < B.y) { std::swap(A, B); }
        if (C.y < D.y) { std::swap(C, D); }
        if (A.y < C.y) {
            std::swap(A, C);
            std::swap(B, D);
        }
        
        return A.y > C.y && B.y > D.y;
    }

    //if both are horizontal lines
    if (A.y == B.y && C.y == D.y) {
        //if they are on a diffrent y
        if (A.y != C.y) {
            return false;
        }


        return A.x > C.x && B.x > D.x;
    }

    // if C and D are on the same y, swap them with A and B.
    // this way, if either line segment is horizontal, it's AB;
    if (C.y == D.y) {
        std::swap(A, B);
        std::swap(C, D);
    }

    
    // function, on which's graph, points C and D sit, is called f2().
    // m2 is the slope of f2(), and b2 is the intersect.
    double m2 = (C.y - D.y) / (C.x - D.x);
    double b2 = C.y - m2 * C.x;
    std::cout << A.y << '<' << m2 * A.x + b2 << ' ' << ' ' << B.y << '>' << m2 * B.x + b2 << std::endl;
    // (if A is above the f() graph AND B is below) OR (A is below AND B is above) OR A is on the graph OR B is on the graph
    return ((A.y < m2 * A.x + b2) == (B.y > m2 * B.x + b2)) || (A.y == m2 * A.x + b2 && A.x >= C.x)
        || (B.y == m2 * B.x + b2 && B.x < D.x);
    
    float x1 = A.x, x2 = B.x, x3 = C.x, x4 = D.x, y1 = A.y, y2 = B.y, y3 = C.y, y4 = D.y;
    float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        return true;
    }
    return false;
}

PhysicsObject::PhysicsObject(std::vector<glm::vec2> h) {
    // Hitbox MUST be a vector of points, IN THE CORRECT SEQUENCE.
    // So when you want to make a rectangular hitbox,
    // the vertices must go {top-left, top-right, bottom-right, bottom-left}.
    // Or the other way around is also fine.
    //this->hitbox = hitbox;
    for(int i = 0; i<sizeof(this->vertices)/sizeof(float); i+=3) {
        this->hitbox.push_back(glm::vec2(this->vertices[i], this->vertices[i + 1]));
    }
}