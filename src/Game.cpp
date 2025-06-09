#include "Game.h"
// BibSistema.h should be included via Game.h, which should provide GLUT
// However, to be safe in typical cross-platform GLUT setups, explicitly include:
#if defined(__APPLE__)
    #include <GLUT/glut.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include <windows.h> // Required for GL/gl.h on Windows for some compilers
    #include <GL/glut.h>
#elif defined(__linux__)
    #include <GL/glut.h>
#else
    #warning "Unsupported platform: Manual GLUT inclusion might be needed."
    #include <GL/glut.h> // Default attempt
#endif

#include <iostream> // For exit() potentially, or can use stdlib.h for exit(0)

// Definition for static member
Game* Game::currentInstance = nullptr;

// Extern global variables that are defined in GlobalVars.cpp
// These are declared in BibSistema.h
extern GLfloat coordenadasMundo[4];
extern GLfloat invaderTamanho; // Used for spacing invaders
// extern GLfloat naveJogadorTamanho; // Not directly used in Game.cpp but for context

Game::Game() : lastFrameTime(0.0f) { // Initialize lastFrameTime
    currentInstance = this;
    // Player object 'player' is default-constructed here
    // Invader spawning moved to init()
}

// This is the correct init method (the previous one is now removed)
void Game::init(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Double buffered, RGB color
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Alien Invasion");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

    // Setup projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Use global coordinates for the orthographic projection
    gluOrtho2D(coordenadasMundo[0], coordenadasMundo[1], coordenadasMundo[2], coordenadasMundo[3]);

    glMatrixMode(GL_MODELVIEW); // Switch back to model-view matrix
    glLoadIdentity();           // Reset model-view matrix

    // Register callbacks
    glutDisplayFunc(Game::displayCallback);
    glutReshapeFunc(Game::reshapeCallback);
    glutKeyboardFunc(Game::keyboardCallback);
    glutIdleFunc(Game::idleCallback); // For animation and continuous updates

    // Initialize lastFrameTime after GLUT is initialized, if using glutGet
    this->lastFrameTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    // Spawning Invaders
    if (currentInstance) { // Should always be true as 'this' is the currentInstance
        currentInstance->invaders.clear();
        int numRows = 2;    // Start with 2 rows
        int numCols = 8;    // 8 columns as per GDD
        // Ensure invaderTamanho is accessible (extern GLfloat invaderTamanho;)
        float spacing = invaderTamanho * 2.0f; // Spacing between invaders (center to center)

        // Calculate total width of invader block to center it
        float totalWorldWidth = coordenadasMundo[1] - coordenadasMundo[0];
        float blockWidth = (numCols - 1) * spacing; // Width of the invader block itself
        float startX = coordenadasMundo[0] + (totalWorldWidth - blockWidth) / 2.0f;

        // Start Y from top, ensure enough space for a couple of rows and padding
        float startY = coordenadasMundo[3] - invaderTamanho - spacing;

        for (int r = 0; r < numRows; ++r) {
            for (int c = 0; c < numCols; ++c) {
                float invaderX = startX + c * spacing;
                float invaderY = startY - r * spacing;
                currentInstance->invaders.emplace_back(invaderX, invaderY);
            }
        }
    }
}

void Game::start() {
    glutMainLoop(); // Enter the GLUT event processing loop
}

void Game::displayCallback() {
    if (!currentInstance) return;

    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
    glLoadIdentity();             // Reset the model-view matrix

    // Render game objects
    currentInstance->player.render(); // Render the player
    currentInstance->player.renderProjectiles(); // Render projectiles

    // Render invaders
    if (currentInstance) {
        for (auto& invader : currentInstance->invaders) {
            if (invader.active) {
                invader.render();
            }
        }
    }

    glutSwapBuffers(); // Swap the front and back buffers (for double buffering)
}

void Game::reshapeCallback(int w, int h) {
    if (h == 0) h = 1; // Prevent a divide by zero
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); // Set the viewport to the new window size

    // Re-apply the orthographic projection based on the world coordinates
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(coordenadasMundo[0], coordenadasMundo[1], coordenadasMundo[2], coordenadasMundo[3]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Game::keyboardCallback(unsigned char key, int x, int y) {
    if (!currentInstance) return;

    switch (key) {
        case 'a': // Move left
        case 'A':
            currentInstance->player.move(-1.0f); // Use a value that makes sense for player speed
            break;
        case 'd': // Move right
        case 'D':
            currentInstance->player.move(1.0f);
            break;
        case ' ': // Spacebar to shoot
            if (currentInstance) currentInstance->player.shoot();
            break;
        case 27: // ESC key to exit
            exit(0); // Or a more graceful shutdown if implemented
            break;
    }
    // glutPostRedisplay(); // Not strictly necessary if idleCallback is used
}

void Game::idleCallback() {
    if (!currentInstance) return;

    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Time in seconds
    float deltaTime = currentTime - currentInstance->lastFrameTime;
    currentInstance->lastFrameTime = currentTime;

    // Cap deltaTime to prevent large jumps (e.g., during debugging pauses)
    if (deltaTime > 0.1f) { // Max deltaTime of 0.1 seconds (10 FPS equivalent)
        deltaTime = 0.1f;
    }

    // Update game logic using deltaTime
    currentInstance->player.updateProjectiles(deltaTime); // Moves projectiles, checks off-screen, and cleans up player's list

    // Collision Detection Logic (Projectile vs Invader)
    if (currentInstance) {
        // invaderTamanho is already extern and available
        for (auto& proj : currentInstance->player.projectiles) {
            if (!proj.active) continue;

            for (auto& inv : currentInstance->invaders) {
                if (!inv.active) continue;

                // AABB Collision Check
                float projLeft = proj.x - proj.width / 2.0f;
                float projRight = proj.x + proj.width / 2.0f;
                float projTop = proj.y + proj.height / 2.0f;
                float projBottom = proj.y - proj.height / 2.0f;

                float invHalfSize = invaderTamanho / 2.0f;
                float invLeft = inv.x - invHalfSize;
                float invRight = inv.x + invHalfSize;
                float invTop = inv.y + invHalfSize;
                float invBottom = inv.y - invHalfSize;

                if (projRight > invLeft && projLeft < invRight &&
                    projTop > invBottom && projBottom < invTop) {

                    proj.active = false; // Deactivate projectile
                    inv.active = false;  // Deactivate invader

                    // std::cout << "Collision! Proj: (" << proj.x << "," << proj.y << ") Inv: (" << inv.x << "," << inv.y << ")" << std::endl;

                    break; // Projectile hits one invader and is done
                }
            }
        }
        // Note: Inactive invaders are not removed from the currentInstance->invaders vector here.
        // They will simply not be rendered or participate in future logic if their 'active' flag is checked.
        // Projectiles marked inactive here will be removed from player.projectiles in the *next* call to
        // player.updateProjectiles() due to its internal cleanup.
    }

    // Add other game updates here (e.g., currentInstance->invaders.update(deltaTime);)

    glutPostRedisplay(); // Request redraw to show changes
}
