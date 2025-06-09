#ifndef PLAYER_H
#define PLAYER_H

#include "BibSistema.h" // For naveJogadorTamanho and GL types
#include "Projectile.h" // For Projectile class
#include <vector>       // For std::vector

class Player {
public:
    Player();
    void render();
    void move(float dx); // Move horizontally
    void shoot();
    void updateProjectiles(float deltaTime);
    void renderProjectiles();

    std::vector<Projectile> projectiles; // Public for now for easier access from Game

private:
    float x, y, angle;
    float movementSpeed;
    float shootCooldown;
    const float shootInterval = 0.5f; // Seconds, reduced for better testing
};

#endif // PLAYER_H
