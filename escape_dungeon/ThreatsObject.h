


#ifndef THREATS_OBJECT_H
#define THREATS_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define THREAT_SPEED 5
#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_FRAME_NUM 8
class ThreatsObject : public BaseObject
{
public:
    ThreatsObject();
    ~ThreatsObject();

    enum TypeMove
    {
        STATIC_THREAT = 0,
        MOVE_IN_SPACE_THREAT =1,
    };
    void set_x_val(const float& xVal){x_val = xVal;}
    void set_y_val(const float& yVal ){y_val = yVal;}

    void set_x_pos(const float& xp){x_pos = xp;}
    void set_y_pos(const float& yp){y_pos = yp;}
    float get_x_pos() const {return x_pos;}
    float get_y_pos() const {return y_pos;}

    void SetMapXY(const int& mp_x , const int& mp_y){map_x = mp_x; map_y = mp_y;}

    void set_clips();
    bool LoadImg(std :: string path , SDL_Renderer * screen);
    void show(SDL_Renderer * des);
    int get_width_frame() const {return width_frame;}
    int get_height_frame() const {return height_frame;}
    void DoPlayer(Map& gMap);
    void checkToMap(Map& gMap);

    void set_type_move(const int& typeMove){type_move = typeMove;}
    void SetAnimationPos(const int& pos_a , const int& pos_b){animation_a = pos_a ; animation_b = pos_b;}
    void set_input_left(const int& ipleft ){input_type.left = ipleft;};
    void ImpMoveType(SDL_Renderer * screen);


private:
    int frame;
    int width_frame;
    int height_frame;
    SDL_Rect frame_clip[THREAT_FRAME_NUM];
    int come_back_time;
    bool on_ground;
    float x_pos;
    float y_pos;
    float x_val;
    float y_val;
    int map_x;
    int map_y;

    int type_move;
    int animation_a;
    int animation_b;
    Input input_type;

};




#endif // THREATS_OBJECT_H
