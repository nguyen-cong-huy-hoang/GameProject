
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
    num_frame = 0;
    status = IDLE_RIGHT;
    input_type.down = 0;
    input_type.left = 0;
    input_type.right = 0;
    input_type.up = 0;
    on_ground = false;
    map_x = 0;
    map_y = 0;
    check_time_skill = false;
    skill_activation_time = 0;
    is_attacking = false;
    money_count = 0;
}


PlayerObject :: ~PlayerObject(){}

// load anh va setup frame

bool PlayerObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret == true)
    {
        num_frame = MAX_FRAME;
        width_frame = rect_.w / MAX_FRAME;
        height_frame = rect_.h;
        set_clips();
    }
    return ret;
}

bool PlayerObject::LoadImgAttack(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret == true)
    {
        num_frame = MAX_FRAME_ATTACK;
        width_frame = rect_.w / MAX_FRAME_ATTACK;
        height_frame = rect_.h;
        set_clips();
    }

    return ret;
}

bool PlayerObject::LoadImgJump(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret == true)
    {
        num_frame = MAX_FRAME_JUMP;
        width_frame = rect_.w / MAX_FRAME_JUMP;
        height_frame = rect_.h;
        set_clips();
    }
    return ret;
}


void PlayerObject :: set_clips()
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
SDL_Rect PlayerObject :: GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame;
    rect.h = height_frame;

    return rect;
}


// xu ly input nguoi dung
void PlayerObject::HandelInputAction(SDL_Event event, SDL_Renderer* screen)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_d:
        {
            // Chỉ xử lý phím 'd' nếu không đang tấn công
            if (!is_attacking)
            {
                status = WALK_RIGHT;
                input_type.right = 1;
                input_type.left = 0;
                if (on_ground == true)
                {
                    LoadImg("img//run_right.png", screen);
                }
                else
                {
                    LoadImgJump("img//jump_right.png", screen);
                }
            }
        }
        break;
        case SDLK_a:
        {
            // Chỉ xử lý phím 'a' nếu không đang tấn công
            if (!is_attacking)
            {
                status = WALK_LEFT;
                input_type.left = 1;
                input_type.right = 0;
                if (on_ground == true)
                {
                    LoadImg("img//run_left.png", screen);
                }
                else
                {
                    LoadImgJump("img//jump_left.png", screen);
                }
            }
        }
        break;
        case SDLK_w:
        {
            // Chỉ xử lý phím 'w' nếu không đang tấn công
            if (!is_attacking && on_ground)
            {
                input_type.up = 1;
                if (input_type.right == 1)
                    LoadImgJump("img//jump_right.png", screen);
                else
                    LoadImgJump("img//jump_left.png", screen);
            }
        }
        break;

        case SDLK_j:
        {
            // Chỉ cho phép tấn công nếu không đang tấn công
            if (!is_attacking)
            {
                BulletObject* p_bullet = new BulletObject();
                p_bullet->LoadImageSkill("img//defensive_skill.png" ,screen);
                p_bullet->set_bullet_type(BulletObject :: DEFENSIVE_SKILL);
                p_bullet->set_bullet_dir(BulletObject :: DIR_ALL);
                int effect_size = 250;
                p_bullet->SetRect(this->rect_.x - (effect_size - width_frame) / 2,
                                  this->rect_.y - (effect_size - height_frame) / 2);
             if (status == IDLE_RIGHT || status == WALK_RIGHT)
                {
                    status = ATTACK_RIGHT;
                    input_type.right = 0;
                    input_type.left = 0;
                    if (LoadImgAttack("img//Attack2_right.png", screen))
                    {
                        frame = 0;
                        is_attacking = true;
                    }
                }
                else if (status == IDLE_LEFT || status == WALK_LEFT)
                {
                    status = ATTACK_LEFT;
                    input_type.right = 0;
                    input_type.left = 0;
                    if (LoadImgAttack("img//Attack2_left.png", screen))
                    {
                        frame = 0;
                        is_attacking = true;
                    }
                }

            p_bullet->set_is_move(true);
            p_bullet_list.push_back(p_bullet);
            }
        }
        break;
        case SDLK_k:
        {
            if(check_time_skill == false)
            {
            // Chỉ cho phép tấn công nếu không đang tấn công
           if (!is_attacking)
            {
                BulletObject* p_bullet = new BulletObject();
                if (status == IDLE_RIGHT || status == WALK_RIGHT)
                {
                    p_bullet->LoadImageSkill("img//skill_effect_right.png",screen);
                    p_bullet->set_bullet_type(BulletObject :: EFFECT_SKILL);
                    p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
                    p_bullet->SetRect(this->rect_.x + width_frame - 20, this->rect_.y + height_frame * 0.001);
                    status = ATTACK_RIGHT;
                    input_type.right = 0;
                    input_type.left = 0;
                    if (LoadImgAttack("img//Attack3_right.png", screen))
                    {
                        frame = 0;
                        is_attacking = true;
                    }
                }
                else if (status == IDLE_LEFT || status == WALK_LEFT)
                {
                    p_bullet->LoadImageSkill("img//skill_effect_left.png",screen);
                    p_bullet->set_bullet_type(BulletObject :: EFFECT_SKILL);
                    p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                    p_bullet->SetRect(this->rect_.x + 20, this->rect_.y + height_frame * 0.001);
                    status = ATTACK_LEFT;
                    input_type.right = 0;
                    input_type.left = 0;
                    if (LoadImgAttack("img//Attack3_left.png", screen))
                    {
                        frame = 0;
                        is_attacking = true;
                    }
                }
            p_bullet->set_x_val(20);
            p_bullet->set_is_move(true);
            p_bullet_list.push_back(p_bullet);
            }
            check_time_skill = true;
            skill_activation_time = SDL_GetTicks() / 1000;
        }

    }
        break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_d:
        {
            // Chỉ xử lý nếu không đang tấn công
            if (!is_attacking)
            {
                status = IDLE_RIGHT;
                input_type.right = 0;
                LoadImg("img//idle_right.png", screen);
            }
        }
        break;
        case SDLK_a:
        {
            // Chỉ xử lý nếu không đang tấn công
            if (!is_attacking)
            {
                status = IDLE_LEFT;
                input_type.left = 0;
                LoadImg("img//idle_left.png", screen);
            }
        }
        break;
        }
    }
}

//Di chuyen va xu ly va cham voi ban do

void PlayerObject :: DoPlayer(Map& map_data)
{
    if(x_pos == 0 && y_pos == 0)
    {
        x_pos = SCREEN_WIDTH/2;
    }
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

void PlayerObject::CheckToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    // Kiểm tra va chạm ngang
    int height_min = height_frame > TILE_SIZE ? height_frame : TILE_SIZE;

    x1 = (x_pos + x_val) / TILE_SIZE;
    x2 = (x_pos + x_val + width_frame - 1) / TILE_SIZE;
    y1 = y_pos / TILE_SIZE;
    y2 = (y_pos + height_min - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val > 0) // Sang phải
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];


            if (map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
            {
                x_pos = x2 * TILE_SIZE - (width_frame + 1);
                x_val = 0;
            }
        }
        else if (x_val < 0) // Sang trái
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];


             if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE)
            {
                x_pos = (x1 + 1) * TILE_SIZE;
                x_val = 0;
            }
        }
    }

    // Kiểm tra va chạm dọc (nhảy/ rơi)
    int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
    x1 = x_pos / TILE_SIZE;
    x2 = (x_pos + width_min) / TILE_SIZE;
    y1 = (y_pos + y_val) / TILE_SIZE;
    y2 = (y_pos + y_val + height_frame - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val > 0) // Rơi
        {
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];


            if (map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
            {
                y_pos = y2 * TILE_SIZE - (height_frame + 1); // Đảm bảo nhân vật nằm đúng trên mặt đất
                y_val = 0;
                on_ground = true;
            }
        }
        else if (y_val < 0) // Nhảy
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];


            if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
            {
                y_pos = (y1 + 1) * TILE_SIZE;
                y_val = 0;
            }
        }
    }

    x_pos += x_val;
    y_pos += y_val;

    if (x_pos < 0)
        x_pos = 0;
    else if (x_pos + width_frame > map_data.max_x)
        x_pos = map_data.max_x - width_frame - 1;
}

void PlayerObject :: CenterEnityOnMap(Map& map_data)
{
    // Tính toán vị trí bắt đầu của bản đồ để giữ nhân vật ở trung tâm
    map_data.start_x = x_pos - (SCREEN_WIDTH / 3);
    map_data.start_y = y_pos - (SCREEN_HEIGHT / 2);

    // Đảm bảo start_x không vượt quá giới hạn
    if (map_data.start_x < 0)
    {
        map_data.start_x = 0;
    }
    else if (map_data.start_x + SCREEN_WIDTH > map_data.max_x)
    {
        map_data.start_x = map_data.max_x - SCREEN_WIDTH;
    }

    // Đảm bảo start_y không vượt quá giới hạn
    if (map_data.start_y < 0)
    {
        map_data.start_y = 0;
    }
    else if (map_data.start_y + SCREEN_HEIGHT > map_data.max_y)
    {
        map_data.start_y = map_data.max_y - SCREEN_HEIGHT;
    }

    // Xử lý trường hợp bản đồ nhỏ hơn màn hình
    if (map_data.max_x < SCREEN_WIDTH)
    {
        map_data.start_x = 0; // Không cuộn nếu bản đồ nhỏ hơn màn hình
    }
    if (map_data.max_y < SCREEN_HEIGHT)
    {
        map_data.start_y = 0; // Không cuộn nếu bản đồ nhỏ hơn màn hình
    }
}

// hiển thị nhân vật
void PlayerObject::show(SDL_Renderer* des)
{
    // Lưu chiều cao khung hình cũ trước khi tải sprite mới
    int old_height_frame = height_frame;

    // Update frame
    if (input_type.left == 1 || input_type.right == 1 || status == ATTACK_LEFT || status == ATTACK_RIGHT || status == IDLE_LEFT || status == IDLE_RIGHT)
    {
        frame++;
    }

    if (frame >= num_frame)
    {
        // Sau khi Attack xong thì quay về Idle
        if (status == ATTACK_LEFT)
        {
            status = IDLE_LEFT;
            LoadImg("img//idle_left.png", des);
            is_attacking = false; // Reset trạng thái tấn công
        }
        else if (status == ATTACK_RIGHT)
        {
            status = IDLE_RIGHT;
            LoadImg("img//idle_right.png", des);
            is_attacking = false; // Reset trạng thái tấn công
        }
        if (on_ground && !is_attacking)
        {
            if (input_type.right == 1)
            {
                status = WALK_RIGHT;
                LoadImg("img//run_right.png", des);
            }
            else if (input_type.left == 1)
            {
                status = WALK_LEFT;
                LoadImg("img//run_left.png", des);
            }
        }
        // Điều chỉnh y_pos để bù lại sự thay đổi chiều cao khung hình
        int new_height_frame = height_frame;
        if (on_ground)
        {
            y_pos += (old_height_frame - new_height_frame);
        }

        frame = 0;
    }

    rect_.x = x_pos - map_x;
    rect_.y = y_pos - map_y;

    SDL_Rect* current_clip = &frame_clip[frame];

    SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame, height_frame };

    SDL_RenderCopy(des, p_object, current_clip, &renderQuad);
}


// xu ly dan

void PlayerObject:: HandleBullet(SDL_Renderer* des)
{
    for(size_t i = 0 ; i < p_bullet_list.size() ; i++)
    {
        BulletObject* p_bullet = p_bullet_list.at(i);
        if(p_bullet->get_is_move() == true)
        {
            if(p_bullet->get_bullet_type() == BulletObject :: EFFECT_SKILL)
            {
                p_bullet->HandleMove(SCREEN_WIDTH , SCREEN_HEIGHT); // dan di ra khoi man hinh thi xoa
            }
            else if(p_bullet->get_bullet_type() == BulletObject:: DEFENSIVE_SKILL)
            {
                 p_bullet->HandleMove(SCREEN_WIDTH , SCREEN_HEIGHT);
            }
            else{
                p_bullet->HandleMove(500 , SCREEN_HEIGHT);
            }
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


void PlayerObject :: removeBullet(const int& idx)
{
    int size = p_bullet_list.size();
    if(size > 0 && idx < size )
    {
        BulletObject * p_bullet = p_bullet_list.at(idx);
        p_bullet_list.erase(p_bullet_list.begin() +idx);

        if(p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }

    }


}

void PlayerObject :: ResetPlayer(){
       x_val = 0;
       y_val = 0;

       x_pos = 0;
       y_pos = 0;
       input_type.right =0;
       input_type.left =0;
        // Giải phóng và xóa danh sách đạn
        for (size_t i = 0; i < p_bullet_list.size(); i++) {
            BulletObject* p_bullet = p_bullet_list.at(i);
            if (p_bullet) {
                delete p_bullet;
                p_bullet = NULL;
            }
        }
        p_bullet_list.clear();

        // Reset vị trí bản đồ
    map_x = 0;
    map_y = 0;
    check_time_skill = false;
    skill_activation_time = 0;
    }

















