#include "Player.h"
#include <GL/glut.h> // Or the correct GLUT header from BibSistema.h

// Global variables are now defined in GlobalVars.cpp
// extern GLfloat tamanhoDoMundo; // Declared in BibSistema.h
// extern GLfloat coordenadasMundo[4]; // Declared in BibSistema.h
// extern GLfloat naveJogadorTamanho; // Declared in BibSistema.h
#include <iostream>  // For std::cout for debugging (optional)
#include <algorithm> // For std::remove_if

extern GLfloat naveJogadorTamanho; // Make sure this is accessible (declared in BibSistema.h, defined in GlobalVars.cpp)


Player::Player() : angle(0.0f), movementSpeed(1.0f), shootCooldown(0.0f) { // Initialize movementSpeed and shootCooldown
    // Initialize position to the center of the bottom part of the screen
    this->x = (coordenadasMundo[0] + coordenadasMundo[1]) / 2.0f;
    // Place the ship slightly above the bottom edge, considering its size
    this->y = coordenadasMundo[2] + naveJogadorTamanho;
}

void Player::shoot() {
    if (shootCooldown <= 0.0f) {
        // Projectile starts from the player's current position (tip of the ship)
        // For a triangle pointing up, the tip is at y + halfSize.
        // If player y is base, tip is y + naveJogadorTamanho. If player y is center, tip is y + naveJogadorTamanho/2
        // Player's y is currently set to bottom edge + naveJogadorTamanho, so its "center" is roughly y.
        // Let's assume player's (x,y) is the center of the base of the triangle.
        // Tip would be player.x, player.y + naveJogadorTamanho (if y is bottom) or player.y + halfsize (if y is center)
        // Current Player render draws triangle top vertex at (0, halfsize) relative to player's (x,y)
        projectiles.emplace_back(this->x, this->y + naveJogadorTamanho / 2.0f);
        shootCooldown = shootInterval;
        // std::cout << "Player shot! Cooldown: " << shootCooldown << "s. Projectiles: " << projectiles.size() << std::endl;
    } else {
        // std::cout << "Player shoot on cooldown: " << shootCooldown << "s" << std::endl;
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

void Player::move(float dx) {
    this->x += dx * this->movementSpeed;

    // Boundary checks
    float halfSize = naveJogadorTamanho / 2.0f;
    if (this->x - halfSize < coordenadasMundo[0]) { // Left boundary
        this->x = coordenadasMundo[0] + halfSize;
    }
    if (this->x + halfSize > coordenadasMundo[1]) { // Right boundary
        this->x = coordenadasMundo[1] - halfSize;
    }
}

void Player::render() {
    glPushMatrix();
    // Translate to the player's current position
    glTranslatef(this->x, this->y, 0.0f);
    // glRotatef(this->angle, 0.0f, 0.0f, 1.0f); // Rotation can be added later

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
