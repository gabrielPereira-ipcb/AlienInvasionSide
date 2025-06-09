#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "BibSistema.h" // For GLfloat and potentially other types

class Projectile {
public:
    float x, y;
    // float speed; // Replaced by vx, vy
    bool active;
    float width;
    float height;
    bool isPlayerProjectile;
    float vx, vy; // Velocity components

    Projectile(float startX, float startY, float velX, float velY, bool isPlayer = true);
    void update(float deltaTime);
    void render();
};

#endif // PROJECTILE_H
