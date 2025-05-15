

#include "ThreatsObject.h"

ThreatsObject :: ThreatsObject()
{
    width_frame = 0;
    height_frame = 0;
    x_val = 0.0;
    y_val = 0.0;
    x_pos = 0.0;
    y_pos = 0.0;
    on_ground = false;
    come_back_time = 0;
    frame = 0;

    animation_a = 0;
    animation_b = 0;
    input_type.left = 0;
    input_type.right = 0;
    type_move = STATIC_THREAT;
    num_frame = 0;

}

ThreatsObject :: ~ThreatsObject()
{

}

bool ThreatsObject :: LoadImgFlying(std :: string path , SDL_Renderer* screen)
{

    bool ret = BaseObject :: LoadImg(path , screen);

    if (!ret) {

    }
    if(ret)
    {
        num_frame = THREAT_FRAME_FLYING;
        width_frame = rect_.w / THREAT_FRAME_FLYING;
        height_frame = rect_.h;
        set_clips();
    }

    return ret;
}

bool ThreatsObject :: LoadImg(std :: string path , SDL_Renderer* screen)
{

    bool ret = BaseObject :: LoadImg(path , screen);

    if (!ret) {

    }
    if(ret)
    {
        num_frame = THREAT_FRAME_NUM;
        width_frame = rect_.w / THREAT_FRAME_NUM;
        height_frame = rect_.h;
        set_clips();
    }

    return ret;
}
bool ThreatsObject :: LoadImgBoss(std :: string path , SDL_Renderer* screen)
{

    bool ret = BaseObject :: LoadImg(path , screen);

    if (!ret) {

    }
    if(ret)
    {
        num_frame = THREAT_FRAME_NUM;
        width_frame = rect_.w / BOSS_FRAME_NUM;
        height_frame = rect_.h;
        set_clips();
    }

    return ret;
}
SDL_Rect ThreatsObject :: GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame;
    rect.h = height_frame;

    return rect;
}


void ThreatsObject :: set_clips()
{
    if(width_frame > 0 && height_frame > 0)
    {

        for(int i = 0 ;i < num_frame ; i++)
        {
            frame_clip[i].x = i*width_frame;
            frame_clip[i].y = 0;
            frame_clip[i].w = width_frame;
            frame_clip[i].h = height_frame;
        }
    }

}

void ThreatsObject::DoPlayer(Map& gMap)
{
    if (come_back_time == 0)
    {
        x_val = 0;

        if (type_move != FLYING_THREAT)
        {

                y_val += THREAT_GRAVITY_SPEED;
                if (y_val >= THREAT_MAX_FALL_SPEED)
                {
                    y_val = THREAT_MAX_FALL_SPEED;
                }

        }
        else
        {
            y_val = 0;
        }

        if (input_type.left == 1)
        {
            x_val -= THREAT_SPEED;
        }
        else if (input_type.right == 1)
        {
            x_val += THREAT_SPEED;
        }

        checkToMap(gMap);
    }
    else if (come_back_time > 0)
    {
        come_back_time--;
        if (come_back_time == 0)
        {
            x_val = 0;
            y_val = 0;
            x_pos = 0;

            int tile_y = (SCREEN_HEIGHT / TILE_SIZE) - 1;
            int ground_y = tile_y * TILE_SIZE;

            if (type_move == FLYING_THREAT)
            {
                y_pos = ground_y - get_height_frame() - TILE_SIZE / 2;
            }
            else
            {
                y_pos = ground_y - get_height_frame();
                on_ground = false;
            }

            come_back_time = 0;
            input_type.left = 1;
        }
    }
}

void ThreatsObject::checkToMap(Map& gMap)
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    //Kiểm tra va chạm dọc
    if (type_move != FLYING_THREAT)
    {


        int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
        x1 = x_pos / TILE_SIZE;
        x2 = (x_pos + width_min) / TILE_SIZE;
        y1 = (y_pos + y_val) / TILE_SIZE;
        y2 = (y_pos + y_val + height_frame - 1) / TILE_SIZE;

        if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
        {
            if (y_val > 0)
            {
                if (gMap.tile[y2][x1] != BLANK_TILE || gMap.tile[y2][x2] != BLANK_TILE)
                {
                    y_pos = y2 * TILE_SIZE - (height_frame + 1);
                    y_val = 0;
                    on_ground = true;
                }
                else
                {
                    on_ground = false;
                }
            }
            else if (y_val < 0)
            {
                if (gMap.tile[y1][x1] != BLANK_TILE || gMap.tile[y1][x2] != BLANK_TILE)
                {
                    y_pos = (y1 + 1) * TILE_SIZE;
                    y_val = 0;
                }
            }
        }
    }

    x_pos += x_val;
    y_pos += y_val;

    if (x_pos < 0)
    {
        x_pos = 0;
    }
    else if (x_pos + width_frame > gMap.max_x)
    {
        x_pos = gMap.max_x - width_frame - 1;
    }

    if (y_pos > gMap.max_y)
    {
        come_back_time = 20;
    }
}


void ThreatsObject::ImpMoveType(SDL_Renderer* screen)
{
    if (type_move == STATIC_THREAT)
    {
        ;
    }
    else if (type_move == MOVE_LEFT)
    {
        if (input_type.left == 1)
        {
            LoadImg("img//monster.png", screen);
        }
    }
    else if (type_move == FLYING_THREAT)
    {
        if (input_type.left == 1)
        {
            LoadImgFlying("img//flying_monster.png", screen);
        }
    }
    else if(type_move == MOVE_RIGHT)
    {
        if(input_type.right == 1)
        {
            LoadImgFlying("img//Boss_threat.png", screen);
        }
    }
    else
    {
        if (on_ground == true)
        {
            input_type.left = 1;
        }
    }
}
// hien thi ke dich ra man hinh
void ThreatsObject :: show(SDL_Renderer* des)
{
    if(come_back_time == 0)
    {
        rect_.x = x_pos - map_x;
        rect_.y = y_pos - map_y;
        frame ++;
        if(frame >= num_frame)
        {
            frame = 0;
        }
        SDL_Rect* currentClip = &frame_clip[frame];
        SDL_Rect rendQuad = {rect_.x , rect_.y , width_frame , height_frame};
        SDL_RenderCopy(des , p_object , currentClip , &rendQuad);

    }

}

// xu ly lop dan ban cua ke dich
void ThreatsObject:: InitBullet(BulletObject* p_bullet,SDL_Renderer * screen)
{

    if(p_bullet != NULL)
    {
        p_bullet->set_bullet_type(BulletObject :: FIRE_BALL);

        bool res = p_bullet->LoadImageBullet(screen);
        if(res)
        {
        p_bullet->set_is_move(true);
        p_bullet->set_bullet_dir(BulletObject :: DIR_LEFT);
        p_bullet->SetRect(rect_.x + 5 , y_pos + 20);
        p_bullet->set_x_val(15);
        bullet_list.push_back(p_bullet);
        }
    }

}
void ThreatsObject:: MakeBullet(SDL_Renderer * screen , const int& x_limit , const int& y_limit)
{

    for(int i = 0 ;i < bullet_list.size() ;i++ )
    {
        BulletObject* p_bullet = bullet_list.at(i);
        if(p_bullet != NULL)
        {
            if(p_bullet->get_is_move())
            {
                int bullet_distancce = rect_.x + width_frame - p_bullet->GetRect().x; // do khoan cach giua dan va nhan vat
                if(bullet_distancce < 300 && bullet_distancce > 0)
                {


                p_bullet->HandleMove(x_limit, y_limit);
                p_bullet->Render(screen);
                }
                else{
                    p_bullet->set_is_move(false);
                }
            }
            else // neu dan khong di chuyen thi chuyen trang thai thanh true de dan di chuyen
            {
                p_bullet->set_is_move(true);
                p_bullet->SetRect(rect_.x + 5 , y_pos + 20);
            }
        }
    }
}


void ThreatsObject :: removeBullet(const int& idx)
{
    int size = bullet_list.size();
    if(size > 0 && idx < size )
    {
        BulletObject * p_bullet = bullet_list.at(idx);
        bullet_list.erase(bullet_list.begin() +idx);

        if(p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}







