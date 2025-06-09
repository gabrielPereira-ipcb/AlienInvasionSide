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

Projectile::Projectile(float startX, float startY) {
    this->x = startX;
    this->y = startY;
    this->speed = 2.0f; // Fixed increment per update() call for now. Units: world units per update.
                        // If deltaTime was used, this would be world units per second.
    this->active = true;
    this->width = 1.0f;  // Example width of the projectile
    this->height = 4.0f; // Example height of the projectile (a thin rectangle)
}

void Projectile::update(float deltaTime) {
    if (!this->active) {
        return;
    }

    // Using a fixed increment for movement upwards, as deltaTime is not yet fully integrated
    // To use deltaTime: this->y += this->speed * deltaTime;
    // For now:
    this->y += this->speed; // Move projectile up by 'speed' units each frame/update call

    // Deactivate if it goes off the top of the screen
    if (this->y - this->height / 2 > coordenadasMundo[3]) { // Check if bottom of projectile is above Ymax
        this->active = false;
    }
}

void Projectile::render() {
    if (!this->active) {
        return;
    }

    glColor3f(1.0f, 1.0f, 1.0f); // White color for player projectile

    // Calculate corners based on center (x,y) and width/height
    float left = this->x - this->width / 2;
    float right = this->x + this->width / 2;
    float bottom = this->y - this->height / 2;
    float top = this->y + this->height / 2;

    glRectf(left, bottom, right, top);
}
