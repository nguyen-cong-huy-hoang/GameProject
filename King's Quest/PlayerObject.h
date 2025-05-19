#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "BulletObject.h"
#include <iostream>
#include <vector>

#define MAX_FRAME 8
#define MAX_FRAME_JUMP 2
#define MAX_FRAME_ATTACK 4
#define PLAYER_JUMP_VAL 14
#define GRAVITY_SPEED 20
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8

class PlayerObject : public BaseObject
{
public:
    PlayerObject();
    ~PlayerObject();

    enum WalkType
    {
        WALK_RIGHT = 0,
        WALK_LEFT = 1,
        IDLE_RIGHT = 2,
        IDLE_LEFT = 3,
        ATTACK_RIGHT = 4,
        ATTACK_LEFT = 5,
    };



    // Tải ảnh nhân vật từ đường dẫn
    bool LoadImg(std::string path, SDL_Renderer *screen);
    bool LoadImgJump(std::string path, SDL_Renderer *screen);
    bool LoadImgAttack(std:: string path , SDL_Renderer * screen);

    // Hiển thị nhân vật lên màn hình
    void show(SDL_Renderer *des);

    // Xử lý sự kiện nhập từ bàn phím
    void HandelInputAction(SDL_Event event, SDL_Renderer *screen);

    // Thiết lập frame cho ảnh động nhân vật
    void set_clips();

    // Cập nhật chuyển động nhân vật và kiểm tra va chạm với bản đồ
    void DoPlayer(Map &map_data);

    // Kiểm tra va chạm giữa nhân vật và bản đồ
    void CheckToMap(Map &map_data);

    // Đặt vị trí camera theo vị trí nhân vật
    void CenterEnityOnMap(Map &map_data);

    // Lấy hình chữ nhật khung hình hiện tại của nhân vật
    SDL_Rect GetRectFrame();

    // Thiết lập danh sách đạn đang tồn tại của nhân vật
    void set_bullet_list(std::vector<BulletObject *> bullet_list) { p_bullet_list = bullet_list; }

    // Lấy danh sách đạn của nhân vật
    std::vector<BulletObject *> get_bullet_list() const { return p_bullet_list; }

    // Xử lý chuyển động và hiển thị đạn
    void HandleBullet(SDL_Renderer *des);

    // Xóa đạn khỏi danh sách theo chỉ số
    void removeBullet(const int &idx);

    // Thiết lập vị trí của map hiện tại
    void setMapXY(const int map_x_, const int map_y_) { map_x = map_x_; map_y = map_y_; }
    //hàm rest nhân vật
    void ResetPlayer();

    void set_check_time_skill(bool time_skill_) {check_time_skill = time_skill_;}
    bool get_check_time_skill() const  {return check_time_skill ;}



private:
    int money_count; // Số tiền thu thập được
    std::vector<BulletObject *> p_bullet_list; // Danh sách đạn nhân vật bắn ra

    float x_val; // Tốc độ di chuyển theo trục X
    float y_val; // Tốc độ di chuyển theo trục Y
    float x_pos; // Vị trí hiện tại theo trục X
    float y_pos; // Vị trí hiện tại theo trục Y

    int num_frame;
    int width_frame; // Chiều rộng 1 frame
    int height_frame; // Chiều cao 1 frame
    SDL_Rect frame_clip[MAX_FRAME]; // Mảng các khung hình animation

    Input input_type; // Trạng thái đầu vào từ bàn phím
    int frame; // Frame hiện tại
    int status; // Trạng thái (WALK_LEFT, WALK_RIGHT,...)
    bool on_ground; // Kiểm tra nhân vật có đang đứng trên mặt đất không

    int map_x; // Tọa độ bản đồ trục X
    int map_y; // Tọa độ bản đồ trục Y
    bool is_attacking; //kiểm tra xem có đang tấn công hay không
    bool check_time_skill;//thời gian hồi  skill


};

#endif // PLAYER_OBJECT_H
