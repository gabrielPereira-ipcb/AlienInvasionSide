#ifndef INVADER_H
#define INVADER_H

#include "BibSistema.h" // For GLfloat

class Invader {
public:
    float x, y;
    bool active;
    // int type; // For different invader types later

    Invader(float startX, float startY);
    void render();
    // void update(float deltaTime); // Static invaders for now
};

#endif // INVADER_H
