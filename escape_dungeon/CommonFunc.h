#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <iostream>
#include "BaseObject.h"
#include <stdio.h>
static SDL_Window* g_window = NULL;
static SDL_Renderer * g_screen = NULL;
static SDL_Event g_event;

void logErrorAndExit(const char* msg, const char* error);

// screen
const int FRAME_PER_SECOND = 25;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;



#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10
#define BLANK_TILE 0
typedef struct Map{
    int start_x;
    int start_y;

    int max_x;
    int max_y;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name;
};


typedef struct Input
{
    int left;
    int right;
    int down ;
    int up;
};


namespace SDLCommonFunc
{
    bool CheckCollision(const SDL_Rect& object1 , const SDL_Rect& object2);
}



#endif // COMMON_FUNCTION_H_
