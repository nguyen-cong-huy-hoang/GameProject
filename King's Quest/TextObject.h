


#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include "CommonFunc.h"
class TextObject
{
public:
    TextObject();
    ~TextObject();

    enum TextColor
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,

    };


    bool LoadFromRenderText(TTF_Font* font , SDL_Renderer* screen);
    void Free();

    void SetColor(Uint8 red , Uint8 green , Uint8 blue);
    void SetColor(int type);
    void RenderText(SDL_Renderer* screen ,
                    int xp , int yp ,
                     SDL_Rect* clip = NULL ,
                     double angle = 0.0 ,
                     SDL_Point* center = NULL ,
                      SDL_RendererFlip flip = SDL_FLIP_NONE);

    int GetWidth() const {return width;}
    int GetHeight() const {return height;}

    void SetText(const std :: string& text){str_val = text;}
    std :: string GetText() const {return str_val;}

private:
    std:: string str_val;
    SDL_Color text_color;
    SDL_Texture* texture;
    int width;
    int height;


};


#endif // TEXT_OBJECT_H
