#include <gb/gb.h>

//generical character structure: id, position, graphics

typedef enum { false, true } bool;
typedef struct GameCharacter
{
    UBYTE spritids[4]; // all characters use 4 sprites
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
    UINT8 ID;
    bool isCollided;
} GameCharacter;
