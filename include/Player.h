#ifndef PLAYER_H
#define PLAYER_H

#include "BibSistema.h" // For naveJogadorTamanho and GL types
#include "Projectile.h" // For Projectile class
#include <vector>       // For std::vector

class Player {
public:
    // Store projectile speed magnitude here, or make it a global const
    static const float projectileSpeedMagnitude = 150.0f;

    Player();
    void render();
    void move(float dx_input, float dy_input); // For 2D movement
    void rotate(float degrees); // For rotation
    void shoot();
    void updateProjectiles(float deltaTime);
    void renderProjectiles();
    void takeHit(); // Method for when player is hit

    std::vector<Projectile> projectiles; // Public for now for easier access from Game
    int lives;                           // Player lives
    int score;                           // Player score
    float angle;                         // Player's current angle in degrees

private:
    float x, y; // Angle moved to public for easier access if needed, or keep private
    float movementSpeed;
    float shootCooldown;
    const float shootInterval = 0.5f; // Seconds, reduced for better testing
};

#endif // PLAYER_H
