#include "Projectile.h"

// Ensure necessary headers for OpenGL/GLUT are available.
// BibSistema.h might provide these, or they might need to be explicit.
#if defined(__APPLE__)
    #include <GLUT/glut.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <GL/glut.h>
#elif defined(__linux__)
    #include <GL/glut.h>
#else
    #warning "Unsupported platform: Manual GLUT inclusion might be needed."
    #include <GL/glut.h> // Default attempt
#endif

// Access global world coordinates for boundary checks
// These are declared in BibSistema.h and defined in GlobalVars.cpp
extern GLfloat coordenadasMundo[4]; // {Xmin, Xmax, Ymin, Ymax}

Projectile::Projectile(float startX, float startY, float velX, float velY, bool isPlayer) {
    this->x = startX;
    this->y = startY;
    this->vx = velX;
    this->vy = velY;
    this->active = true;
    this->width = 1.0f;  // Example width
    this->height = 4.0f; // Example height (thin rectangle)
    this->isPlayerProjectile = isPlayer;
}

void Projectile::update(float deltaTime) {
    if (!this->active) {
        return;
    }

    this->x += this->vx * deltaTime;
    this->y += this->vy * deltaTime;

    // Deactivate if it goes off any screen edge
    if (this->x - this->width / 2 > coordenadasMundo[1] || // Off right
        this->x + this->width / 2 < coordenadasMundo[0] || // Off left
        this->y - this->height / 2 > coordenadasMundo[3] || // Off top
        this->y + this->height / 2 < coordenadasMundo[2])   // Off bottom
    {
        this->active = false;
    }
}

void Projectile::render() {
    if (!this->active) {
        return;
    }

    if (this->isPlayerProjectile) {
        glColor3f(1.0f, 1.0f, 1.0f); // White color for player projectile
    } else {
        glColor3f(1.0f, 0.5f, 0.0f); // Orange-Red for invader projectiles (distinguishable)
    }

    // Calculate corners based on center (x,y) and width/height
    float left = this->x - this->width / 2;
    float right = this->x + this->width / 2;
    float bottom = this->y - this->height / 2;
    float top = this->y + this->height / 2;

    glRectf(left, bottom, right, top);
}
