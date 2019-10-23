#include <gb/gb.h>
#include <gb/drawing.h>
#include <gb/console.h>
#include <gb/font.h>
#include <stdio.h>
#include "GameCharacter.c"
#include "GameSprites.c"


//functions
void GameLoop();
void GameOver();

//variables
GameCharacter ship;
UBYTE spritesize = 8;
const int asteroidcount = 3;
GameCharacter asteroid[3];
int i;
//int distance = 0;
//int hight = 0;
int asteroidsSpawnX[16] = {0, 20, 50, 70, 120, 140, 30, 40, 60, 80, 90, 20, 100, 10, 50, 10};
int asteroidsSpawnXCount = 16;
int j;
int HP = 3;
bool IsGameOver = false;
int Hight = 0;
// #region Collision
UBYTE checkCollisions(GameCharacter *one, GameCharacter *two)
{
    return (one->x >= two->x && one->x <= two->x + two->width) &&
               (one->y >= two->y && one->y <= two->y + two->height) ||
           (two->x >= one->x && two->x <= one->x + one->width) &&
               (two->y >= one->y && two->y <= one->y + one->height);
}

// #endregion

// #region delay
void performantdelay(UINT8 numloops)
{
    UINT8 i;
    for (i = 0; i < numloops; i++)
    {
        wait_vbl_done();
    }
}

// #endregion

// #region Movegamecharacter
void movegamecharacter(GameCharacter *character, UINT8 x, UINT8 y)
{
    move_sprite(character->spritids[0], x, y);
    move_sprite(character->spritids[1], x + spritesize, y);
    move_sprite(character->spritids[2], x, y + spritesize);
    move_sprite(character->spritids[3], x + spritesize, y + spritesize);
}
// #endregion

// #region SetupShip
void setupShip()
{
    ship.x = 80;
    ship.y = 130;
    ship.width = 16;
    ship.height = 16;

    //load sprite for ship
    set_sprite_tile(0, 0);
    ship.spritids[0] = 0;
    set_sprite_tile(1, 1);
    ship.spritids[1] = 1;
    set_sprite_tile(2, 2);
    ship.spritids[2] = 2;
    set_sprite_tile(3, 3);
    ship.spritids[3] = 3;

    movegamecharacter(&ship, ship.x, ship.y);
}
// #endregion

// #region SetupAsteroid
void setupAsteroid()
{
    int temp = 4;
    int distance = 0;
    int hight = 0;
    //load sprite for ship
    int forCounter = 0;
    for (forCounter = 0; forCounter < 12; forCounter++)
    {
        set_sprite_tile(temp, temp);
        temp++;
    }

    temp = 4;
    for (i = 0; i < asteroidcount; i++)
    {
        distance += 20;
        hight += 10;
        asteroid[i].ID = i;
        asteroid[i].x = distance;
        asteroid[i].y = 20 + hight;
        asteroid[i].width = 16;
        asteroid[i].height = 16;
        asteroid[i].isCollided = true;

        asteroid[i].spritids[0] = temp; //setting each diffrent Asteroidtile
        temp += 1;
        asteroid[i].spritids[1] = temp; //setting each diffrent Asteroidtile
        temp += 1;
        asteroid[i].spritids[2] = temp; //setting each diffrent Asteroidtile
        temp += 1;
        asteroid[i].spritids[3] = temp; //setting each diffrent Asteroidtile
        temp += 1;
        movegamecharacter(&asteroid[i], asteroid[i].x, asteroid[i].y);
    }
}
// #endregion

// #region GameOver
void GameOver()
{
    //Hide Gamesprites
    HIDE_SPRITES;

    gotoxy(0, 0);
    printf("=====PRESS START TO TRY AGAIN=====");
   
    for (i = 0; i < asteroidcount; i++)
    {
        if (j > asteroidsSpawnXCount)
        {
            j = 0;
        }
        asteroid[i].x = asteroidsSpawnX[j];
        asteroid[i].y = Hight;
        Hight += 10;
        j += 1;
        movegamecharacter(&asteroid[i], asteroid[i].x, asteroid[i].y);
    }

    Hight = 0;

    waitpad(J_START);
    HP = 3;
    gotoxy(0, 0);
    printf("                                                            ");
    //show them back again
    SHOW_SPRITES;
    GameLoop();
}
// #endregion

// #region GameLoop
void GameLoop()
{

    gotoxy(0, 0);
    printf("HP: %d", HP);

    IsGameOver = false;
    while (IsGameOver == false)
    {

        for (i = 0; i < asteroidcount; i++)
        {
            //check the collision
            if (checkCollisions(&ship, &asteroid[i]))
            {
                asteroid[i].isCollided = true;
                IsGameOver = true;
            }
            if (j > asteroidsSpawnXCount)
            {
                j = 0;
            }

            asteroid[i].y += 2;
            if (asteroid[i].y >= 144) //144 screen hight
            {
                asteroid[i].y = 0;
                asteroid[i].x = asteroidsSpawnX[j];
                j += 1;
            }
            movegamecharacter(&asteroid[i], asteroid[i].x, asteroid[i].y);
        }
        if (joypad() & J_LEFT)
        {
            ship.x -= 2;
            movegamecharacter(&ship, ship.x, ship.y);
        }
        if (joypad() & J_RIGHT)
        {
            ship.x += 2;

            movegamecharacter(&ship, ship.x, ship.y);
        }

        performantdelay(2);
    }
    HP -= 1;
    if (HP > 0)
    {
        for (i = 0; i < asteroidcount; i++)
        {
            if (j > asteroidsSpawnXCount)
            {
                j = 0;
            }
            asteroid[i].x = asteroidsSpawnX[j];
            asteroid[i].y = Hight;
            Hight += 10;
            j += 1;
            movegamecharacter(&asteroid[i], asteroid[i].x, asteroid[i].y);
        }

        Hight = 0;
        IsGameOver = false;
        GameLoop();
    }
    else
    {
        IsGameOver = false;
        GameOver();
    }
}
// #endregion

// #region main
void main()
{
    font_t ibm_font;

    font_init();
    ibm_font = font_load(font_ibm);
    font_set(ibm_font);

    set_sprite_data(0, 16, gamesprites);
    setupShip();
    setupAsteroid();

    SHOW_SPRITES;
    DISPLAY_ON;
    GameLoop();
}

// #endregion
