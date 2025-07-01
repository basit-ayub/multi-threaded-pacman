#include<pthread.h>
#include<SFML/Graphics.h>
#include<semaphore.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



// pacman thingies
int directX = 1, directY = 0, pacmanX = 495, pacmanY = 465, pacmanEat = 0, score = 0, lives = 3;
// ghost thingies
int ghostRedX = 465, ghostRedY = 405, ghostBlueX = 525, ghostBlueY = 405, ghostMove = 0, ghostMove1 = 0, ghostMove2 = 0, ghostMove3 = 0;
int ghostYellow1X = 465, ghostYellow1Y = 375, ghostYellow2X = 525, ghostYellow2Y = 375, ghostYellow1Steps = 0, ghostYellow2Steps = 0;
int ghostYellow1DirectX = 0, ghostYellow1DirectY = 0, ghostYellow2DirectY = 0, ghostYellow2DirectX = 0, ghostRedSpeed = 1;
int ghostBlueSteps = 0, ghostRedSteps = 0, ghostBlueDirectX = 0, ghostBlueDirectY = 0, ghostRedDirectX = 0, ghostRedDirectY = 0;
bool ghostEat = false, gameReset = false, ghost1Reset = false, ghost2Reset = false, ghost3Reset = false, ghost4Reset = false, pacmanReset = false;;
char keyPressed = ' ';
int pacmanMove = 0;
//sfVideoMode mode;
sfRenderWindow* window;
sfEvent event;
pthread_mutex_t scoreLock;
sem_t keySemaphore, moveSemaphore, palletSemaphore, ghost1Semaphore, ghost2Semaphore, collision1Semaphore, collision2Semaphore, collision3Semaphore, collision4Semaphore;
sem_t collision1Semaphore1, collision2Semaphore1, collision3Semaphore1, collision4Semaphore1;
int grid[25][33] = {    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                        {-1, 2, 1, 1, 1, 1, 1, 1, 1,-1, 1, 1, 1, 1, 1, 1,-1, 1, 1, 1, 1, 1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 2,-1},
                        {-1, 1,-1,-1,-1,-1, 1,-1, 1,-1, 1,-1, 1,-1,-1, 1,-1, 1,-1,-1, 1,-1, 1,-1, 1,-1, 1,-1,-1,-1,-1, 1,-1},
                        {-1, 1, 1, 1, 1, 1, 1,-1, 1, 1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1, 1, 1, 1,-1, 1, 1, 1, 1, 1, 1,-1},
                        {-1, 1,-1,-1, 1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1,-1, 1,-1,-1, 1,-1},
                        {-1, 1,-1,-1, 1, 1, 1,-1, 1, 1, 1,-1, 1, 1, 1,-1,-1,-1, 1, 1, 1,-1, 1, 1, 1,-1, 1, 1, 1,-1,-1, 1,-1},
                        {-1, 1, 1, 1, 1,-1, 1,-1, 1,-1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1, 1,-1, 1,-1, 1,-1, 1, 1, 1, 1,-1},
                        {-1, 1,-1,-1, 1,-1, 1,-1, 1,-1, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1, 1, 1, 1,-1, 1,-1, 1,-1, 1,-1,-1, 1,-1},
                        {-1, 1, 1, 1, 1,-1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1, 1,-1, 1, 1, 1, 1,-1},
                        {-1,-1,-1,-1, 1,-1, 1,-1, 1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 0, 0,-1,-1,-1,-1, 1,-1, 1,-1, 1,-1,-1,-1,-1},
                        {-1,-1,-1,-1, 1,-1, 1, 1, 1,-1, 1, 1, 1, 1,-1, 0, 0, 0,-1, 1, 1, 1, 1,-1, 1, 1, 1,-1, 1,-1,-1,-1,-1},
                        {-1,-1,-1,-1, 1,-1, 1,-1,-1,-1, 1,-1,-1, 1,-1, 0, 0, 0,-1, 1,-1,-1, 1,-1,-1,-1, 1,-1, 1,-1,-1,-1,-1},
                        { 0, 0, 0, 0, 1, 3, 1, 1, 1, 1, 1,-1,-1, 1,-1, 0, 0, 0,-1, 1,-1,-1, 1, 1, 1, 1, 1, 3, 1, 0, 0, 0, 0},
                        {-1,-1,-1,-1, 1,-1, 1,-1,-1,-1, 1,-1,-1, 1,-1, 0, 0, 0,-1, 1,-1,-1, 1,-1,-1,-1, 1,-1, 1,-1,-1,-1,-1},
                        {-1,-1,-1,-1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,-1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1,-1, 1,-1,-1,-1,-1},
                        {-1,-1,-1,-1, 1,-1, 1,-1,-1,-1,-1,-1, 1, 1, 0, 0, 0, 0, 0, 1, 1,-1,-1,-1,-1,-1, 1,-1, 1,-1,-1,-1,-1},
                        {-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1, 1,-1,-1,-1,-1,-1,-1,-1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1},
                        {-1, 1,-1,-1,-1, 1,-1,-1,-1,-1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1, 1,-1,-1,-1,-1, 1,-1,-1,-1, 1,-1},
                        {-1, 1,-1,-1,-1, 1,-1,-1,-1,-1, 1,-1, 1,-1,-1,-1,-1,-1,-1,-1, 1,-1, 1,-1,-1,-1,-1, 1,-1,-1,-1, 1,-1},
                        {-1, 1, 1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1, 1, 1, 1,-1},
                        {-1,-1,-1, 1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,-1, 1,-1, 1,-1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1, 1,-1,-1,-1},
                        {-1, 1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1, 1,-1},
                        {-1, 1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1, 1,-1,-1,-1,-1,-1,-1,-1, 1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1},
                        {-1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,-1},
                        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} };


// ------------------------------------------ROUNDED RECTANGLE -------------------------------------------------------

void drawRoundedRectangle(sfRenderWindow* window, sfVector2f position, sfVector2f size, float radius, sfColor color) {
    // Center rectangle
    sfRectangleShape* center = sfRectangleShape_create();
    sfRectangleShape_setSize(center, (sfVector2f) { size.x - 2 * radius, size.y - 2 * radius });
    sfRectangleShape_setPosition(center, (sfVector2f) { position.x + radius, position.y + radius });
    sfRectangleShape_setFillColor(center, color);
    sfRenderWindow_drawRectangleShape(window, center, NULL);

    // Side rectangles
    sfRectangleShape* side = sfRectangleShape_create();
    sfRectangleShape_setFillColor(side, color);

    // Top
    sfRectangleShape_setSize(side, (sfVector2f) { size.x - 2 * radius, radius });
    sfRectangleShape_setPosition(side, (sfVector2f) { position.x + radius, position.y });
    sfRenderWindow_drawRectangleShape(window, side, NULL);

    // Bottom
    sfRectangleShape_setPosition(side, (sfVector2f) { position.x + radius, position.y + size.y - radius });
    sfRenderWindow_drawRectangleShape(window, side, NULL);

    // Left
    sfRectangleShape_setSize(side, (sfVector2f) { radius, size.y - 2 * radius });
    sfRectangleShape_setPosition(side, (sfVector2f) { position.x, position.y + radius });
    sfRenderWindow_drawRectangleShape(window, side, NULL);

    // Right
    sfRectangleShape_setPosition(side, (sfVector2f) { position.x + size.x - radius, position.y + radius });
    sfRenderWindow_drawRectangleShape(window, side, NULL);

    // Circles for corners
    sfCircleShape* circle = sfCircleShape_create();
    sfCircleShape_setRadius(circle, radius);
    sfCircleShape_setPointCount(circle, 30); // smoother circle
    sfCircleShape_setFillColor(circle, color);

    // Top-left
    sfCircleShape_setPosition(circle, position);
    sfRenderWindow_drawCircleShape(window, circle, NULL);

    // Top-right
    sfCircleShape_setPosition(circle, (sfVector2f) { position.x + size.x - radius * 2, position.y });
    sfRenderWindow_drawCircleShape(window, circle, NULL);

    // Bottom-left
    sfCircleShape_setPosition(circle, (sfVector2f) { position.x, position.y + size.y - radius * 2 });
    sfRenderWindow_drawCircleShape(window, circle, NULL);

    // Bottom-right
    sfCircleShape_setPosition(circle, (sfVector2f) { position.x + size.x - radius * 2, position.y + size.y - radius * 2 });
    sfRenderWindow_drawCircleShape(window, circle, NULL);

    // Cleanup
    sfRectangleShape_destroy(center);
    sfRectangleShape_destroy(side);
    sfCircleShape_destroy(circle);
}



//------------------------------------------------ MENU -------------------------------------------------
//For startup window
#define FRAME_WIDTH 450
#define FRAME_HEIGHT 500
#define TOTAL_FRAMES 416
#define FPS 30
#define FOLDER "intro/"

//Players Name
char PLAYERNAME[32];

//picture names for mainmenu
const char* menuImages[4] =
{
    "Resources/startGame.png",
    "Resources/instructions.png",
    "Resources/highscores.png",
    "Resources/credits.png"
};

void loadingSequence(sfRenderWindow*);
void showPage(sfRenderWindow*, const char*);
void showHighScores(sfRenderWindow*, const char*);
void showMainMenu(sfRenderWindow*);
void userInput(sfRenderWindow*);


void startup()
{
    sfVideoMode mode = { FRAME_WIDTH, FRAME_HEIGHT, 32 };
    sfRenderWindow* window = sfRenderWindow_create(mode, NULL, sfNone, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create window.\n");
        return;
    }
    sfVector2i position = { 700, 300 };
    sfRenderWindow_setPosition(window, position);
    //Load the starting vid
    loadingSequence(window);
    //Load the menu
    showMainMenu(window);
    //Destroy Window
    sfRenderWindow_destroy(window);
    return;

}
void loadingSequence(sfRenderWindow* window)
{
    sfTexture* textures[TOTAL_FRAMES];
    char filename[64];

    for (int i = 0; i < TOTAL_FRAMES; ++i)
    {
        snprintf(filename, sizeof(filename), FOLDER "%04d.bmp", i);
        textures[i] = sfTexture_createFromFile(filename, NULL);
        if (!textures[i])
        {
            fprintf(stderr, "Failed to load: %s\n", filename);
            return;
        }
    }

    sfSprite* sprite = sfSprite_create();
    sfClock* clock = sfClock_create();
    int currentFrame = 0;

    while (sfRenderWindow_isOpen(window) && currentFrame < TOTAL_FRAMES)
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        if (sfClock_getElapsedTime(clock).microseconds >= (1000000 / FPS))
        {
            sfSprite_setTexture(sprite, textures[currentFrame], sfTrue);
            currentFrame++;
            sfClock_restart(clock);
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_display(window);


    }

    for (int i = 0; i < TOTAL_FRAMES; ++i)
        sfTexture_destroy(textures[i]);

    sfSprite_destroy(sprite);
    sfClock_destroy(clock);
    return;
}
void showPage(sfRenderWindow* window, const char* imagePath)
{
    sfTexture* texture = sfTexture_createFromFile(imagePath, NULL);
    if (!texture)
    {
        fprintf(stderr, "Failed to load %s\n", imagePath);
        return;
    }

    sfSprite* sprite = sfSprite_create();
    sfSprite_setTexture(sprite, texture, sfTrue);

    bool wait = true;
    while (sfRenderWindow_isOpen(window) && wait)
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
            }
            else if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape)
            {
                wait = false; // Go back to menu
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_display(window);
    }

    sfSprite_destroy(sprite);
    sfTexture_destroy(texture);
}
void showHighScores(sfRenderWindow* window, const char* imagePath)
{
    sfTexture* texture = sfTexture_createFromFile(imagePath, NULL);
    if (!texture)
    {
        fprintf(stderr, "Failed to load %s\n", imagePath);
        return;
    }

    sfSprite* sprite = sfSprite_create();
    sfSprite_setTexture(sprite, texture, sfTrue);

    bool wait = true;
    while (sfRenderWindow_isOpen(window) && wait)
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
            }
            else if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape)
            {
                wait = false; // Go back to menu
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_display(window);
    }

    sfSprite_destroy(sprite);
    sfTexture_destroy(texture);
}
void showMainMenu(sfRenderWindow* window)
{
    int currentOption = 0;
    sfTexture* texture = NULL;
    sfSprite* sprite = sfSprite_create();

    texture = sfTexture_createFromFile(menuImages[currentOption], NULL);
    sfSprite_setTexture(sprite, texture, sfTrue);

    bool selected = false;

    while (sfRenderWindow_isOpen(window) && !selected)
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
            }
            else if (event.type == sfEvtKeyPressed)
            {
                if (event.key.code == sfKeyUp)
                {
                    currentOption = (currentOption - 1 + 4) % 4;

                    sfTexture_destroy(texture);
                    texture = sfTexture_createFromFile(menuImages[currentOption], NULL);
                    if (texture)
                        sfSprite_setTexture(sprite, texture, sfTrue);
                }
                else if (event.key.code == sfKeyDown)
                {
                    currentOption = (currentOption + 1) % 4;

                    sfTexture_destroy(texture);
                    texture = sfTexture_createFromFile(menuImages[currentOption], NULL);
                    if (texture)
                        sfSprite_setTexture(sprite, texture, sfTrue);
                }
                else if (event.key.code == sfKeyReturn)
                {
                    switch (currentOption)
                    {
                    case 0: // Start Game
                    {
                        selected = true;

                        userInput(window);
                        return;
                    }
                    case 1: // Instructions
                        showPage(window, "Resources/instructionsPage.png");
                        break;
                    case 2: // Highscores
                        showHighScores(window, "Resources/highScorePage.png");
                        break;
                    case 3: // Credits
                        showPage(window, "Resources/creditsPage.png");
                        break;
                    }
                    // Refresh menu image after returning from sub-page
                    sfTexture_destroy(texture);
                    texture = sfTexture_createFromFile(menuImages[currentOption], NULL);
                    if (texture)
                        sfSprite_setTexture(sprite, texture, sfTrue);
                }
            }
        }
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_display(window);
    }
    sfSprite_destroy(sprite);
    sfTexture_destroy(texture);
    return;
}
void userInput(sfRenderWindow* window)
{
    //Take User Input
    sfTexture* inputTexture = sfTexture_createFromFile("Resources/userInput.png", NULL);
    if (!inputTexture)
    {
        fprintf(stderr, "Failed to load userInput.png\n");
        return;
    }

    sfSprite* sprite2 = sfSprite_create();
    sfSprite_setTexture(sprite2, inputTexture, sfTrue);

    sfFont* font = sfFont_createFromFile("Fonts/ARIALNB.TTF");
    if (!font)
    {
        fprintf(stderr, "Failed to load font.\n");
        return;
    }

    sfText* text = sfText_create();
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 30);
    sfText_setPosition(text, (sfVector2f) { 50, 285 });
    sfText_setFillColor(text, sfYellow);

    char playerName[32] = "";
    int length = 0;
    bool done = false;

    sfEvent dump;
    while (sfRenderWindow_pollEvent(window, &dump)) {}

    while (sfRenderWindow_isOpen(window) && !done)
    {
        sfEvent event2;
        while (sfRenderWindow_pollEvent(window, &event2))
        {
            if (event2.type == sfEvtClosed)
                sfRenderWindow_close(window);
            else if (event2.type == sfEvtTextEntered)
            {
                if (event2.text.unicode == '\b' && length > 0)
                {
                    playerName[--length] = '\0';
                }
                else if (event2.text.unicode >= 32 && event2.text.unicode <= 126 && length < 31)
                {
                    playerName[length++] = (char)event2.text.unicode;
                    playerName[length] = '\0';
                }
                else if (event2.text.unicode == '\r' || event2.text.unicode == '\n')  // Enter key
                {
                    for (int i = 0; i < 32; i++)
                        PLAYERNAME[i] = playerName[i];
                    done = true;
                }
                sfText_setString(text, playerName);
            }
        }
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite2, NULL);
        sfRenderWindow_drawText(window, text, NULL);
        sfRenderWindow_display(window);
    }
    //Free Memory
    sfSprite_destroy(sprite2);
    sfTexture_destroy(inputTexture);
    sfText_destroy(text);
    sfFont_destroy(font);
}
//----------------------------------------------- GAME ---------------------------------------------------


void* timer(void* args) {
    sfTime sleepDuration = sfMilliseconds(10000);
    sfSleep(sleepDuration);

    ghostEat = false;
}
void* timer2(void* args) {
    sfTime sleepDuration = sfMilliseconds(10000);
    sfSleep(sleepDuration);
    ghostRedSpeed = 1;

}
void * gateTimer(void * args){
    sfTime sleepDuration = sfMilliseconds(10000);
    sfSleep(sleepDuration);
    grid[10][15] = 0;
    grid[10][16] = 0;
    grid[10][17] = 0;
}


//int directX = 1, directY = 0, pacmanX = 495, pacmanY = 465, pacmanEat = 0, score = 0, lives = 3;
// ghost thingies
//int ghostRedX = 465, ghostRedY = 405, ghostBlueX = 525, ghostBlueY = 405, ghostMove = 0, ghostMove1 = 0, ghostMove2 = 0, ghostMove3 = 0;
//int ghostYellow1X = 465, ghostYellow1Y = 375, ghostYellow2X = 525, ghostYellow2Y = 375, ghostYellow1Steps = 0, ghostYellow2Steps = 0;

//------------------------------------------------------------- SEMAPHORE 1 --------------------------------------------------------------
void* pallets(void* args) {
    sem_wait(&palletSemaphore);
    if (grid[((pacmanY - 15) / 30)][((pacmanX - 15) / 30)] == 1) {
        score += 2 *grid[((pacmanY - 15) / 30)][((pacmanX - 15) / 30)];
        grid[((pacmanY - 15) / 30)][((pacmanX - 15) / 30)] = 0;
    }
    else if (grid[((pacmanY - 15) / 30)][((pacmanX - 15) / 30)] == 2 && !ghostEat)
    {
        ghostEat = true;
        pthread_t timerT;
        pthread_create(&timerT, NULL, timer, NULL);
        score += 2 * grid[((pacmanY - 15) / 30)][((pacmanX - 15) / 30)];
        grid[((pacmanY - 15) / 30)][((pacmanX - 15) / 30)] = 0;
    }
    else if (grid[((ghostRedY - 15) / 30)][((ghostRedX - 15) / 30)] == 3 && ghostRedSpeed == 1) {
        ghostRedSpeed = 2;
        pthread_t timerT;
        pthread_create(&timerT, NULL, timer2, NULL);
        grid[((ghostRedY - 15) / 30)][((ghostRedX - 15) / 30)] = 0;
    
    }
    return 0;
}

void* userInterface(void * args) {
    if (event.type == sfEvtClosed) {
        sfRenderWindow_close(window);
    }
    if (event.type == sfEvtKeyPressed) {
        if (event.key.code == sfKeyUp ) {
            keyPressed = 'w';
        }
        else if (event.key.code == sfKeyDown) {
            keyPressed = 's';
        }
        else if (event.key.code == sfKeyLeft) {
            keyPressed = 'a';
        }
        else if (event.key.code == sfKeyRight) {
            keyPressed = 'd';
        }
    }
    if (event.type == sfEvtKeyPressed) {
        sem_wait(&keySemaphore);
        if (keyPressed == 'w' && grid[((pacmanY - 15) / 30)  - 1][((pacmanX - 15) / 30)] != -1) {
            directY = -1; directX = 0;
            keyPressed = 'w';
        }
        else if (keyPressed == 's' && grid[((pacmanY - 15) / 30) + 1][((pacmanX - 15) / 30)] != -1) {
            directY = 1; directX = 0;
            keyPressed = 's';
        }
        else if (keyPressed == 'a' && grid[((pacmanY - 15) / 30)][((pacmanX - 15) / 30) - 1] != -1) {
            directX = -1; directY = 0;
            keyPressed = 'a';
        }
        else if (keyPressed == 'd' && grid[((pacmanY - 15) / 30)][((pacmanX - 15) / 30) + 1] != -1) {
            directX = 1; directY = 0;
            keyPressed = 'd';
        }
        sem_post(&moveSemaphore);
    }

    return 0;
}

void* pacman(void * args) {

    if (gameReset) {
        pacmanReset = true;
        return NULL;
    }
    sem_wait(&moveSemaphore);
    if (pacmanX == 15 && pacmanY == 375 && directX == -1)
    {
        pacmanX = 975;
    }
    else if (pacmanX == 975 && pacmanY == 375 && directX == 1) {
        pacmanX = 15;
    }

    sfTime sleepDuration = sfMilliseconds(100);
    if (grid[((pacmanY - 15) / 30) + directY ][((pacmanX - 15) / 30) + directX] != -1)
    {
        //for (int i = 0; i < 30; i++)
        //{
            if (gameReset) {
                pacmanReset = true;
                return NULL;
            }
            sfSleep(sleepDuration);
            sem_wait(&ghost1Semaphore);
            sem_wait(&ghost2Semaphore);
            pacmanX += directX * 30;
            pacmanY += directY * 30;
            sem_post(&ghost1Semaphore);
            sem_post(&ghost2Semaphore);
        //}
    }
    sem_post(&keySemaphore);
    sem_post(&palletSemaphore);
    if (gameReset) {
        pacmanReset = true;
        return NULL;
    }
    return 0;
}


// Structure to hold position data for pathfinding
typedef struct {
    int x;
    int y;
} Position;

// Helper function to check if position is valid
int isValid(int x, int y) {
    return x >= 0 && x < 33 && y >= 0 && y < 25 && grid[y][x] != -1;
}
// ---------------------------------------- GHOST RED ---------------------------------------------

// finding the shortest path between the ghost's current location and pacman's current location
void * calculateGhostMove1(void * args) {
    // converting the coordinates from pixel to grid array
    if (gameReset) {
        ghost1Reset = true;
        return NULL;
    }
    if (ghostEat)
    {
        if (grid[((ghostRedY - 15) / 30) + ghostRedDirectY][((ghostRedX - 15) / 30) + ghostRedDirectX] != -1) {
            sfTime sleepDuration = sfMilliseconds(5);
            int temp = ghostRedSpeed;
            //for (int i = 0; i < 30 / temp; i++)
            //{
                if (gameReset) {
                    ghost1Reset = true;
                    return NULL;
                }
                sem_wait(&collision1Semaphore);
                sfSleep(sleepDuration);
                ghostRedX += ghostRedDirectX * 30;
                ghostRedY += ghostRedDirectY * 30;
                sem_post(&collision1Semaphore);
            //}
            ghostRedSteps--;
        }
        else
        {
            ghostRedSteps = 0;
        }
    }
    else
    {
        int ghostGridX = (ghostRedX - 15) / 30;
        int ghostGridY = (ghostRedY - 15) / 30;
        sem_wait(&ghost1Semaphore);
        int pacmanGridX = (pacmanX - 15) / 30;
        int pacmanGridY = (pacmanY - 15) / 30;
        sem_post(&ghost1Semaphore);

        // If ghost is already at pacman's position then we return because no movement needed
        if (ghostGridX == pacmanGridX && ghostGridY == pacmanGridY) {
            return NULL;
        }

        // BFS setup
        Position parent[25][33];
        int visited[25][33] = { 0 };
        Position queue[25 * 33];
        int front = 0, rear = 0;

        // Start from ghost's position
        queue[rear++] = (Position){ ghostGridX, ghostGridY };
        visited[ghostGridY][ghostGridX] = 1;

        // Possible movement directions (left, right, up, down)
        int dx[] = { -1, 1, 0, 0 };
        int dy[] = { 0, 0, -1, 1 };

        Position current;
        int found = 0;

        // BFS loop
        while (front < rear && !found) {
            current = queue[front++];

            for (int i = 0; i < 4; i++) {
                int newX = current.x + dx[i];
                int newY = current.y + dy[i];

                if (isValid(newX, newY)) {
                    if (!visited[newY][newX]) {
                        parent[newY][newX] = current;
                        visited[newY][newX] = 1;
                        queue[rear++] = (Position){ newX, newY };

                        // If we reached pacman, backtrack to find first move
                        if (newX == pacmanGridX && newY == pacmanGridY) {
                            found = 1;
                            break;
                        }
                    }
                }
            }
        }
        // If path found, determine first move
        if (found) {
            Position target = { pacmanGridX, pacmanGridY };
            Position next;

            // Backtrack to find the first move from ghost's position
            while (!(parent[target.y][target.x].x == ghostGridX &&
                parent[target.y][target.x].y == ghostGridY)) {
                target = parent[target.y][target.x];
            }

            next = target;

            // Calculate direction based on first move
            int dirX = next.x - ghostGridX;
            int dirY = next.y - ghostGridY;
            sfTime sleepDuration = sfMilliseconds(5);
            // Move ghost in that direction
            int temp = ghostRedSpeed;
            //for (int i = 0; i < 30 / temp; i++)
            //{
                if (gameReset) {
                    ghost1Reset = true;
                    return NULL;
                }
                sem_wait(&collision1Semaphore);
                sfSleep(sleepDuration);
                ghostRedX += dirX * 30;
                ghostRedY += dirY * 30;
                sem_post(&collision1Semaphore);
            //}
        }
    }
    if ((((ghostRedY - 15) / 30) == 10) && ((((ghostRedX - 15) / 30) == 16) || (((ghostRedX - 15) / 30) == 15) || (((ghostRedX - 15) / 30) == 17)))
    {
        grid[10][15] = -1;
        grid[10][16] = -1;
        grid[10][17] = -1;
    }
    if (gameReset) {
        ghost1Reset = true;
        return NULL;
    }
}
//---------------------------------------- GHOST BLUE --------------------------------------------------------
void* calculateGhostMove2(void* args) {
    // converting the coordinates from pixel to grid array
    if (gameReset) {
        ghost2Reset = true;
        return NULL;
    }
    if (ghostEat)
    {
        if (grid[((ghostBlueY - 15) / 30) + ghostBlueDirectY][((ghostBlueX - 15) / 30) + ghostBlueDirectX] != -1) {
            sfTime sleepDuration = sfMilliseconds(7);
            //for (int i = 0; i < 30; i++)
            //{
                if (gameReset) {
                    ghost2Reset = true;
                    return NULL;
                }
                sem_wait(&collision2Semaphore);
                sfSleep(sleepDuration);
                ghostBlueX += ghostBlueDirectX * 30;
                ghostBlueY += ghostBlueDirectY * 30;
                sem_post(&collision2Semaphore);
            //}
            ghostBlueSteps--;
        }
        else
        {
            ghostBlueSteps = 0;
        }
    }
    else
    {
        int ghostGridX = (ghostBlueX - 15) / 30;
        int ghostGridY = (ghostBlueY - 15) / 30;
        sem_wait(&ghost2Semaphore);
        int pacmanGridX = (pacmanX - 15) / 30;
        int pacmanGridY = (pacmanY - 15) / 30;
        sem_post(&ghost2Semaphore);

        // If ghost is already at pacman's position then we return because no movement needed
        if (ghostGridX == pacmanGridX && ghostGridY == pacmanGridY) {
            return NULL;
        }

        // BFS setup
        Position parent[25][33];
        int visited[25][33] = { 0 };
        Position queue[25 * 33];
        int front = 0, rear = 0;

        // Start from ghost's position
        queue[rear++] = (Position){ ghostGridX, ghostGridY };
        visited[ghostGridY][ghostGridX] = 1;

        // Possible movement directions (left, right, up, down)
        int dx[] = { -1, 1, 0, 0 };
        int dy[] = { 0, 0, -1, 1 };

        Position current;
        int found = 0;

        // BFS loop
        while (front < rear && !found) {
            current = queue[front++];

            for (int i = 0; i < 4; i++) {
                int newX = current.x + dx[i];
                int newY = current.y + dy[i];

                if (isValid(newX, newY)) {
                    if (!visited[newY][newX]) {
                        parent[newY][newX] = current;
                        visited[newY][newX] = 1;
                        queue[rear++] = (Position){ newX, newY };

                        // If we reached pacman, backtrack to find first move
                        if (newX == pacmanGridX && newY == pacmanGridY) {
                            found = 1;
                            break;
                        }
                    }
                }
            }
        }
        // If path found, determine first move
        if (found) {
            Position target = { pacmanGridX, pacmanGridY };
            Position next;

            // Backtrack to find the first move from ghost's position
            while (!(parent[target.y][target.x].x == ghostGridX &&
                parent[target.y][target.x].y == ghostGridY)) {
                target = parent[target.y][target.x];
            }

            next = target;

            // Calculate direction based on first move
            int dirX = next.x - ghostGridX;
            int dirY = next.y - ghostGridY;
            sfTime sleepDuration = sfMilliseconds(7);
            // Move ghost in that direction
            //for (int i = 0; i < 30; i++)
            //{
                if (gameReset) {
                    ghost2Reset = true;
                    return NULL;
                }
                sem_wait(&collision2Semaphore);
                sfSleep(sleepDuration);
                ghostBlueX += dirX * 30;
                ghostBlueY += dirY * 30;
                sem_post(&collision2Semaphore);
            //}
        }
    }
    if ((((ghostBlueY - 15) / 30) == 10) && ((((ghostBlueX - 15) / 30) == 16) || (((ghostBlueX - 15) / 30) == 15) || (((ghostBlueX - 15) / 30) == 17)))
    {
        grid[10][15] = -1;
        grid[10][16] = -1;
        grid[10][17] = -1;
    }
    if (gameReset) {
        ghost2Reset = true;
        return NULL;
    }
    
}

void * calculateGhostMove3(void * args){
    if (gameReset) {
        ghost3Reset = true;
        return NULL;
    }
    if (grid[((ghostYellow1Y - 15) / 30) + ghostYellow1DirectY][((ghostYellow1X - 15) / 30) + ghostYellow1DirectX] != -1) {
        sfTime sleepDuration = sfMilliseconds(5);
        //for (int i = 0; i < 30; i++)
        //{
            if (gameReset) {
                ghost3Reset = true;
                return NULL;
            }
            sem_wait(&collision3Semaphore);
            sfSleep(sleepDuration);
            ghostYellow1X += ghostYellow1DirectX * 30;
            ghostYellow1Y += ghostYellow1DirectY * 30;
            sem_post(&collision3Semaphore);
        //}
        ghostYellow1Steps--;
    }
    else
    {
        ghostYellow1Steps = 0;
    }
    if ((((ghostYellow1Y - 15) / 30) == 10) && ((((ghostYellow1X - 15) / 30) == 16) || (((ghostYellow1X - 15) / 30) == 15) || (((ghostYellow1X - 15) / 30) == 17)))
    {
        grid[10][15] = -1;
        grid[10][16] = -1;
        grid[10][17] = -1;
    }
}
void* calculateGhostMove4(void* args) {
    if (gameReset) {
        ghost4Reset = true;
        return NULL;
    }
    if (grid[((ghostYellow2Y - 15) / 30) + ghostYellow2DirectY][((ghostYellow2X - 15) / 30) + ghostYellow2DirectX] != -1) {
        sfTime sleepDuration = sfMilliseconds(5);
        //for (int i = 0; i < 30; i++)
        //{
            if (gameReset) {
                ghost4Reset = true;
                return NULL;
            }
            sem_wait(&collision4Semaphore);
            sfSleep(sleepDuration);
            ghostYellow2X += ghostYellow2DirectX * 30;
            ghostYellow2Y += ghostYellow2DirectY * 30;
            sem_post(&collision4Semaphore);
        //}
        ghostYellow2Steps--;
    }
    else
    {
        ghostYellow2Steps = 0;
    }
    if ((((ghostYellow2Y - 15) / 30) == 10) && ((((ghostYellow2X - 15) / 30) == 16) || (((ghostYellow2X - 15) / 30) == 15) || (((ghostYellow2X - 15) / 30) == 17)))
    {
        grid[10][15] = -1;
        grid[10][16] = -1;
        grid[10][17] = -1;
    }
}


//-------------------------------------------------------------------- MAZE ------------------------------------------------------------------------
void drawMaze(sfRenderWindow* window) {
    drawRoundedRectangle(window, (sfVector2f) { 65, 65 }, (sfVector2f) { 110, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 70, 70 }, (sfVector2f) { 100, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 815, 65 }, (sfVector2f) { 110, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 820, 70 }, (sfVector2f) { 100, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 65, 125 }, (sfVector2f) { 50, 50 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 70, 130 }, (sfVector2f) { 40, 40 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 875, 125 }, (sfVector2f) { 50, 50 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 880, 130 }, (sfVector2f) { 40, 40 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 65, 215 }, (sfVector2f) { 50, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 70, 220 }, (sfVector2f) { 40, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 875, 215 }, (sfVector2f) { 50, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 880, 220 }, (sfVector2f) { 40, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 155, 185 }, (sfVector2f) { 20, 170 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 160, 190 }, (sfVector2f) { 10, 160 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 815, 185 }, (sfVector2f) { 20, 170 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 820, 190 }, (sfVector2f) { 10, 160 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 155, 395 }, (sfVector2f) { 20, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 160, 400 }, (sfVector2f) { 10, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 815, 395 }, (sfVector2f) { 20, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 820, 400 }, (sfVector2f) { 10, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 215, 395 }, (sfVector2f) { 80, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 220, 400 }, (sfVector2f) { 70, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 695, 395 }, (sfVector2f) { 80, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 700, 400 }, (sfVector2f) { 70, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 275, 185 }, (sfVector2f) { 20, 50 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 280, 190 }, (sfVector2f) { 10, 40 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 695, 185 }, (sfVector2f) { 20, 50 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 700, 190 }, (sfVector2f) { 10, 40 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 395, 65 }, (sfVector2f) { 50, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 400, 70 }, (sfVector2f) { 40, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 545, 65 }, (sfVector2f) { 50, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 550, 70 }, (sfVector2f) { 40, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 335, 335 }, (sfVector2f) { 50, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 340, 340 }, (sfVector2f) { 40, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 605, 335 }, (sfVector2f) { 50, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 610, 340 }, (sfVector2f) { 40, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 455, 125 }, (sfVector2f) { 80, 50 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 460, 130 }, (sfVector2f) { 70, 40 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 395, 215 }, (sfVector2f) { 200, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 400, 220 }, (sfVector2f) { 190, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 395, 485 }, (sfVector2f) { 200, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 400, 490 }, (sfVector2f) { 190, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 395, 545 }, (sfVector2f) { 200, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 400, 550 }, (sfVector2f) { 190, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 185, 515 }, (sfVector2f) { 110, 50 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 190, 520 }, (sfVector2f) { 100, 40 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 695, 515 }, (sfVector2f) { 110, 50 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 700, 520 }, (sfVector2f) { 100, 40 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 275, 605 }, (sfVector2f) { 170, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 280, 610 }, (sfVector2f) { 160, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 545, 605 }, (sfVector2f) { 170, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 550, 610 }, (sfVector2f) { 160, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 275, 125 }, (sfVector2f) { 140, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 335, 65 }, (sfVector2f) { 20, 140 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 280, 130 }, (sfVector2f) { 130, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 340, 70 }, (sfVector2f) { 10, 130 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 575, 125 }, (sfVector2f) { 140, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 635, 65 }, (sfVector2f) { 20, 140 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 580, 130 }, (sfVector2f) { 130, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 640, 70 }, (sfVector2f) { 10, 130 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 215, 65 }, (sfVector2f) { 20, 230 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 155, 125 }, (sfVector2f) { 80, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 220, 70 }, (sfVector2f) { 10, 220 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 160, 130 }, (sfVector2f) { 70, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 755, 65 }, (sfVector2f) { 20, 230 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 755, 125 }, (sfVector2f) { 80, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 760, 70 }, (sfVector2f) { 10, 220 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 760, 130 }, (sfVector2f) { 70, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 215, 335 }, (sfVector2f) { 80, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 275, 275 }, (sfVector2f) { 110, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 275, 275 }, (sfVector2f) { 20, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 220, 340 }, (sfVector2f) { 70, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 280, 280 }, (sfVector2f) { 100, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 280, 280 }, (sfVector2f) { 10, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 65, 515 }, (sfVector2f) { 80, 50 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 125, 515 }, (sfVector2f) { 20, 110 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 70, 520 }, (sfVector2f) { 70, 40 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 130, 520 }, (sfVector2f) { 10, 100 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 845, 515 }, (sfVector2f) { 80, 50 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 845, 515 }, (sfVector2f) { 20, 110 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 850, 520 }, (sfVector2f) { 70, 40 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 850, 520 }, (sfVector2f) { 10, 100 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 695, 335 }, (sfVector2f) { 80, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 605, 275 }, (sfVector2f) { 110, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 695, 275 }, (sfVector2f) { 20, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 700, 340 }, (sfVector2f) { 70, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 610, 280 }, (sfVector2f) { 100, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 700, 280 }, (sfVector2f) { 10, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 65, 665 }, (sfVector2f) { 230, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 185, 605 }, (sfVector2f) { 50, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 70, 670 }, (sfVector2f) { 220, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 190, 610 }, (sfVector2f) { 40, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 695, 665 }, (sfVector2f) { 230, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 755, 605 }, (sfVector2f) { 50, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 700, 670 }, (sfVector2f) { 220, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 760, 610 }, (sfVector2f) { 40, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 395, 665 }, (sfVector2f) { 200, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 485, 605 }, (sfVector2f) { 20, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 400, 670 }, (sfVector2f) { 190, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 490, 610 }, (sfVector2f) { 10, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 215, 455 }, (sfVector2f) { 140, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 335, 455 }, (sfVector2f) { 20, 110 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 220, 460 }, (sfVector2f) { 130, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 340, 460 }, (sfVector2f) { 10, 100 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 635, 455 }, (sfVector2f) { 140, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 635, 455 }, (sfVector2f) { 20, 110 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 640, 460 }, (sfVector2f) { 130, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 640, 460 }, (sfVector2f) { 10, 100 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 5, 605 }, (sfVector2f) { 80, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 10, 610 }, (sfVector2f) { 70, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 275, 5 }, (sfVector2f) { 20, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 280, 10 }, (sfVector2f) { 10, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 485, 5 }, (sfVector2f) { 20, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 490, 10 }, (sfVector2f) { 10, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 695, 5 }, (sfVector2f) { 20, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 700, 10 }, (sfVector2f) { 10, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 905, 605 }, (sfVector2f) { 80, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 910, 610 }, (sfVector2f) { 70, 10 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 335, 665 }, (sfVector2f) { 20, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 340, 670 }, (sfVector2f) { 10, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 635, 665 }, (sfVector2f) { 20, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 640, 670 }, (sfVector2f) { 10, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 5, 275 }, (sfVector2f) { 110, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 10, 280 }, (sfVector2f) { 100, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 5, 395 }, (sfVector2f) { 110, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 10, 400 }, (sfVector2f) { 100, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 875, 275 }, (sfVector2f) { 110, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 880, 280 }, (sfVector2f) { 100, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 875, 395 }, (sfVector2f) { 110, 80 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 880, 400 }, (sfVector2f) { 100, 70 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 425, 305 }, (sfVector2f) { 20, 140 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 545, 305 }, (sfVector2f) { 20, 140 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 425, 425 }, (sfVector2f) { 140, 20 }, 10, sfBlue);
    drawRoundedRectangle(window, (sfVector2f) { 430, 310 }, (sfVector2f) { 10, 130 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 550, 310 }, (sfVector2f) { 10, 130 }, 5, sfBlack);
    drawRoundedRectangle(window, (sfVector2f) { 430, 430 }, (sfVector2f) { 130, 10 }, 5, sfBlack);
}


//------------------------------------------------------------------- MAIN -------------------------------------------------------------------------
int main() {
    // Create a window with a title and a size

    startup();
    srand(time(0));
    sfVideoMode mode;
    mode.width = 1400;
    mode.height = 750;
    mode.bitsPerPixel = 32;


    // Create the window
    window = sfRenderWindow_create(mode, "My CSFML Window", sfResize | sfClose, NULL);
    if (!window) {
        return -1; // Failed to create the window
    }

    pthread_t uiT, palletT, pacmanT, ghostT1, ghostT2, ghostT3, ghostT4, collisionT, gateT;
    //pthread_mutex_init(&scoreLock, NULL);
    sem_init(&keySemaphore, 0, 1);
    sem_init(&moveSemaphore, 0, 0);
    sem_init(&palletSemaphore, 0, 0);
    sem_init(&ghost1Semaphore, 0, 1);
    sem_init(&ghost2Semaphore, 0, 1);
    sem_init(&collision1Semaphore, 0, 1);
    sem_init(&collision2Semaphore, 0, 1);
    sem_init(&collision3Semaphore, 0, 1);
    sem_init(&collision4Semaphore, 0, 1);
    

    // border shapes
    sfRectangleShape* upperBorder = sfRectangleShape_create();
    sfRectangleShape* upperBorder1 = sfRectangleShape_create();
    sfRectangleShape* lowerBorder = sfRectangleShape_create();
    sfRectangleShape* lowerBorder1 = sfRectangleShape_create();
    sfRectangleShape* leftBorder1 = sfRectangleShape_create();
    sfRectangleShape* rightBorder1 = sfRectangleShape_create(); 
    sfRectangleShape* leftBorder2 = sfRectangleShape_create();
    sfRectangleShape* leftBorder3 = sfRectangleShape_create();
    sfRectangleShape* leftBorder4 = sfRectangleShape_create();
    sfRectangleShape* rightBorder2 = sfRectangleShape_create();
    sfRectangleShape* rightBorder3 = sfRectangleShape_create();
    sfRectangleShape* rightBorder4 = sfRectangleShape_create();
    sfCircleShape* circle = sfCircleShape_create(); 

    sfFont* font = sfFont_createFromFile("Fonts/arial.ttf");
    if (!font) return -1;

    // Create text
    //score
    sfText* text = sfText_create();
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 20);
    sfText_setPosition(text, (sfVector2f) { 1050, 10 });
    sfText_setFillColor(text, sfWhite);
    // lives
    sfText* text2 = sfText_create();
    sfText_setFont(text2, font);
    sfText_setCharacterSize(text2, 20);
    sfText_setPosition(text2, (sfVector2f) { 1052, 40 });
    sfText_setFillColor(text2, sfWhite);

    // Convert int to string
    char display[50];
    



    sfCircleShape_setRadius(circle, 3); // Radius = 3 pixels
    sfCircleShape_setFillColor(circle, sfYellow);
    

    
// ---------------------------------------------- BORDERS ------------------------------------------------------

    sfRectangleShape_setSize(upperBorder, (sfVector2f) { 933, 27 });
    sfRectangleShape_setFillColor(upperBorder, sfBlue);
    sfRectangleShape_setPosition(upperBorder, (sfVector2f) { 27, 0 });

    sfRectangleShape_setSize(upperBorder1, (sfVector2f) { 990, 22 });
    sfRectangleShape_setFillColor(upperBorder1, sfBlack);
    sfRectangleShape_setPosition(upperBorder1, (sfVector2f) { 0, 0 });

    sfRectangleShape_setSize(lowerBorder, (sfVector2f) { 933, 27 });
    sfRectangleShape_setFillColor(lowerBorder, sfBlue);
    sfRectangleShape_setPosition(lowerBorder, (sfVector2f) { 27, 723 });

    sfRectangleShape_setSize(lowerBorder1, (sfVector2f) { 990, 22 });
    sfRectangleShape_setFillColor(lowerBorder1, sfBlack);
    sfRectangleShape_setPosition(lowerBorder1, (sfVector2f) { 0, 728 });

    sfRectangleShape_setSize(leftBorder1, (sfVector2f) { 27, 355 });
    sfRectangleShape_setFillColor(leftBorder1, sfBlue);
    sfRectangleShape_setPosition(leftBorder1, (sfVector2f) { 0, 0 });

    sfRectangleShape_setSize(leftBorder3, (sfVector2f) { 22, 350 });
    sfRectangleShape_setFillColor(leftBorder3, sfBlack);
    sfRectangleShape_setPosition(leftBorder3, (sfVector2f) { 0, 0 });

    sfRectangleShape_setSize(rightBorder1, (sfVector2f) { 27, 355 });
    sfRectangleShape_setFillColor(rightBorder1, sfBlue);
    sfRectangleShape_setPosition(rightBorder1, (sfVector2f) { 960, 0 });

    sfRectangleShape_setSize(rightBorder3, (sfVector2f) { 22, 350 });
    sfRectangleShape_setFillColor(rightBorder3, sfBlack);
    sfRectangleShape_setPosition(rightBorder3, (sfVector2f) { 965, 0 });

    sfRectangleShape_setSize(leftBorder2, (sfVector2f) { 27, 355 });
    sfRectangleShape_setFillColor(leftBorder2, sfBlue);
    sfRectangleShape_setPosition(leftBorder2, (sfVector2f) { 0, 395 });

    sfRectangleShape_setSize(leftBorder4, (sfVector2f) { 22, 355 });
    sfRectangleShape_setFillColor(leftBorder4, sfBlack);
    sfRectangleShape_setPosition(leftBorder4, (sfVector2f) { 0, 400 });

    sfRectangleShape_setSize(rightBorder2, (sfVector2f) { 27, 355 });
    sfRectangleShape_setFillColor(rightBorder2, sfBlue);
    sfRectangleShape_setPosition(rightBorder2, (sfVector2f) { 960, 395 });

    sfRectangleShape_setSize(rightBorder4, (sfVector2f) { 22, 355 });
    sfRectangleShape_setFillColor(rightBorder4, sfBlack);
    sfRectangleShape_setPosition(rightBorder4, (sfVector2f) { 965, 400 });

    sfSprite* backgroundSprite = sfSprite_create();
    sfTexture* background = sfTexture_createFromFile("Resources/maze.png", NULL);
    sfSprite_setTexture(backgroundSprite, background, sfTrue);
    sfSprite_setPosition(backgroundSprite, (sfVector2f) { 0, 0 });
    

//-------------------------------------------------- PACMAN PICTURES ----------------------------------------------------------------
    sfSprite* sprite = sfSprite_create();
    sfTexture* pacmanRight = sfTexture_createFromFile("Resources/pacmanRight.png", NULL);
    sfTexture* pacmanLeft = sfTexture_createFromFile("Resources/pacmanLeft.png", NULL);
    sfTexture* pacmanUp = sfTexture_createFromFile("Resources/pacmanUp.png", NULL);
    sfTexture* pacmanDown = sfTexture_createFromFile("Resources/pacmanDown.png", NULL);
    sfTexture* pacmanDown1 = sfTexture_createFromFile("Resources/pacmanDown1.png", NULL);
    sfTexture* pacmanUp1 = sfTexture_createFromFile("Resources/pacmanUp1.png", NULL);
    sfTexture* pacmanRight1 = sfTexture_createFromFile("Resources/pacmanRight1.png", NULL);
    sfTexture* pacmanLeft1 = sfTexture_createFromFile("Resources/pacmanLeft1.png", NULL);
    if (!pacmanRight || !pacmanLeft || !pacmanUp || !pacmanDown || !pacmanDown1 || !pacmanLeft1 || !pacmanRight1 || !pacmanUp1) {
        return -1;
    }
//----------------------------------------------------- GHOST PICTURES ----------------------------------------------------
    sfSprite* ghostSprite1 = sfSprite_create();
    sfSprite* ghostSprite2 = sfSprite_create();
    sfSprite* ghostSprite3 = sfSprite_create();
    sfSprite* ghostSprite4 = sfSprite_create();
    sfTexture* ghostRed1;
    sfTexture* ghostBlue1;
    sfTexture* ghostYellow1;
    sfTexture* ghostYellow2;

//------------------------------------------------ GAME LOOP -------------------------------------------------------
    while (sfRenderWindow_isOpen(window)) {
        // Process events
        
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
        sfTime sleepDuration = sfMilliseconds(4);
        sfSleep(sleepDuration);
        // Clear the window with a color (black)
        sfRenderWindow_clear(window, sfBlack);
        // Draw the rectangle shape
        pthread_create(&palletT, NULL, pallets, NULL);
        pthread_create(&uiT, NULL, userInterface, NULL);
        pthread_create(&pacmanT, NULL, pacman, NULL);
        pthread_create(&gateT, NULL, gateTimer, NULL);
        sem_wait(&collision1Semaphore);
        if ((pacmanX <= ghostRedX + 5) && (pacmanX >= ghostRedX - 5) && (pacmanY >= ghostRedY - 5) && (pacmanY <= ghostRedY + 5))
        {
            if (ghostEat)
            {
                ghostRedX = 465, ghostRedY = 405;
                ghostRedSteps = 0;
            }
            else
            {
                lives--;
                gameReset = true;
                directY = 0;
            }
        }
        sem_post(&collision1Semaphore);
        sem_wait(&collision2Semaphore);
        if ((pacmanX <= ghostBlueX + 5) && (pacmanX >= ghostBlueX - 5) && (pacmanY >= ghostBlueY - 5) && (pacmanY <= ghostBlueY + 5))
        {
            if (ghostEat)
            {
                ghostBlueX = 525, ghostBlueY = 405;
                ghostBlueSteps = 0;
            }
            else
            {
                lives--;
                gameReset = true;
                directY = 0;
            }
        }
        sem_post(&collision2Semaphore);
        sem_wait(&collision3Semaphore);
        if ((pacmanX <= ghostYellow1X + 5) && (pacmanX >= ghostYellow1X - 5) && (pacmanY >= ghostYellow1Y - 5) && (pacmanY <= ghostYellow1Y + 5))
        {
            if (ghostEat)
            {
                ghostYellow1X = 465, ghostYellow1Y = 375;
                ghostYellow1Steps = 0;
            }
            else
            {
                lives--;
                gameReset = true;
                directY = 0;
            }
        }
        sem_post(&collision3Semaphore);
        sem_wait(&collision4Semaphore);
        if ((pacmanX <= ghostYellow2X + 5) && (pacmanX >= ghostYellow2X - 5) && (pacmanY >= ghostYellow2Y - 5) && (pacmanY <= ghostYellow2Y + 5))
        {
            if (ghostEat)
            {
                ghostYellow2X = 525, ghostYellow2Y = 375;
                ghostYellow2Steps = 0;
            }
            else
            {
                lives--;
                gameReset = true;
                directY = 0;
            }
        }
        sem_post(&collision4Semaphore);

        if (gameReset) {
            ghostRedSpeed = 1;
            pacmanX = 495, pacmanY = 465, pacmanEat = 0, directX = 1, directY = 0;
            pacmanReset = false, ghost1Reset = false, ghost2Reset = false, ghost3Reset = false, ghost4Reset = false;
            keyPressed = 'd';
            // ghost thingies
            ghostRedX = 465, ghostRedY = 405, ghostBlueX = 525, ghostBlueY = 405;
            ghostYellow1X = 465, ghostYellow1Y = 375, ghostYellow2X = 525, ghostYellow2Y = 375, ghostYellow1Steps = 0, ghostYellow2Steps = 0;
            gameReset = false;
        }
        if (lives == 0)
        {
            return 0;
        }
        // moving ghost
        ghostMove++;
        ghostMove1++;
        ghostMove2++;
        ghostMove3++;
        

        if (!ghostEat && grid[1][1] == 0 && grid[23][31] == 0 && grid[23][1] == 0 && grid[1][31] == 0)
        {
            grid[1][1] = 2, grid[23][31] = 2, grid[23][1] = 2, grid[1][31] = 2;
        }

        if (ghostEat)
        {
            ghostRed1 = sfTexture_createFromFile("Resources/ghostEat.png", NULL);
            if (!ghostRed1) {
                return -1;
            }
            ghostBlue1 = sfTexture_createFromFile("Resources/ghostEat.png", NULL);
            if (!ghostBlue1) {
                return -1;
            }
            ghostYellow1 = sfTexture_createFromFile("Resources/ghostEat.png", NULL);
            if (!ghostYellow1) {
                return -1;
            }
            ghostYellow2 = sfTexture_createFromFile("Resources/ghostEat.png", NULL);
            if (!ghostYellow2) {
                return -1;
            }
        }
        else
        {
            
            ghostRed1 = sfTexture_createFromFile("Resources/ghostRed.png", NULL);
            if (!ghostRed1) {
                return -1;
            }
            ghostBlue1 = sfTexture_createFromFile("Resources/ghostBlue.png", NULL);
            if (!ghostBlue1) {
                return -1;
            }
            ghostYellow1 = sfTexture_createFromFile("Resources/ghostYellow.png", NULL);
            if (!ghostYellow1) {
                return -1;
            }
            ghostYellow2 = sfTexture_createFromFile("Resources/ghostYellow.png", NULL);
            if (!ghostYellow2) {
                return -1;
            }
        }

        if (ghostYellow1Steps == 0)
        {
            ghostYellow1Steps = rand() % 9;
            int toss = rand() % 2;
            if (toss)
            {
                int toss1 = rand() % 2;
                if (toss1)
                {
                    ghostYellow1DirectX = -1;
                    ghostYellow1DirectY = 0;
                }
                else
                {
                    ghostYellow1DirectX = 1;
                    ghostYellow1DirectY = 0;
                }
            }
            else
            {
                int toss1 = rand() % 2;
                if (toss1)
                {
                    ghostYellow1DirectX = 0;
                    ghostYellow1DirectY = -1;
                }
                else
                {
                    ghostYellow1DirectX = 0;
                    ghostYellow1DirectY = 1;
                }
            }
        }
        if (ghostYellow2Steps == 0)
        {
            ghostYellow2Steps = rand() % 9;
            int toss = rand() % 2;
            if (toss)
            {
                int toss1 = rand() % 2;
                if (toss1)
                {
                    ghostYellow2DirectX = -1;
                    ghostYellow2DirectY = 0;
                }
                else
                {
                    ghostYellow2DirectX = 1;
                    ghostYellow2DirectY = 0;
                }
            }
            else
            {
                int toss1 = rand() % 2;
                if (toss1)
                {
                    ghostYellow2DirectX = 0;
                    ghostYellow2DirectY = -1;
                }
                else
                {
                    ghostYellow2DirectX = 0;
                    ghostYellow2DirectY = 1;
                }
            }
        }

        if (ghostBlueSteps == 0 && ghostEat)
        {
            ghostBlueSteps = rand() % 9;
            int toss = rand() % 2;
            if (toss)
            {
                int toss1 = rand() % 2;
                if (toss1)
                {
                    ghostBlueDirectX = -1;
                    ghostBlueDirectY = 0;
                }
                else
                {
                    ghostBlueDirectX = 1;
                    ghostBlueDirectY = 0;
                }
            }
            else
            {
                int toss1 = rand() % 2;
                if (toss1)
                {
                    ghostBlueDirectX = 0;
                    ghostBlueDirectY = -1;
                }
                else
                {
                    ghostBlueDirectX = 0;
                    ghostBlueDirectY = 1;
                }
            }
        }

        if (ghostRedSteps == 0 && ghostEat)
        {
            ghostRedSteps = rand() % 9;
            int toss = rand() % 2;
            if (toss)
            {
                int toss1 = rand() % 2;
                if (toss1)
                {
                    ghostRedDirectX = -1;
                    ghostRedDirectY = 0;
                }
                else
                {
                    ghostRedDirectX = 1;
                    ghostRedDirectY = 0;
                }
            }
            else
            {
                int toss1 = rand() % 2;
                if (toss1)
                {
                    ghostRedDirectX = 0;
                    ghostRedDirectY = -1;
                }
                else
                {
                    ghostRedDirectX = 0;
                    ghostRedDirectY = 1;
                }
            }
        }

        if (ghostMove >= 30 - (ghostRedSpeed * 10)) {
            ghostMove = 0;
            pthread_create(&ghostT1, NULL, calculateGhostMove1, NULL);
            //calculateGhostMove1();
        }
        if (ghostMove1 >= 30) {
            ghostMove1 = 0;
            pthread_create(&ghostT2, NULL, calculateGhostMove2, NULL);
        }
        if (ghostMove2 == 25) {
            ghostMove2 = 0;
            pthread_create(&ghostT3, NULL, calculateGhostMove3, NULL);
        }
        if (ghostMove3 == 25)
        {
            ghostMove3 = 0;
            pthread_create(&ghostT4, NULL, calculateGhostMove4, NULL);
        }

        sfRenderWindow_drawRectangleShape(window, upperBorder, NULL);
        sfRenderWindow_drawRectangleShape(window, lowerBorder, NULL);
        sfRenderWindow_drawRectangleShape(window, leftBorder1, NULL);
        sfRenderWindow_drawRectangleShape(window, rightBorder1, NULL);
        sfRenderWindow_drawRectangleShape(window, leftBorder2, NULL);
        sfRenderWindow_drawRectangleShape(window, rightBorder2, NULL);
        drawMaze(window);
        sfRenderWindow_drawRectangleShape(window, upperBorder1, NULL);
        sfRenderWindow_drawRectangleShape(window, lowerBorder1, NULL);
        sfRenderWindow_drawRectangleShape(window, leftBorder3, NULL);
        sfRenderWindow_drawRectangleShape(window, leftBorder4, NULL);
        sfRenderWindow_drawRectangleShape(window, rightBorder3, NULL);
        sfRenderWindow_drawRectangleShape(window, rightBorder4, NULL);

        sprintf(display, "Score: %d", score);

        sfText_setString(text, display);
        sfRenderWindow_drawText(window, text, NULL);

        sprintf(display, "Lives: %d", lives);
        sfText_setString(text2, display);
        sfRenderWindow_drawText(window, text2, NULL);

    ///------------------------------------- GHOST -------------------------------------------
        sfVector2u size, ghostSize1, ghostSize2, ghostSize3, ghostSize4;
        sfSprite_setTexture(ghostSprite1, ghostRed1, sfTrue);
        ghostSize1 = sfTexture_getSize(ghostRed1);
        sfSprite_setOrigin(ghostSprite1, (sfVector2f) { ghostSize1.x / 2.0f, ghostSize1.y / 2.0f });
        sfSprite_setPosition(ghostSprite1, (sfVector2f) { ghostRedX, ghostRedY }); 

        sfSprite_setTexture(ghostSprite2, ghostBlue1, sfTrue);
        ghostSize2 = sfTexture_getSize(ghostBlue1);
        sfSprite_setOrigin(ghostSprite2, (sfVector2f) { ghostSize2.x / 2.0f, ghostSize2.y / 2.0f });
        sfSprite_setPosition(ghostSprite2, (sfVector2f) { ghostBlueX, ghostBlueY });

        sfSprite_setTexture(ghostSprite3, ghostYellow1, sfTrue);
        ghostSize3 = sfTexture_getSize(ghostYellow1);
        sfSprite_setOrigin(ghostSprite3, (sfVector2f) { ghostSize3.x / 2.0f, ghostSize3.y / 2.0f });
        sfSprite_setPosition(ghostSprite3, (sfVector2f) { ghostYellow1X, ghostYellow1Y });

        sfSprite_setTexture(ghostSprite4, ghostYellow2, sfTrue);
        ghostSize4 = sfTexture_getSize(ghostYellow2);
        sfSprite_setOrigin(ghostSprite4, (sfVector2f) { ghostSize4.x / 2.0f, ghostSize4.y / 2.0f });
        sfSprite_setPosition(ghostSprite4, (sfVector2f) { ghostYellow2X, ghostYellow2Y });

        if (directX == 0 && directY == -1)
        {
            if (pacmanEat >= 0 && pacmanEat <= 19)
            {
                sfSprite_setTexture(sprite, pacmanUp1, sfTrue);
                size = sfTexture_getSize(pacmanUp1);
                pacmanEat++;
            }
            else if(pacmanEat >= 20 && pacmanEat <=39){
                sfSprite_setTexture(sprite, pacmanUp, sfTrue);
                size = sfTexture_getSize(pacmanUp);
                pacmanEat++;
                if (pacmanEat == 40)
                {
                    pacmanEat = 0;
                }
            }
            sfSprite_setOrigin(sprite, (sfVector2f) { size.x / 2.0f, size.y / 2.0f });
        }
        else if (directX == 0 && directY == 1)
        {
            if (pacmanEat >= 0 && pacmanEat <= 19)
            {
                sfSprite_setTexture(sprite, pacmanDown1, sfTrue);
                size = sfTexture_getSize(pacmanDown1);
                pacmanEat++;

            }
            else if (pacmanEat >= 20 && pacmanEat <= 39) {
                sfSprite_setTexture(sprite, pacmanDown, sfTrue);
                size = sfTexture_getSize(pacmanDown);
                pacmanEat++;
                if (pacmanEat == 40)
                {
                    pacmanEat = 0;
                }
            }
            sfSprite_setOrigin(sprite, (sfVector2f) { size.x / 2.0f, size.y / 2.0f });
        }
        else if (directX == 1 && directY == 0)
        {
            if (pacmanEat >= 0 && pacmanEat <= 19)
            {
                sfSprite_setTexture(sprite, pacmanRight1, sfTrue);
                size = sfTexture_getSize(pacmanRight1);
                pacmanEat++;
            }
            else if (pacmanEat >= 20 && pacmanEat <= 39) {
                sfSprite_setTexture(sprite, pacmanRight, sfTrue);
                size = sfTexture_getSize(pacmanRight);
                pacmanEat++;
                if (pacmanEat == 40)
                {
                    pacmanEat = 0;
                }
            }
            sfSprite_setOrigin(sprite, (sfVector2f) { size.x / 2.0f, size.y / 2.0f });
        }
        else if (directX == -1 && directY == 0)
        {
            if (pacmanEat >= 0 && pacmanEat <= 19)
            {
                sfSprite_setTexture(sprite, pacmanLeft1, sfTrue);
                size = sfTexture_getSize(pacmanLeft1);
                pacmanEat++;
            }
            else if (pacmanEat >= 20 && pacmanEat <= 39) {
                sfSprite_setTexture(sprite, pacmanLeft, sfTrue);
                size = sfTexture_getSize(pacmanLeft);
                pacmanEat++;
                if (pacmanEat == 40)
                {
                    pacmanEat = 0;
                }
            }
            sfSprite_setOrigin(sprite, (sfVector2f) { size.x / 2.0f, size.y / 2.0f });
        }
        sfSprite_setPosition(sprite, (sfVector2f) { pacmanX, pacmanY }); // X = 100, Y = 100
        
        for (int i = 0; i < 25; i++)
        {
            for (int j = 0; j < 33; j++)
            {
                if (grid[i][j] == 1)
                {
                    sfCircleShape_setRadius(circle, 3); // Radius = 3 pixels
                    sfCircleShape_setOrigin(circle, (sfVector2f) { 3, 3 });
                    sfCircleShape_setPosition(circle, (sfVector2f) { (j * 30) + 15, (i * 30) + 15 }); // Center-ish
                    sfRenderWindow_drawCircleShape(window, circle, NULL);
                }
                else if (grid[i][j] == 2) {
                    sfCircleShape_setRadius(circle, 8); // Radius = 8 pixels
                    sfCircleShape_setOrigin(circle, (sfVector2f) { 8, 8 });
                    sfCircleShape_setPosition(circle, (sfVector2f) { (j * 30) + 15, (i * 30) + 15 }); // Center-ish
                    sfRenderWindow_drawCircleShape(window, circle, NULL);
                }
                else if (grid[i][j] == 3) {
                    sfSprite* lightSprite = sfSprite_create();
                    sfTexture* light = sfTexture_createFromFile("Resources/light.png", NULL);
                    sfSprite_setTexture(lightSprite, light, sfTrue);
                    sfSprite_setPosition(lightSprite, (sfVector2f) { j * 30, i * 30 }); // X = 100, Y = 100
                    sfRenderWindow_drawSprite(window, lightSprite, NULL);
                }
            }
        }
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_drawSprite(window, ghostSprite1, NULL);
        sfRenderWindow_drawSprite(window, ghostSprite2, NULL);
        sfRenderWindow_drawSprite(window, ghostSprite3, NULL);
        sfRenderWindow_drawSprite(window, ghostSprite4, NULL);
        
        // Display the contents of the window
        sfRenderWindow_display(window);

    }

    // Cleanup
    //sfRectangleShape_destroy(ghost); // ghost cleanup
    sfCircleShape_destroy(circle);
    sfRectangleShape_destroy(upperBorder);
    sfRectangleShape_destroy(lowerBorder);
    sfRectangleShape_destroy(rightBorder1);
    sfRectangleShape_destroy(rightBorder2);
    sfRectangleShape_destroy(leftBorder1);
    sfRectangleShape_destroy(leftBorder2);
    sfSprite_destroy(sprite);
    sfSprite_destroy(ghostSprite1);
    sfSprite_destroy(ghostSprite2);
    sfSprite_destroy(ghostSprite3);
    sfSprite_destroy(ghostSprite4);
    sfSprite_destroy(backgroundSprite);
    sfTexture_destroy(pacmanUp);
    sfTexture_destroy(pacmanDown);
    sfTexture_destroy(pacmanRight);
    sfTexture_destroy(pacmanLeft);
    sfTexture_destroy(pacmanUp1);
    sfTexture_destroy(pacmanDown1);
    sfTexture_destroy(pacmanRight1);
    sfTexture_destroy(pacmanLeft1);
    sfTexture_destroy(ghostRed1);
    sfTexture_destroy(ghostBlue1);
    sfTexture_destroy(ghostYellow1);
    sfTexture_destroy(ghostYellow2);
    sfTexture_destroy(background);
    sfText_destroy(text);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);

    return 0;
}