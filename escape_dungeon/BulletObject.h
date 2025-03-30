#ifndef BULLET_OBJECT_H
#define BULLET_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"

class BulletObject : public BaseObject
{
public:
    BulletObject();
    ~BulletObject();


    enum BulletDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
    };

    enum BulletType
    {

        SPHERE_BULLET = 50,
        LASER_BULLET = 51,
    };

    void set_x_val(const int& xVal) { x_val = xVal; }
    void set_y_val(const int& yVal) { y_val = yVal; }
    int get_x_val() const { return x_val; }
    int get_y_val() const { return y_val; }

    void set_bullet_dir(const unsigned int& bulletDir){bullet_dir = bulletDir;}
    int get_bullet_dir() const{return bullet_dir;}
    void set_is_move(const bool& isMove) { is_move = isMove; }
    bool get_is_move() const { return is_move; }

    void HandleMove(const int& x_border, const int& y_border);


    void set_bullet_type(const unsigned int& bulletType){bullet_type =bulletType;}
    unsigned int get_bullet_type() const {return bullet_type;}
    void LoadImageBullet(SDL_Renderer* des);
private:
    int x_val; // van toc theo truc x
    int y_val; // van toc theo truc y
    bool is_move; // trang thai dan
    unsigned int bullet_dir; // huong
    unsigned int bullet_type;
};

#endif // BULLET_OBJECT_H
