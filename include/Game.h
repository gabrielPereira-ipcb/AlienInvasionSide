#ifndef GAME_H
#define GAME_H

#include "BibSistema.h"
#include "Player.h" // Include Player header
#include "Invader.h" // Include Invader header
#include <vector>    // For std::vector

class Game {
// Define GameState enum before it's used in the class
enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER,
    STATE_BETWEEN_WAVES
};

public:
    Player player; // Public for simplicity, consider accessors later
    std::vector<Invader> invaders; // Public for now
    std::vector<Projectile> invaderProjectiles; // For projectiles shot by invaders
    GameState currentState; // Current state of the game
    int currentWave;        // Current wave number

    Game();
    void init(int argc, char** argv);
    void spawnInvadersForWave(int waveNumber); // Public method to setup waves
    void resetGame(); // Method to reset game state for a new game
    void start();

    static void displayCallback();
    static void reshapeCallback(int w, int h);
    static void keyboardCallback(unsigned char key, int x, int y);
    static void idleCallback();

private:
    static Game* currentInstance; // Static pointer to the current game instance
    float lastFrameTime;          // For delta time calculation

    // Invader group movement variables
    float invaderMoveTimer;
    float invaderMoveInterval;    // How often invaders attempt to move
    float invaderStepSize;        // How far they move in one step
    int invaderDirection;         // 1 for right, -1 for left
    bool invadersShouldMoveDown;  // Flag to indicate vertical movement for the current step

    // Invader shooting variables
    float invaderShootTimer;
    float invaderShootIntervalMin;
    float invaderShootIntervalMax;
    float invaderProjectileSpeed;
    float betweenWaveTimer;       // Timer for the duration between waves
};

#endif // GAME_H
