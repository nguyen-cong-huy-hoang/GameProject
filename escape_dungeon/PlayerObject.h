


#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "BulletObject.h"
#include <vector>

#define MAX_FRAME 8
#define PLAYER_JUMP_VAL 18
#define GRAVITY_SPEED 20
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 10
// xây dựng hành động nhân vật

class PlayerObject : public BaseObject
{
public:
    PlayerObject();
    ~PlayerObject();

    enum WalkType
    {
        WALK_RIGHT = 0,
        WALK_LEFT = 1
    };

    void  increasemoney();
    bool LoadImg(std:: string path , SDL_Renderer * screen); // tai anh ra man hinh
    void show(SDL_Renderer * des); // hien thi nhan vat
    void HandelInputAction(SDL_Event event , SDL_Renderer * screen); // cac phim chuc nang
    void set_clips(); // tao ra hinh anh dong

    void DoPlayer(Map& map_data); // di chuyen nhan vat
    void CheckToMap(Map& map_data); // kiem tra va cham
    void setMapXY(const int map_x_ , const int map_y_){map_x = map_x_ ; map_y = map_y_;} // dat  toa do nhan vat trong map
    void CenterEnityOnMap(Map& map_data); // di chuyen man hinh theo nhan vat


    SDL_Rect GetRectFrame();
    void set_bullet_list(std:: vector<BulletObject*> bullet_list) // thiet lap dan cho nhan vat
    {
        p_bullet_list = bullet_list;
    }
    std:: vector<BulletObject*> get_bullet_list() const {return p_bullet_list;} // lay ra dan

    void HandleBullet(SDL_Renderer* des);
    void removeBullet(const int& idx);

private:

    int money_count;
    std :: vector<BulletObject*> p_bullet_list;
    float x_val;
    float y_val;

    float x_pos;
    float y_pos;
    // luu frame hien tai cua nhan vat
    int width_frame;
    int height_frame;

    SDL_Rect frame_clip[MAX_FRAME]; // luu 8 frame cua nhan vat

    Input input_type;
    int frame;
    int status;
    bool on_ground;

    int map_x;
    int map_y;

};


#endif // PLAYER_OBJECT_H
