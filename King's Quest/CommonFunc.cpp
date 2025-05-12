#include "CommonFunc.h"

void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}


bool SDLCommonFunc::CheckCollisionPlayer(const SDL_Rect& a, const SDL_Rect& b)
{
    if (a.x + a.w - PLAYER_COLLISION_OFFSET_X <= b.x) return false; // a nằm bên trái b
    if (a.x +PLAYER_COLLISION_OFFSET_X >= b.x + b.w) return false; // a nằm bên phải b
    if (a.y + a.h <= b.y) return false; // a nằm trên b
    if (a.y >= b.y + b.h) return false; // a nằm dưới b

    return true; // nếu không rơi vào 4 trường hợp trên => có va chạm
}

bool SDLCommonFunc::CheckCollision(const SDL_Rect& a, const SDL_Rect& b)
{
    if (a.x + a.w<= b.x) return false; // a nằm bên trái b
    if (a.x >= b.x + b.w) return false; // a nằm bên phải b
    if (a.y + a.h<= b.y) return false; // a nằm trên b
    if (a.y >= b.y + b.h) return false; // a nằm dưới b

    return true; // nếu không rơi vào 4 trường hợp trên => có va chạm
}
