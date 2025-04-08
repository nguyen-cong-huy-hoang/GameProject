

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
    type_move = STATIC_THREAT;

}

ThreatsObject :: ~ThreatsObject()
{

}

bool ThreatsObject :: LoadImg(std :: string path , SDL_Renderer* screen)
{

    bool ret = BaseObject :: LoadImg(path , screen);

    if (!ret) {
     printf("Error loading image: %s\n", SDL_GetError());
    }
    if(ret)
    {
        width_frame = rect_.w / THREAT_FRAME_NUM;
        height_frame = rect_.h;
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

        frame_clip[0].x = 0;
        frame_clip[0].y = 0;
        frame_clip[0].w = width_frame;
        frame_clip[0].h = height_frame;

        frame_clip[1].x = width_frame;
        frame_clip[1].y = 0;
        frame_clip[1].w = width_frame;
        frame_clip[1].h = height_frame;

        frame_clip[2].x = 2*width_frame;
        frame_clip[2].y = 0;
        frame_clip[2].w = width_frame;
        frame_clip[2].h = height_frame;

        frame_clip[3].x = 3*width_frame;
        frame_clip[3].y = 0;
        frame_clip[3].w = width_frame;
        frame_clip[3].h = height_frame;

        frame_clip[4].x = 4*width_frame;
        frame_clip[4].y = 0;
        frame_clip[4].w = width_frame;
        frame_clip[4].h = height_frame;

        frame_clip[5].x = 5*width_frame;
        frame_clip[5].y = 0;
        frame_clip[5].w = width_frame;
        frame_clip[5].h = height_frame;

        frame_clip[6].x = 6*width_frame;
        frame_clip[6].y = 0;
        frame_clip[6].w = width_frame;
        frame_clip[6].h = height_frame;

        frame_clip[7].x = 7*width_frame;
        frame_clip[7].y = 0;
        frame_clip[7].w = width_frame;
        frame_clip[7].h = height_frame;
    }




}


void ThreatsObject :: show(SDL_Renderer* des)
{
    if(come_back_time == 0)
    {
        rect_.x = x_pos - map_x;
        rect_.y = y_pos - map_y;
        frame ++;
        if(frame >= 8)
        {
            frame = 0;
        }
        SDL_Rect* currentClip = &frame_clip[frame];
        SDL_Rect rendQuad = {rect_.x , rect_.y , width_frame , height_frame};
        SDL_RenderCopy(des , p_object , currentClip , &rendQuad);

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


void ThreatsObject :: DoPlayer(Map& gMap)
{

    if(come_back_time == 0)
    {
        x_val = 0;
        y_val += THREAT_GRAVITY_SPEED;

        if(y_val >= THREAT_MAX_FALL_SPEED)
        {
            y_val = THREAT_MAX_FALL_SPEED;
        }
        if(input_type.left == 1)
        {
            x_val -= THREAT_SPEED;
        }
        else if(input_type.right == 1)
        {
             x_val += THREAT_SPEED;
        }
        checkToMap(gMap);
    }
    else if(come_back_time >0 )
    {
        come_back_time --;
        if(come_back_time == 0)
        {
        x_val = 0;
        y_val = 0;
        if(x_pos > 256)
        {
            x_pos -= 256;
            animation_a -= 256;
            animation_b -= 256;
        }
        else{
            x_pos = 0;
        }
        int tile_y = (SCREEN_HEIGHT / TILE_SIZE) - 2;
        int ground_y = tile_y * TILE_SIZE;
        y_pos = ground_y - get_height_frame();
        come_back_time = 0;
        input_type.left = 1;
        }
    }
}

void ThreatsObject :: checkToMap(Map& gMap)
{
    int x1 = 0; // vi tri cot ben trai cua nhan vat
    int x2 = 0; // vi tri cot ben phai cua nhan vat
    int y1 = 0; // vi tri hang ben tren cua nhan vat
    int y2 = 0; // vi tri hang ben duoi cua nhan vat

    // Kiem tra va cham ngang
    int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE;

    x1 = (x_pos + x_val) / TILE_SIZE;
    x2 = (x_pos + width_frame + x_val - 1) / TILE_SIZE;
    y1 = y_pos / TILE_SIZE;
    y2 = (y_pos + height_min - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val > 0) // Di chuyen sang phai
        {
            if (gMap.tile[y1][x2] != BLANK_TILE || gMap.tile[y2][x2] != BLANK_TILE)
            {

                x_pos = x2 * TILE_SIZE - (width_frame+ 1); // Dung ngay truoc vat can

                x_val = 0;

            }
        }
        else if (x_val < 0) // Di chuyen sang trai
        {
            if (gMap.tile[y1][x1] != BLANK_TILE || gMap.tile[y2][x1] != BLANK_TILE)
            {
                x_pos = (x1 + 1) * TILE_SIZE; // Dung ngay sau vat can

                x_val = 0;

            }

        }
    }

    // Kiem tra va cham doc
    int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
    x1 = x_pos / TILE_SIZE;
    x2 = (x_pos + width_min) / TILE_SIZE;
    y1 = (y_pos + y_val) / TILE_SIZE;
    y2 = (y_pos + y_val + height_frame - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val > 0) // Roi xuong
        {
            if (gMap.tile[y2][x1] != BLANK_TILE || gMap.tile[y2][x2] != BLANK_TILE)
            {
                y_pos = y2 * TILE_SIZE - (height_frame+1); // Dung lai tren san
                y_val = 0;
                on_ground = true;
            }

        }
        else if (y_val < 0) // Nhay len
        {
            if (gMap.tile[y1][x1] != BLANK_TILE || gMap.tile[y1][x2] != BLANK_TILE)
            {
                y_pos = (y1 + 1) * TILE_SIZE; // Cham tran
                y_val = 0;
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
    if(y_pos > gMap.max_y)
    {
        come_back_time = 60;
    }
}


void ThreatsObject :: ImpMoveType(SDL_Renderer * screen)
{
    if(type_move == STATIC_THREAT)
    {
        ;
    }
    else{
        if(on_ground == true)
        {
            if(x_pos > animation_b )
            {

                input_type.left = 1;
                input_type.right = 0;
                LoadImg("img//test1.png" , screen);

            }
            else if(x_pos < animation_a )
            {

                input_type.left = 0;
                input_type.right = 1;
                LoadImg("img//test2.png" , screen);

            }
        }
        else{
            if(input_type.left == 1)
            {
                LoadImg("img//test1.png" , screen);
            }
        }
    }


}

void ThreatsObject:: InitBullet(BulletObject* p_bullet,SDL_Renderer * screen)
{

    if(p_bullet != NULL)
    {
        p_bullet->set_bullet_type(BulletObject :: LASER_BULLET);

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
                int bullet_distancce = rect_.x + width_frame - p_bullet->GetRect().x;
                if(bullet_distancce < 300 && bullet_distancce > 0)
                {


                p_bullet->HandleMove(x_limit, y_limit);
                p_bullet->Render(screen);
                }
                else{
                    p_bullet->set_is_move(false);
                }
            }
            else
            {
                p_bullet->set_is_move(true);
                p_bullet->SetRect(rect_.x + 5 , y_pos + 20);
            }
        }
    }
}






