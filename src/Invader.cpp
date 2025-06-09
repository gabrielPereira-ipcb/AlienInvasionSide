#include "Invader.h"

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

// Access the global invader size
// This is declared in BibSistema.h (presumably) and defined in GlobalVars.cpp
// If not in BibSistema.h, this extern declaration makes it available from GlobalVars.cpp
extern GLfloat invaderTamanho;

Invader::Invader(float startX, float startY, InvaderType invaderType) {
    this->x = startX;
    this->y = startY;
    this->active = true;
    this->type = invaderType;

    switch (this->type) {
        case InvaderType::EXPLORER:
            this->pointsValue = 10;
            break;
        case InvaderType::SOLDIER:
            this->pointsValue = 20;
            break;
        case InvaderType::COMMANDER:
            this->pointsValue = 30;
            break;
        default:
            this->pointsValue = 0; // Should not happen
            break;
    }
}

void Invader::render() {
    if (!this->active) {
        return;
    }

    switch (this->type) {
        case InvaderType::EXPLORER:
            glColor3f(1.0f, 0.5f, 0.5f); // Light Red/Pink for Explorer
            break;
        case InvaderType::SOLDIER:
            glColor3f(1.0f, 1.0f, 0.5f); // Light Yellow for Soldier
            break;
        case InvaderType::COMMANDER:
            glColor3f(0.5f, 1.0f, 1.0f); // Light Cyan for Commander
            break;
        default:
            glColor3f(0.7f, 0.7f, 0.7f); // Grey for default/error
            break;
    }

    // Draw a simple square for the invader
    float halfSize = invaderTamanho / 2.0f;

    glBegin(GL_QUADS); // Or use glRectf
        glVertex2f(this->x - halfSize, this->y - halfSize); // Bottom-left
        glVertex2f(this->x + halfSize, this->y - halfSize); // Bottom-right
        glVertex2f(this->x + halfSize, this->y + halfSize); // Top-right
        glVertex2f(this->x - halfSize, this->y + halfSize); // Top-left
    glEnd();

    // Alternative using glRectf:
    // glRectf(this->x - halfSize, this->y - halfSize, this->x + halfSize, this->y + halfSize);
}
