
#include "PlayerObject.h"


PlayerObject :: PlayerObject()
{
    frame = 0;
    x_pos = 0;
    y_pos = 0;
    x_val = 0;
    y_val = 0;
    width_frame = 0;
    height_frame = 0;
    status = -1;
    input_type.down = 0;
    input_type.left = 0;
    input_type.right = 0;
    input_type.up = 0;
    on_ground = false;
    map_x = 0;
    map_y = 0;
    money_count = 0;
}


PlayerObject :: ~PlayerObject(){}

bool PlayerObject ::LoadImg(std:: string path , SDL_Renderer * screen)
{
    bool ret = BaseObject :: LoadImg(path , screen);
    if(ret == true)
    {

        width_frame = rect_.w/8;
        height_frame = rect_.h;
    }
    return ret;
}

void PlayerObject :: set_clips()
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

void PlayerObject :: show(SDL_Renderer* des)
{
    if(on_ground == true)
    {
    if(status == WALK_LEFT)
    {

        LoadImg("img//player_left.png" , des);
    }
    else{
        LoadImg("img//player_right.png", des);
    }
}

    if(input_type.left == 1 || input_type.right == 1)
    {
        frame++;
    }
    else
    {
        frame = 0;
    }
    if(frame >=8)
    {
        frame = 0;
    }

    rect_.x = x_pos - map_x ;
    rect_.y = y_pos - map_y;

    SDL_Rect * current_clip = &frame_clip[frame];

    SDL_Rect rendrQuad = {rect_.x , rect_.y , width_frame , height_frame};

    SDL_RenderCopy(des , p_object , current_clip , &rendrQuad);

}


void PlayerObject :: HandelInputAction(SDL_Event event , SDL_Renderer * screen)
{
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_d:
            {
                status = WALK_RIGHT;
                input_type.right  =1;
                input_type.left = 0;
                if(on_ground == true)
                {
                    LoadImg("img//player_right.png" , screen);
                }
                else{
                    LoadImg("img//jum_right.png" , screen);
                }

            }
            break;
        case SDLK_a:
            {
                status =  WALK_LEFT;
                input_type.left = 1;
                input_type.right =0;
                if(on_ground == true)
                {
                    LoadImg("img//player_left.png" , screen);
                }
                else{
                    LoadImg("img//jum_left.png" , screen);
                }
            }
             break;
         case SDLK_w:
                if(on_ground)
                {
                    input_type.up = 1;
                    if( input_type.right  == 1)
                    LoadImg("img//jum_right.png" , screen);
                    else LoadImg("img//jum_left.png" , screen);
                }

            break;
         case SDLK_j:
            {
            BulletObject* p_bullet = new BulletObject();

            p_bullet->LoadImageBullet(screen);
            if(status == WALK_LEFT)
            {
                p_bullet->set_bullet_dir(BulletObject :: DIR_LEFT);
                p_bullet->SetRect(this->rect_.x +20  , this->rect_.y + height_frame*0.3);
            }
            else if(status == WALK_RIGHT)
            {
                p_bullet->set_bullet_dir(BulletObject :: DIR_RIGHT);
                 p_bullet->SetRect(this->rect_.x + width_frame - 20 , this->rect_.y + height_frame*0.3);
            }


            p_bullet->set_x_val(20);
            p_bullet->set_is_move(true);
            p_bullet_list.push_back(p_bullet);
            }
        }

    }

    else if(event.type == SDL_KEYUP)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_d:
            {
                input_type.right  =0;

            }
            break;
        case SDLK_a:
            {

                input_type.left = 0;
            }
             break;
        }

    }



}

void PlayerObject:: HandleBullet(SDL_Renderer* des)
{
    for(size_t i = 0 ; i < p_bullet_list.size() ; i++)
    {
        BulletObject* p_bullet = p_bullet_list.at(i);
        if(p_bullet->get_is_move() == true)
        {
            p_bullet->HandleMove(SCREEN_WIDTH , SCREEN_HEIGHT); // dan di ra khoi man hinh thi xoa
            p_bullet->Render(des);
        }
        else
        {
            p_bullet_list.erase(p_bullet_list.begin() + i);
            if(p_bullet != NULL)
            {
                delete p_bullet;
                p_bullet = NULL;
            }
        }
    }
}
void PlayerObject :: DoPlayer(Map& map_data)
{
    x_val = 0;
    y_val += 0.8;

    if(y_val >= MAX_FALL_SPEED)
    {
        y_val = MAX_FALL_SPEED;
    }
    if(input_type.left ==1)
    {
        x_val -= PLAYER_SPEED;
    }
    if(input_type.right ==1)
    {
        x_val += PLAYER_SPEED;
    }
    if(input_type.up == 1 && on_ground)
    {
        y_val = - PLAYER_JUMP_VAL;
        on_ground = false;
        input_type.up = 0;
    }

        CheckToMap(map_data);
        CenterEnityOnMap(map_data);
}

void PlayerObject :: CenterEnityOnMap(Map& map_data)
{
    map_data.start_x = x_pos - (SCREEN_WIDTH / 2);
    if(map_data.start_x < 0)
    {
        map_data.start_x = 0;
    }
    else if(map_data.start_x + SCREEN_WIDTH >= map_data.max_x)
    {
        map_data.start_x = map_data.max_x - SCREEN_WIDTH;
    }

    map_data.start_y = y_pos - (SCREEN_HEIGHT / 2);
    if(map_data.start_y < 0)
    {
        map_data.start_y = 0;
    }
    else if(map_data.start_y + SCREEN_HEIGHT >= map_data.max_y)
    {
        map_data.start_y = map_data.max_y - SCREEN_HEIGHT;

    }

}

 void PlayerObject :: increasemoney()
 {
     money_count++;
 }

void PlayerObject :: CheckToMap(Map& map_data)
{
    int x1 = 0; // vi tri cot ben trai cua nhan vat
    int x2 = 0; // vi tri cot ben phai cua nhan vat
    int y1 = 0; // vi tri hang ben tren cua nhan vat
    int y2 = 0; // vi tri hang ben duoi cua nhan vat

    // Kiem tra va cham ngang
    int height_min = height_frame > TILE_SIZE ? height_frame : TILE_SIZE;

    x1 = (x_pos + x_val) / TILE_SIZE;
    x2 = (x_pos + width_frame + x_val - 1) / TILE_SIZE;
    y1 = y_pos / TILE_SIZE;
    y2 = (y_pos + height_min - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val > 0) // Di chuyen sang phai
        {

            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

            if(val1 == 4 || val2 == 4)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                increasemoney();
            }
            else{
                 if (map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
                {

                    x_pos = x2 * TILE_SIZE - (width_frame+ 1); // Dung ngay truoc vat can
                    x_val = 0;
                }
            }

        }
        else if (x_val < 0) // Di chuyen sang trai
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

            if(val1 == 4 || val2 == 4)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                increasemoney();
            }
            else{
                if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE)
                {
                    x_pos = (x1 + 1) * TILE_SIZE; // Dung ngay sau vat can
                    x_val = 0;
                }
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
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];
              if(val1 == 4 || val2 == 4)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                increasemoney();
            }
            else{
                 if (map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
                {
                    y_pos = y2 * TILE_SIZE - (height_frame+1); // Dung lai tren san
                    y_val = 0;
                    on_ground = true;
                }
            }
        }
        else if (y_val < 0) // Nhay len
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if(val1 == 4 || val2 == 4)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                increasemoney();
            }
            else{
                if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
                {
                    y_pos = (y1 + 1) * TILE_SIZE; // Cham tran
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
    else if (x_pos + width_frame > map_data.max_x)
    {
        x_pos = map_data.max_x - width_frame - 1;
    }
}

















