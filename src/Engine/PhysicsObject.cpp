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
    auto A = this;
    int i = 0;
    std::vector<glm::vec2>  edgesA(sizeof(A->indices) / sizeof(float));
    while (i < sizeof(A->indices) / sizeof(float)) {
        edgesA[i] = glm::vec2(A->vertices[i], A->vertices[i + 1]);
        i++;
        edgesA[i] = glm::vec2(A->vertices[i], A->vertices[i + 1]);
        i++;
        edgesA[i] = glm::vec2(A->vertices[i], A->vertices[i - 2]);
        i++;
    }
    i = 0;
    std::vector<glm::vec2>  edgesB(sizeof(B->indices) / sizeof(float));
    while (i < sizeof(B->indices) / sizeof(float)) {
        edgesB[i] = glm::vec2(B->vertices[i], B->vertices[i + 1]);
        i++;
        edgesB[i] = glm::vec2(B->vertices[i], B->vertices[i + 1]);
        i++;
        edgesB[i] = glm::vec2(B->vertices[i], B->vertices[i - 2]);
        i++;
    }

    return false;
}
