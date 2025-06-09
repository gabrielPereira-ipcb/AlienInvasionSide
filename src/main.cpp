#include "Game.h" // Should be in the include path

int main(int argc, char** argv) {
    Game alienInvasionGame;
    alienInvasionGame.init(argc, argv);
    alienInvasionGame.start(); // This will enter the GLUT main loop

    return 0; // This line might not be reached if glutMainLoop() doesn't return
}
