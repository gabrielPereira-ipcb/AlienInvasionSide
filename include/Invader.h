#ifndef INVADER_H
#define INVADER_H

#include "BibSistema.h" // For GLfloat

// Define InvaderType enum class before it's used in the Invader class
enum class InvaderType {
    EXPLORER,
    SOLDIER,
    COMMANDER
};

class Invader {
public:
    float x, y;
    bool active;
    InvaderType type; // Type of the invader
    int pointsValue;  // How many points this invader is worth

    Invader(float startX, float startY, InvaderType invaderType); // Updated constructor
    void render();
    // void update(float deltaTime); // Static invaders for now, type might influence behavior later
};

#endif // INVADER_H
