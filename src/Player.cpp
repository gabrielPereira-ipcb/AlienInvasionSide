#include "Player.h"
#include <GL/glut.h> // Or the correct GLUT header from BibSistema.h

// Global variables are now defined in GlobalVars.cpp
// extern GLfloat tamanhoDoMundo; // Declared in BibSistema.h
// extern GLfloat coordenadasMundo[4]; // Declared in BibSistema.h
// extern GLfloat naveJogadorTamanho; // Declared in BibSistema.h
#include <iostream>  // For std::cout for debugging (optional)
#include <algorithm> // For std::remove_if
#include <cmath>     // For M_PI, sin, cos, round

extern GLfloat naveJogadorTamanho; // Make sure this is accessible (declared in BibSistema.h, defined in GlobalVars.cpp)
extern GLfloat coordenadasMundo[4]; // Used in constructor


Player::Player() : x(0.0f), y(0.0f), angle(0.0f), movementSpeed(50.0f), shootCooldown(0.0f), lives(3), score(0) {
    // Initialize position to the center of the bottom part of the screen
    this->x = (coordenadasMundo[0] + coordenadasMundo[1]) / 2.0f;
    // Place the ship slightly above the bottom edge, considering its size
    this->y = coordenadasMundo[2] + naveJogadorTamanho;
    this->movementSpeed = 20.0f; // Adjusted movement speed for per-frame delta time based movement
}

void Player::rotate(float degrees) {
    this->angle += degrees;
    while (this->angle >= 360.0f) this->angle -= 360.0f;
    while (this->angle < 0.0f) this->angle += 360.0f;
    // std::cout << "Player angle: " << this->angle << std::endl;
}


void Player::shoot() {
    if (shootCooldown <= 0.0f) {
        float pvx = 0.0f, pvy = 0.0f;
        // Angle 0 = Up. Q (+90 deg) -> ship aims Left. E (-90 deg) -> ship aims Right.
        // Convert angle to radians for trigonometric functions. Standard math angle (0=right) vs game angle (0=up).
        // For 0=Up: sin for X component, cos for Y component. Adjust signs based on rotation direction.
        // Angle for trig: (90 - game_angle)
        float radAngle = (90.0f - this->angle) * (M_PI / 180.0f);

        pvx = cos(radAngle) * projectileSpeedMagnitude;
        pvy = sin(radAngle) * projectileSpeedMagnitude;

        // Determine spawn point relative to player center and rotation
        // For simplicity, let's spawn from player's center for now
        // A more accurate spawn would be from the tip of the ship, rotated.
        // float spawnOffsetX = 0; // Placeholder for rotated offset X
        // float spawnOffsetY = naveJogadorTamanho / 2.0f; // Placeholder for rotated offset Y (tip of triangle)
        // float rotatedSpawnX = this->x + (spawnOffsetX * cos(this->angle * M_PI / 180.0f) - spawnOffsetY * sin(this->angle * M_PI / 180.0f));
        // float rotatedSpawnY = this->y + (spawnOffsetX * sin(this->angle * M_PI / 180.0f) + spawnOffsetY * cos(this->angle * M_PI / 180.0f));

        projectiles.emplace_back(this->x, this->y, pvx, pvy, true);
        shootCooldown = shootInterval;
    }
}


void Player::updateProjectiles(float deltaTime) {
    // Update cooldown
    if (shootCooldown > 0.0f) {
        shootCooldown -= deltaTime;
        if (shootCooldown < 0.0f) shootCooldown = 0.0f; // Clamp to zero
    }

    // Update all active projectiles
    for (auto& p : projectiles) {
        if (p.active) {
            p.update(deltaTime); // Projectile's update method
        }
    }

    // Remove inactive projectiles
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& p) {
            return !p.active;
        }),
        projectiles.end()
    );
    // if (removed != projectiles.end()) {
    //     std::cout << "Removed inactive projectiles. Count: " << projectiles.size() << std::endl;
    // }
}

void Player::renderProjectiles() {
    for (auto& p : projectiles) {
        if (p.active) {
            p.render();
        }
    }
}

void Player::move(float dx_input, float dy_input) {
    // Using deltaTime-based movement requires deltaTime to be passed or a global game speed factor
    // For now, this is a per-call small step. If Game::idleCallback passes deltaTime to a Player::update,
    // then this logic would change.
    // Let's assume movementSpeed is "units per second" and deltaTime is passed to an update method
    // For this subtask, let's treat dx_input/dy_input as direct small changes for simplicity,
    // scaled by a factor of movementSpeed if it's too large.
    // The prompt suggests "movementSpeed * 0.1f". Let's use a fixed step for now.
    float moveStep = 1.5f; // Fixed step per key press event for WASD
                           // If using continuous key down, this would be different.

    this->x += dx_input * moveStep;
    this->y += dy_input * moveStep;

    // Boundary checks
    float halfSize = naveJogadorTamanho / 2.0f; // Assuming square bounds for simplicity
    if (this->x - halfSize < coordenadasMundo[0]) { this->x = coordenadasMundo[0] + halfSize; }
    if (this->x + halfSize > coordenadasMundo[1]) { this->x = coordenadasMundo[1] - halfSize; }
    if (this->y - halfSize < coordenadasMundo[2]) { this->y = coordenadasMundo[2] + halfSize; }
    if (this->y + halfSize > coordenadasMundo[3]) { this->y = coordenadasMundo[3] - halfSize; }
}

void Player::render() {
    if (lives <= 0) return; // Don't render if no lives

    glPushMatrix();
    glTranslatef(this->x, this->y, 0.0f);             // 1. Move to position
    glRotatef(this->angle, 0.0f, 0.0f, 1.0f);       // 2. Rotate around its center (now 0,0 locally)

    // Define half size for convenience in drawing
    float halfSize = naveJogadorTamanho / 2.0f;

    // Set color to green
    glColor3f(0.0f, 1.0f, 0.0f);

    // Begin drawing a triangle
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, halfSize);      // Top vertex (at player's x, y + halfSize)
        glVertex2f(-halfSize, -halfSize); // Bottom-left vertex (at player's x - halfSize, y - halfSize)
        glVertex2f(halfSize, -halfSize);  // Bottom-right vertex (at player's x + halfSize, y - halfSize)
    glEnd();

    glPopMatrix();
}

void Player::takeHit() {
    if (lives > 0) { // Only decrement if alive
        lives--;
        std::cout << "Player hit! Lives remaining: " << lives << std::endl; // Debug output
        if (lives <= 0) {
            std::cout << "Player has no lives left! Game Over (pending proper state)." << std::endl;
            // Future: Set a game over flag or state here
            // For now, player will stop rendering.
        }
    }
}
