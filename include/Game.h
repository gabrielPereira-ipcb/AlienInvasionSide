#ifndef GAME_H
#define GAME_H

#include "BibSistema.h"
#include "Player.h" // Include Player header
#include "Invader.h" // Include Invader header
#include <vector>    // For std::vector

class Game {
public:
    Player player; // Public for simplicity, consider accessors later
    std::vector<Invader> invaders; // Public for now

    Game();
    void init(int argc, char** argv);
    void start();

    static void displayCallback();
    static void reshapeCallback(int w, int h);
    static void keyboardCallback(unsigned char key, int x, int y);
    static void idleCallback();

private:
    static Game* currentInstance; // Static pointer to the current game instance
    float lastFrameTime;          // For delta time calculation
};

#endif // GAME_H
