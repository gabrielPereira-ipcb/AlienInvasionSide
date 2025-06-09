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
#include <string>   // For std::string and std::to_string
#include <cmath>    // For M_PI if Player.cpp needs it (it does for shooting) - already included by Player.cpp though
#include <algorithm> // For std::max

// Definition for static member
Game* Game::currentInstance = nullptr;

// Helper function to draw bitmap text
static void drawBitmapText(float x, float y, void* font, const std::string& str, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y); // Set position for text
    for (char character : str) {
        glutBitmapCharacter(font, character);
    }
}

// Extern global variables that are defined in GlobalVars.cpp
// These are declared in BibSistema.h
extern GLfloat coordenadasMundo[4];
extern GLfloat invaderTamanho; // Used for spacing invaders
// extern GLfloat naveJogadorTamanho; // Not directly used in Game.cpp but for context

Game::Game() :
    lastFrameTime(0.0f),
    invaderMoveTimer(0.0f),
    invaderMoveInterval(1.0f), // Move once per second
    invaderDirection(1),      // Start moving right
    invadersShouldMoveDown(false),
    invaderShootIntervalMin(2.5f), // Min time between invader shots
    invaderShootIntervalMax(5.0f), // Max time
    invaderProjectileSpeed(-75.0f) // Negative for downward
{
    currentInstance = this;
    // Initialize invaderShootTimer with a random value in the range
    // Ensure rand() is seeded (e.g. in main.cpp or Game::init)
    // Note: (invaderShootIntervalMax - invaderShootIntervalMin) could be 0 if Min=Max.
    float shootRange = invaderShootIntervalMax - invaderShootIntervalMin;
    if (shootRange < 0.0f) shootRange = 0.0f; // Avoid issues if min > max
    invaderShootTimer = (static_cast<float>(rand()) / RAND_MAX * shootRange) + invaderShootIntervalMin;

    // invaderStepSize is initialized in init()
    // Player object 'player' is default-constructed here
    // Invader spawning moved to init()
}

// This is the correct init method (the previous one is now removed)
void Game::init(int argc, char** argv) {
    // Initialize invaderStepSize here where invaderTamanho is reliably available
    if (currentInstance) { // Should always be true
      currentInstance->invaderStepSize = invaderTamanho / 2.0f;
      currentInstance->currentState = STATE_MENU; // Set initial game state to MENU
      // srand(time(NULL)); // Seed rand() - it's in main.cpp
    }

    this->currentWave = 1; // Initialize current wave
    // Initial invader setup will be done by spawnInvadersForWave called below

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

    // Spawning Invaders for the first wave
    this->spawnInvadersForWave(this->currentWave);
}

void Game::spawnInvadersForWave(int waveNumber) {
    this->invaders.clear();
    this->invaderProjectiles.clear(); // Also clear any stray invader projectiles

    // Adjust difficulty based on waveNumber
    this->invaderMoveInterval = std::max(0.1f, 1.0f - (waveNumber - 1) * 0.05f);
    this->invaderShootIntervalMin = std::max(0.3f, 2.5f - (waveNumber - 1) * 0.15f);
    this->invaderShootIntervalMax = std::max(0.6f, 5.0f - (waveNumber - 1) * 0.2f);

    if (this->invaderShootIntervalMin >= this->invaderShootIntervalMax) {
        this->invaderShootIntervalMin = std::max(0.1f, this->invaderShootIntervalMax - 0.3f);
    }
    if (this->invaderShootIntervalMax <= this->invaderShootIntervalMin) { // Ensure Max is greater
        this->invaderShootIntervalMax = this->invaderShootIntervalMin + 0.2f;
    }


    // Respawn invaders
    // extern GLfloat invaderTamanho; // Already available via file scope extern
    // extern GLfloat invaderTamanho; // Already available via file scope extern
    // extern GLfloat coordenadasMundo[4]; // Already available via file scope extern
    int numRows = 5; // Increased to 5 rows
    int numCols = 8;
    float spacing = invaderTamanho * 2.0f;
    float totalWorldWidth = coordenadasMundo[1] - coordenadasMundo[0];
    float blockWidth = (numCols - 1) * spacing;
    float startX = coordenadasMundo[0] + (totalWorldWidth - blockWidth) / 2.0f;
    float startY = coordenadasMundo[3] - invaderTamanho - spacing;  // Y position for the top row

    for (int r_idx = 0; r_idx < numRows; ++r_idx) {
        Invader::InvaderType currentType;
        if (r_idx < 2) { // Top 2 rows: Explorers
            currentType = Invader::InvaderType::EXPLORER;
        } else if (r_idx < 4) { // Middle 2 rows: Soldiers
            currentType = Invader::InvaderType::SOLDIER;
        } else { // Bottom row (r_idx == 4): Commanders
            currentType = Invader::InvaderType::COMMANDER;
        }

        for (int c_idx = 0; c_idx < numCols; ++c_idx) {
            float xPos = startX + c_idx * spacing;
            float yPos = startY - r_idx * spacing;
            this->invaders.emplace_back(xPos, yPos, currentType);
        }
    }

    // Reset invader movement state for the new wave
    this->invaderMoveTimer = this->invaderMoveInterval;
    this->invaderDirection = 1;
    this->invadersShouldMoveDown = false;

    // Reset invader shooting timer for the new wave
    if (this->invaderShootIntervalMax > this->invaderShootIntervalMin) {
         this->invaderShootTimer = (static_cast<float>(rand()) / RAND_MAX * (this->invaderShootIntervalMax - this->invaderShootIntervalMin)) + this->invaderShootIntervalMin;
    } else {
        this->invaderShootTimer = this->invaderShootIntervalMin;
    }
    std::cout << "Spawning Wave " << waveNumber
              << ". MoveInt: " << this->invaderMoveInterval
              << ". ShootMin: " << this->invaderShootIntervalMin
              << ". ShootMax: " << this->invaderShootIntervalMax << std::endl;
}

void Game::resetGame() {
    this->player = Player(); // Re-creates player, resets score/lives, and internal projectile list
    this->currentWave = 1;
    this->spawnInvadersForWave(this->currentWave); // Handles invader spawning, projectile clearing, timer resets
    // player.projectiles are cleared by Player() constructor re-assignment.
    // invaderProjectiles are cleared in spawnInvadersForWave.
    // Game timers are reset in spawnInvadersForWave.
    // currentState will be set by the caller of resetGame.
    std::cout << "Game Reset to Wave 1." << std::endl;
}

void Game::start() {
    glutMainLoop(); // Enter the GLUT event processing loop
}

void Game::displayCallback() {
    if (!currentInstance) return;

    glClear(GL_COLOR_BUFFER_BIT); // Clear screen once
    glLoadIdentity(); // Reset model-view matrix, important if using matrix transformations for text.

    if (currentInstance->currentState == STATE_MENU) {
        // Render Menu
        std::string title = "ALIEN INVASION";
        // Approximate centering for title
        float titleWidth = 0;
        for(char ch : title) { titleWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, ch); }
        float approxTitleWorldWidth = titleWidth / 20.0f; // GUESS, adjust this scaling factor
        drawBitmapText((coordenadasMundo[0] + coordenadasMundo[1] - approxTitleWorldWidth) / 2.0f,
                       coordenadasMundo[3] - 20.0f, GLUT_BITMAP_TIMES_ROMAN_24, title, 0.5f, 1.0f, 0.5f);

        std::string startGameText = "Start Game (S)";
        float startTextWidth = 0;
        for(char ch : startGameText) { startTextWidth += glutBitmapWidth(GLUT_BITMAP_9_BY_15, ch); }
        float approxStartTextWorldWidth = startTextWidth / 15.0f; // GUESS
        drawBitmapText((coordenadasMundo[0] + coordenadasMundo[1] - approxStartTextWorldWidth) / 2.0f,
                       coordenadasMundo[3] - 45.0f, GLUT_BITMAP_9_BY_15, startGameText, 1.0f, 1.0f, 1.0f);

        std::string quitText = "Quit (Q or ESC)";
        float quitTextWidth = 0;
        for(char ch : quitText) { quitTextWidth += glutBitmapWidth(GLUT_BITMAP_9_BY_15, ch); }
        float approxQuitTextWorldWidth = quitTextWidth / 15.0f; // GUESS
        drawBitmapText((coordenadasMundo[0] + coordenadasMundo[1] - approxQuitTextWorldWidth) / 2.0f,
                       coordenadasMundo[3] - 60.0f, GLUT_BITMAP_9_BY_15, quitText, 1.0f, 1.0f, 1.0f);

    } else if (currentInstance->currentState == STATE_PLAYING ||
               currentInstance->currentState == STATE_PAUSED ||
               currentInstance->currentState == STATE_BETWEEN_WAVES ||
               currentInstance->currentState == STATE_GAME_OVER) {

        // Render Game Elements (Player, Invaders, Projectiles)
        currentInstance->player.render();
        currentInstance->player.renderProjectiles();
        for (auto& inv : currentInstance->invaders) { if (inv.active) inv.render(); }
        for (auto& p : currentInstance->invaderProjectiles) { if (p.active) p.render(); }

        // Render HUD (Score, Lives)
        std::string scoreStr = "SCORE: " + std::to_string(currentInstance->player.score);
        drawBitmapText(coordenadasMundo[0] + 2.0f, coordenadasMundo[3] - 5.0f, GLUT_BITMAP_9_BY_15, scoreStr, 1.0f, 1.0f, 1.0f);
        std::string livesStr = "LIVES: " + std::to_string(currentInstance->player.lives > 0 ? currentInstance->player.lives : 0);
        drawBitmapText(coordenadasMundo[0] + 2.0f, coordenadasMundo[2] + 3.0f, GLUT_BITMAP_9_BY_15, livesStr, 1.0f, 1.0f, 1.0f);

        // State-Specific Messages overlayed on game screen
        if (currentInstance->currentState == STATE_GAME_OVER) {
            // Define centerX and centerY for this block
            // extern GLfloat coordenadasMundo[4]; // Already globally accessible from top of file
            float centerX = coordenadasMundo[0] + (coordenadasMundo[1] - coordenadasMundo[0]) / 2.0f;
            float centerY = coordenadasMundo[2] + (coordenadasMundo[3] - coordenadasMundo[2]) / 2.0f;

            std::string gameOverStr = "GAME OVER";
            // Note: The previous text width calculation was an estimation. Using fixed offsets.
            drawBitmapText(centerX - 35.0f, centerY + 20.0f, GLUT_BITMAP_TIMES_ROMAN_24, gameOverStr, 1.0f, 0.2f, 0.2f);

            std::string finalScoreMsg = "Final Score: " + std::to_string(currentInstance->player.score);
            drawBitmapText(centerX - 30.0f, centerY + 0.0f, GLUT_BITMAP_9_BY_15, finalScoreMsg, 1.0f, 1.0f, 1.0f);

            std::string restartMsg = "Press R to Restart";
            drawBitmapText(centerX - 30.0f, centerY - 20.0f, GLUT_BITMAP_9_BY_15, restartMsg, 1.0f, 1.0f, 1.0f);

            std::string menuMsg = "Press ESC for Menu";
            drawBitmapText(centerX - 30.0f, centerY - 35.0f, GLUT_BITMAP_9_BY_15, menuMsg, 1.0f, 1.0f, 1.0f);

        } else if (currentInstance->currentState == STATE_PAUSED) {
            std::string pauseMsg = "PAUSED";
            // Simplified centering for PAUSED message
            float centerX = coordenadasMundo[0] + (coordenadasMundo[1] - coordenadasMundo[0]) / 2.0f;
            float centerY = coordenadasMundo[2] + (coordenadasMundo[3] - coordenadasMundo[2]) / 2.0f;
            drawBitmapText(centerX - 15.0f, // Adjust X as needed for "PAUSED"
                           centerY + 10.0f,
                           GLUT_BITMAP_TIMES_ROMAN_24, pauseMsg, 1.0f, 1.0f, 0.0f);
                           centerY + 10.0f,
                           GLUT_BITMAP_TIMES_ROMAN_24, pauseMsg, 1.0f, 1.0f, 0.0f);

            std::string resumeMsg = "Press P to Resume";
            // Simplified centering for resume message
            drawBitmapText(centerX - 22.0f, // Adjust X as needed for "Press P to Resume"
                           centerY - 5.0f,
                           GLUT_BITMAP_9_BY_15, resumeMsg, 0.9f, 0.9f, 0.9f);
        } else if (currentInstance->currentState == STATE_BETWEEN_WAVES) {
            std::string waveClearedMsg = "WAVE " + std::to_string(currentInstance->currentWave -1) + " CLEARED!";
             float waveClearedWidth = 0; for(char ch : waveClearedMsg) { waveClearedWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, ch); }
            float approxWaveClearedWidth = waveClearedWidth / 20.0f; // GUESS
            drawBitmapText((coordenadasMundo[0] + coordenadasMundo[1] - approxWaveClearedWidth) / 2.0f,
                           (coordenadasMundo[2] + coordenadasMundo[3]) / 2.0f + 15.0f,
                           GLUT_BITMAP_TIMES_ROMAN_24, waveClearedMsg, 0.8f, 0.8f, 1.0f); // Light blue

            std::string nextWaveMsg = "Prepare for Wave " + std::to_string(currentInstance->currentWave);
            float nextWaveWidth = 0; for(char ch : nextWaveMsg) { nextWaveWidth += glutBitmapWidth(GLUT_BITMAP_9_BY_15, ch); }
            float approxNextWaveWidth = nextWaveWidth / 15.0f; // GUESS
            drawBitmapText((coordenadasMundo[0] + coordenadasMundo[1] - approxNextWaveWidth) / 2.0f,
                           (coordenadasMundo[2] + coordenadasMundo[3]) / 2.0f - 5.0f,
                           GLUT_BITMAP_9_BY_15, nextWaveMsg, 1.0f, 1.0f, 1.0f); // White
        }
    }
    glutSwapBuffers();
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

    // Global Pause/Resume Check (available in PLAYING or PAUSED states)
    if (key == 'p' || key == 'P') {
        if (currentInstance->currentState == STATE_PLAYING) {
            currentInstance->currentState = STATE_PAUSED;
            // std::cout << "Game Paused." << std::endl;
            glutPostRedisplay(); // Request redraw to show PAUSED message
            return;
        } else if (currentInstance->currentState == STATE_PAUSED) {
            currentInstance->currentState = STATE_PLAYING;
            // Recalculate lastFrameTime to prevent a large deltaTime jump after unpausing
            currentInstance->lastFrameTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
            // std::cout << "Game Resumed." << std::endl;
            glutPostRedisplay();
            return;
        }
    }

    // Menu State Input Handling
    if (currentInstance->currentState == STATE_MENU) {
        switch (key) {
            case 's':
            case 'S':
                currentInstance->resetGame(); // This will set up wave 1
                currentInstance->currentState = STATE_PLAYING;
                // std::cout << "State changed to PLAYING from MENU" << std::endl;
                break;
            case 'q':
            case 'Q':
            // Fall through for ESC
            case 27: // ESC key
                exit(0);
                break;
        }
        glutPostRedisplay(); // Request redraw to show menu or game screen
        return; // Consume key press, don't process further for other states
    }

    // Playing State Input Handling (WASDQE, Space)
    if (currentInstance->currentState == STATE_PLAYING) {
        switch (key) {
            case 'a': case 'A': currentInstance->player.move(-1.0f, 0.0f); break;
            case 'd': case 'D': currentInstance->player.move(1.0f, 0.0f); break;
            case 'w': case 'W': currentInstance->player.move(0.0f, 1.0f); break;
            case 's': case 'S': currentInstance->player.move(0.0f, -1.0f); break;
            case 'q': case 'Q': currentInstance->player.rotate(90.0f); break;
            case 'e': case 'E': currentInstance->player.rotate(-90.0f); break;
            case ' ': currentInstance->player.shoot(); break;
            case 27: // ESC key can have different meaning in PLAYING vs MENU (e.g. pause)
                     // For now, let it be quit globally if not handled by other states.
                     // Or, it could bring up a PAUSE menu.
                exit(0); // Simple quit for now from playing state too
                break;
        }
    }

    // Game Over State Input Handling (Restart)
    if (currentInstance->currentState == STATE_GAME_OVER) {
        if (key == 'r' || key == 'R') {
            currentInstance->resetGame();
            currentInstance->currentState = STATE_PLAYING;
            // std::cout << "Game Restarted!" << std::endl;
            glutPostRedisplay(); // Added to ensure screen updates after state change
        } else if (key == 27) { // ESC key for Menu
            currentInstance->currentState = STATE_MENU;
            // std::cout << "Returning to Menu from Game Over." << std::endl;
            glutPostRedisplay();
        } else if (key == 'q' || key == 'Q') { // Q still quits directly
             exit(0);
        }
    }
    // glutPostRedisplay(); // Usually called in idle or after state change
}

void Game::idleCallback() {
    if (!currentInstance) return;

    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Time in seconds
    float deltaTime = currentTime - currentInstance->lastFrameTime;
    currentInstance->lastFrameTime = currentTime;

    // Cap deltaTime to prevent large jumps (e.g., during debugging pauses)
    if (deltaTime > 0.1f) {
        deltaTime = 0.1f;
    }

    if (currentInstance->currentState == STATE_PLAYING) {
        // Player projectile updates
        currentInstance->player.updateProjectiles(deltaTime);

        // Collision Detection Logic (Player Projectile vs Invader)
        for (auto& proj : currentInstance->player.projectiles) {
            if (!proj.active) continue;
            for (auto& inv : currentInstance->invaders) {
                if (!inv.active) continue;
                float projLeft = proj.x - proj.width / 2.0f;
                float projRight = proj.x + proj.width / 2.0f;
                float projTop = proj.y + proj.height / 2.0f;
                float projBottom = proj.y - proj.height / 2.0f;
                float invHalfSize = invaderTamanho / 2.0f;
                float invLeft = inv.x - invHalfSize;
                float invRight = inv.x + invHalfSize;
                float invTop = inv.y + invHalfSize;
                float invBottom = inv.y - invHalfSize;
                if (projRight > invLeft && projLeft < invRight && projTop > invBottom && projBottom < invTop) {
                    proj.active = false;
                    inv.active = false;
                    currentInstance->player.score += inv.pointsValue;
                    break;
                }
            }
        }

        // Invader Movement Logic
        currentInstance->invaderMoveTimer -= deltaTime;
        if (currentInstance->invaderMoveTimer <= 0.0f) {
            currentInstance->invaderMoveTimer += currentInstance->invaderMoveInterval;
            bool boundaryHit = false;
            for (const auto& inv : currentInstance->invaders) {
                if (!inv.active) continue;
                float nextX = inv.x + (currentInstance->invaderDirection * currentInstance->invaderStepSize);
                float invHalfSize = invaderTamanho / 2.0f;
                if ((currentInstance->invaderDirection == 1 && (nextX + invHalfSize > coordenadasMundo[1])) ||
                    (currentInstance->invaderDirection == -1 && (nextX - invHalfSize < coordenadasMundo[0]))) {
                    boundaryHit = true;
                    break;
                }
            }
            currentInstance->invadersShouldMoveDown = false;
            if (boundaryHit) {
                currentInstance->invadersShouldMoveDown = true;
                currentInstance->invaderDirection *= -1;
            }
            for (auto& inv : currentInstance->invaders) {
                if (!inv.active) continue;
                if (currentInstance->invadersShouldMoveDown) {
                    inv.y -= currentInstance->invaderStepSize;
                } else {
                    inv.x += (currentInstance->invaderDirection * currentInstance->invaderStepSize);
                }
            }
        }

        // Invader Shooting Logic
        currentInstance->invaderShootTimer -= deltaTime;
        if (currentInstance->invaderShootTimer <= 0.0f) {
            std::vector<Invader*> potentialShooters;
            for (auto& inv : currentInstance->invaders) {
                if (inv.active) potentialShooters.push_back(&inv);
            }
            if (!potentialShooters.empty()) {
                Invader* shooter = potentialShooters[rand() % potentialShooters.size()];
                currentInstance->invaderProjectiles.emplace_back(
                    shooter->x, shooter->y - invaderTamanho / 2.0f - 1.0f, // Spawn point
                    0.0f, // vx = 0 for straight down
                    currentInstance->invaderProjectileSpeed, // vy (negative for down)
                    false // isPlayerProjectile = false
                );
            }
            float shootRange = currentInstance->invaderShootIntervalMax - currentInstance->invaderShootIntervalMin;
            if (shootRange < 0.0f) shootRange = 0.0f;
            currentInstance->invaderShootTimer = (static_cast<float>(rand()) / RAND_MAX * shootRange) + currentInstance->invaderShootIntervalMin;
        }

        // Collision Detection: Invader Projectiles vs Player
        if (currentInstance->player.lives > 0) {
            // extern GLfloat naveJogadorTamanho; // Already available
            float playerHalfWidth = naveJogadorTamanho / 2.0f;
            float playerHalfHeight = naveJogadorTamanho / 2.0f;
            float playerLeft = currentInstance->player.x - playerHalfWidth;
            float playerRight = currentInstance->player.x + playerHalfWidth;
            float playerTop = currentInstance->player.y + playerHalfHeight;
            float playerBottom = currentInstance->player.y - playerHalfHeight;
            for (auto& proj : currentInstance->invaderProjectiles) {
                if (!proj.active) continue;
                float projLeft = proj.x - proj.width / 2.0f;
                float projRight = proj.x + proj.width / 2.0f;
                float projTop = proj.y + proj.height / 2.0f;
                float projBottom = proj.y - proj.height / 2.0f;
                if (projRight > playerLeft && projLeft < playerRight && projTop > playerBottom && projBottom < playerTop) {
                    currentInstance->player.takeHit();
                    proj.active = false;
                    if (currentInstance->player.lives <= 0) {
                        currentInstance->currentState = STATE_GAME_OVER;
                    }
                    break;
                }
            }
        }

        // Wave Progression Check
        bool activeInvadersFound = false;
        if (!currentInstance->invaders.empty()) { // Only check if the wave was populated
            for (const auto& inv : currentInstance->invaders) {
                if (inv.active) {
                    activeInvadersFound = true;
                    break;
                }
            }
            if (!activeInvadersFound) { // All invaders from current wave are inactive
                currentInstance->currentWave++;
                // std::cout << "Wave " << currentInstance->currentWave -1 << " cleared! Starting Wave " << currentInstance->currentWave << std::endl;
                currentInstance->spawnInvadersForWave(currentInstance->currentWave);
                currentInstance->player.projectiles.clear(); // Clear player projectiles
            }
        } else if (currentInstance->currentWave > 0) {
            // This case handles if invaders list becomes empty somehow after starting (e.g. if they were removed instead of inactivated)
            // Or if spawnInvadersForWave was called and it cleared the list, but this is checked before it repopulates.
            // To be safe, if list is empty and we think a wave was in progress, start next wave.
            // This might be redundant with the above check if invaders are only ever set inactive.
            // std::cout << "Invaders list empty, potentially advancing wave." << std::endl;
            // currentInstance->currentWave++;
            // currentInstance->spawnInvadersForWave(currentInstance->currentWave);
            // currentInstance->player.projectiles.clear();
        }
    } // End of STATE_PLAYING block
    else if (currentInstance->currentState == STATE_BETWEEN_WAVES) {
        currentInstance->betweenWaveTimer -= deltaTime;
        if (currentInstance->betweenWaveTimer <= 0.0f) {
            currentInstance->spawnInvadersForWave(currentInstance->currentWave); // currentWave is already the *next* wave number
            currentInstance->currentState = STATE_PLAYING;
            // Reset lastFrameTime to prevent deltaTime jump after the pause-like state
            currentInstance->lastFrameTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
            // std::cout << "Starting Wave " << currentInstance->currentWave << std::endl;
        }
    } // End of STATE_BETWEEN_WAVES block

    // Update and cleanup invader projectiles regardless of game state (unless menu)
    if (currentInstance->currentState != STATE_MENU) {
        for (auto& p : currentInstance->invaderProjectiles) {
            if (p.active) {
                p.update(deltaTime);
            }
        }
        currentInstance->invaderProjectiles.erase(
            std::remove_if(currentInstance->invaderProjectiles.begin(),
                            currentInstance->invaderProjectiles.end(),
                            [](const Projectile& p){ return !p.active; }),
            currentInstance->invaderProjectiles.end()
        );
    }

    glutPostRedisplay(); // Request redraw to show changes
}
