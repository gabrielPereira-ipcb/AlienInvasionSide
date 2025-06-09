#include "Game.h" // Should be in the include path
#include <stdlib.h> // For srand
#include <time.h>   // For time

int main(int argc, char** argv) {
    srand(time(NULL)); // Seed random number generator once

    Game alienInvasionGame;
    alienInvasionGame.init(argc, argv);
    alienInvasionGame.start(); // This will enter the GLUT main loop

    return 0; // This line might not be reached if glutMainLoop() doesn't return
}
