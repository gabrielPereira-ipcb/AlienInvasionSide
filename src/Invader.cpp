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

Invader::Invader(float startX, float startY) {
    this->x = startX;
    this->y = startY;
    this->active = true;
    // this->type = 0; // Default type for now
}

void Invader::render() {
    if (!this->active) {
        return;
    }

    glColor3f(1.0f, 0.0f, 0.0f); // Red color for invaders

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
