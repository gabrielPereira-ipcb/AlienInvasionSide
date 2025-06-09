#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "BibSistema.h" // For GLfloat and potentially other types

class Projectile {
public:
    float x, y;
    float speed;
    bool active;
    float width;
    float height;

    Projectile(float startX, float startY);
    void update(float deltaTime); // deltaTime for future use, might use fixed step for now
    void render();
};

#endif // PROJECTILE_H
