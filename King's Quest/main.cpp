#include "CommonFunc.h"
#include "GameMap.h"
#include "Time.h"
#include "PlayerObject.h"
#include "ThreatsObject.h"
#include "TextObject.h"
#include "ButtonObject.h"
#include "SoundManager.h"
#include "SliderObject.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>

BaseObject loadBackground;
BaseObject loadGameOver;
TTF_Font* font = NULL;
TextObject titleText;
TextObject startText;
TextObject Options;
TextObject time_game;
TextObject volume_text;
TextObject HighScore;
TextObject Score;
ButtonObject music_button;
ButtonObject restart_button;
SliderObject volume_slider;
float max_x_pos_fly = 0;
float max_x_pos_dynamic = 0;
int high_score = 0;
int skill_activation_time = 0;
int last_time_val = 0;
Time game_timer;


bool InitData() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        logErrorAndExit("SDL could not initialize", SDL_GetError());
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        logErrorAndExit("SDL_image could not initialize", IMG_GetError());
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("King Quest",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);
    if (!g_window) {
        logErrorAndExit("Create Window", SDL_GetError());
        return false;
    }
    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (!g_screen) {
        logErrorAndExit("Create Renderer", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
    if (TTF_Init() == -1) return false;
    else {
        font = TTF_OpenFont("font//dlxfont_.ttf", 15);
        if (font == NULL) {
            return false;
        }
    }

    return true;
}



void close() {
    loadBackground.Free();
    loadGameOver.Free();
    titleText.Free();
    startText.Free();
    Options.Free();
    time_game.Free();
    volume_text.Free();
    HighScore.Free();
    Score.Free();
    music_button.Free();
    volume_slider.Free();
    TTF_CloseFont(font);
    font = nullptr;
    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);
    g_window = nullptr;
    g_screen = nullptr;
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
std::vector<ThreatsObject*> MakeThreatdList() {
    std::vector<ThreatsObject*> list_threats;

    ThreatsObject* flying_threats = new ThreatsObject[10];
    for (int i = 0; i < 10; i++) {
        ThreatsObject* p_threat = (flying_threats + i);
        if (p_threat != NULL) {
            p_threat->LoadImg("img//flying_monster.png", g_screen);
            p_threat->set_type_move(ThreatsObject::FLYING_THREAT);
            p_threat->set_x_pos(1200*(i+1));
            max_x_pos_fly = std :: max(max_x_pos_fly , p_threat->get_x_pos());
            int tile_y = (SCREEN_HEIGHT / TILE_SIZE) - 1;
            int ground_y = tile_y * TILE_SIZE;
            p_threat->set_y_pos(ground_y - p_threat->get_height_frame() - TILE_SIZE / 2);
            p_threat->set_input_left(1);
            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);
            list_threats.push_back(p_threat);
        }
    }

    ThreatsObject* dynamic_threats = new ThreatsObject[10];
    for (int i = 0; i < 10; i++) {
        ThreatsObject* p1_threat = (dynamic_threats + i);
        if (p1_threat != NULL) {
            p1_threat->LoadImg("img//monster.png", g_screen);
            p1_threat->set_type_move(ThreatsObject::MOVE_LEFT);
            p1_threat->set_x_pos(1500*(i+1));
            max_x_pos_dynamic = std :: max(max_x_pos_dynamic , p1_threat->get_x_pos());
            int tile_y = (SCREEN_HEIGHT / TILE_SIZE) - 1;
            int ground_y = tile_y * TILE_SIZE;
            p1_threat->set_y_pos(ground_y - p1_threat->get_height_frame());
            p1_threat->set_input_left(1);
            BulletObject* p1_bullet = new BulletObject();
            p1_threat->InitBullet(p1_bullet, g_screen);
            list_threats.push_back(p1_threat);
        }
    }
    return list_threats;
}

ThreatsObject* MakeBossObject() {
    ThreatsObject* boss_threat = new ThreatsObject();
    if (!boss_threat->LoadImgBoss("img//Boss_threat.png", g_screen)) {
        std::cout << "Failed to load Boss_threat.png: " << SDL_GetError() << std::endl;
    }
    boss_threat->set_type_move(ThreatsObject::MOVE_RIGHT);
    boss_threat->set_x_pos(150);
    int tile_y = (SCREEN_HEIGHT / TILE_SIZE) - 1;
    int ground_y = tile_y * TILE_SIZE;
    boss_threat->set_y_pos(ground_y - boss_threat->get_height_frame());
    boss_threat->set_input_right(1);
    return boss_threat;
}

void ResetGame(std::vector<ThreatsObject*>& threats_list, PlayerObject& p_player,ThreatsObject*& boss_threat) {
    for (size_t i = 0; i < threats_list.size(); i++) {
        ThreatsObject* p_threat = threats_list.at(i);
        if (p_threat) {
            p_threat->Free();
            p_threat = NULL;
        }
    }
    threats_list.clear();
    max_x_pos_fly = 0;
    max_x_pos_dynamic = 0;
    threats_list = MakeThreatdList();
    p_player.ResetPlayer();
    p_player.set_check_time_skill(false);
    p_player.LoadImg("img//idle_right.png", g_screen);
    if (boss_threat) {
        boss_threat->Free();
        delete boss_threat;
    }
    boss_threat = MakeBossObject();
    skill_activation_time = 0;
    game_timer.start();
}

int main(int argc, char* argv[]) {
    bool check_continue = false;
    Time fps_timer;
    game_timer.start();
    if (!InitData()) return -1;
    loadBackground.LoadImg("img//king.png" , g_screen);
    loadGameOver.LoadImg("img//game_over.png",g_screen);

    bool music_state = true;
    if (!music_button.LoadImg("img//music_on.png", g_screen)) {
        std::cout << "Failed to load music_on: " << SDL_GetError() << std::endl;
        return -1;
    }
    music_button.SetRect(70, 10, 50, 50, "music");

    SoundManager sound_manager;
    if (!sound_manager.Init()) {
        std::cout << "Failed to initialize SoundManager!" << std::endl;
        return -1;
    }

    if (!sound_manager.LoadMusic("audio/background_music.mp3")) {
        std::cout << "Failed to load background music!" << std::endl;
    }
    if (!sound_manager.LoadSoundEffect("audio/button_click.wav", "button_click")) {
        std::cout << "Failed to load button click sound!" << std::endl;
    }
    if (!sound_manager.LoadSoundEffect("audio/game_over.wav", "game_over")) {
        std::cout << "Failed to load game over sound!" << std::endl;
    }

    sound_manager.PlayMusic(-1);

    volume_slider.SetRect(SCREEN_WIDTH/2 -50 , SCREEN_HEIGHT/2 - 50, 150, 20, 10, 20);
    if (!volume_slider.LoadImg("img//slider.png", g_screen)) {
        std::cout << "Failed to load slider image, using default rectangle" << std::endl;
    }
    volume_text.SetColor(TextObject::WHITE_TEXT);

    enum GameState { START_SCREEN, PLAYING, PAUSED , OPTIONS , GAME_OVER };
    GameState game_state = START_SCREEN;


    restart_button.SetRect(SCREEN_WIDTH/2 , SCREEN_HEIGHT/2 +25, 50 , 50 , "button");
    if (!restart_button.LoadImg("img//restart_button.png", g_screen)) {
        std::cout << "Failed to load restart_button: " << SDL_GetError() << std::endl;
        return -1;
    }

    ButtonObject setting_button;

    if (!setting_button.LoadImg("img//setting_button.png", g_screen)) {
        std::cout << "Failed to load setting_button: " << SDL_GetError() << std::endl;
        return -1;
    }

    ButtonObject home_button;
    if (!home_button.LoadImg("img//home_button.png", g_screen)) {
        std::cout << "Failed to load home_button: " << SDL_GetError() << std::endl;
        return -1;
    }

    ButtonObject pause_button;
    if (!pause_button.LoadImg("img//pause_button.png", g_screen)) {
        std::cout << "Failed to load pause_button: " << SDL_GetError() << std::endl;
        return -1;
    }
    pause_button.SetRect(10, 10, 50, 50, "pause");

    ButtonObject continue_button;
    if (!continue_button.LoadImg("img//continue_button.png", g_screen)) {
        std::cout << "Failed to load continue_button: " << SDL_GetError() << std::endl;
        return -1;
    }


    titleText.SetText("King Quest");
    titleText.SetColor(TextObject::WHITE_TEXT);
    if (!titleText.LoadFromRenderText(font, g_screen)) {
        std::cout << "Failed to load titleText: " << TTF_GetError() << std::endl;
        return -1;
    }

    startText.SetText("Start");
    startText.SetColor(TextObject::WHITE_TEXT);
    if (!startText.LoadFromRenderText(font, g_screen)) {
        std::cout << "Failed to load startText: " << TTF_GetError() << std::endl;
        return -1;
    }

    Options.SetText("Setting");
    Options.SetColor(TextObject::WHITE_TEXT);
    if (!Options.LoadFromRenderText(font, g_screen)) {
        std::cout << "Failed to load Options text: " << TTF_GetError() << std::endl;
        return -1;
    }

    HighScore.SetColor(TextObject :: WHITE_TEXT);
    Score.SetColor(TextObject :: WHITE_TEXT);

    GameMap loadmap;
    loadmap.LoadMap("map//map01_generated.dat");
    loadmap.LoadTiles(g_screen);
    PlayerObject p_player;
    p_player.LoadImg("img//idle_right.png", g_screen);

    std::vector<ThreatsObject*> threats_list = MakeThreatdList();
    ThreatsObject* boss_threat = MakeBossObject();

    time_game.SetColor(TextObject::WHITE_TEXT);


    int last_volume = -1;

    bool quit = false;
    while (!quit) {
        fps_timer.start();
        while (SDL_PollEvent(&g_event)) {
            if (g_event.type == SDL_QUIT) quit = true;
            if (g_event.type == SDL_KEYDOWN) {
                if (g_event.key.keysym.sym == SDLK_q) {
                    quit = true;
                }

            }

            if (game_state == START_SCREEN) {
                 loadBackground.LoadImg("img//king.png",g_screen);
                if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    SDL_Rect startButton = { (SCREEN_WIDTH - 150) / 2, SCREEN_HEIGHT - 250, 150, 50 };
                    if (x >= startButton.x && x <= startButton.x + startButton.w &&
                        y >= startButton.y && y <= startButton.y + startButton.h) {
                        game_state = PLAYING;
                        sound_manager.PlaySoundEffect("button_click");
                        sound_manager.LoadMusic("audio//battle.mp3");
                        if (music_state) sound_manager.PlayMusic(-1);
                        ResetGame(threats_list, p_player, boss_threat);
                    }

                    SDL_Rect optionButton = { (SCREEN_WIDTH - 150) / 2, SCREEN_HEIGHT - 180, 150, 50 };
                    if (x >= optionButton.x && x <= optionButton.x + optionButton.w &&
                        y >= optionButton.y && y <= optionButton.y + optionButton.h) {
                        check_continue = false;
                        game_state = OPTIONS;
                        sound_manager.PlaySoundEffect("button_click");
                    }
                }
                music_button.SetRect(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100, 50, 50, "music");
                if (music_button.CheckClick(g_event)) {
                    music_button.ToggleMusic(g_screen, &sound_manager, music_state);
                    sound_manager.PlaySoundEffect("button_click");
                }


            } else if (game_state == PLAYING) {
                loadBackground.LoadImg("img//night_town.png",g_screen);
                p_player.HandelInputAction(g_event, g_screen);
                if (pause_button.CheckClick(g_event)) {
                    if (pause_button.IsClicked()) {
                        game_state = PAUSED;
                        game_timer.paused(); // Tạm dừng bộ đếm
                        sound_manager.PauseMusic();
                    }
                }
            } else if (game_state == PAUSED) {
                continue_button.SetRect((SCREEN_WIDTH - 100) / 2, SCREEN_HEIGHT - 200, 50, 50, "continue");
                if (continue_button.CheckClick(g_event) && continue_button.IsClicked()) {
                    check_continue = false;
                    game_state = PLAYING;
                    game_timer.unpaused();
                    pause_button.ResetClicked();
                    continue_button.ResetClicked();
                    if (music_state && sound_manager.IsPaused()) {
                        sound_manager.ResumeMusic();
                    }

                }
                if(restart_button.CheckClick(g_event))
                {
                    ResetGame(threats_list, p_player, boss_threat);
                    game_state = PLAYING;
                    restart_button.ResetClicked();
                }
                if(home_button.CheckClick(g_event))
                {
                    check_continue = false;
                    game_state = START_SCREEN;
                    game_timer.stop();
                    last_time_val = 0;
                    home_button.ResetClicked();
                }
                home_button.SetRect(SCREEN_WIDTH/2 -50 , SCREEN_HEIGHT/2 + 75,50 , 50, "home");
                setting_button.SetRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 +75, 50 , 50 ,"setting");
                if(setting_button.CheckClick(g_event))
                {
                    check_continue = true;
                    game_state = OPTIONS;
                    setting_button.ResetClicked();
                }

            }
            else if(game_state == OPTIONS)
            {
                loadBackground.LoadImg("img//king.png",g_screen);
                volume_slider.HandleInput(g_event, &sound_manager);
                home_button.SetRect(SCREEN_WIDTH/2 , SCREEN_HEIGHT/2,50 , 50, "home");
                if(home_button.CheckClick(g_event))
                {
                    game_state = START_SCREEN;
                    home_button.ResetClicked();
                }
                if(check_continue == true)
                {

                    if(continue_button.CheckClick(g_event))
                    {

                        game_state = PAUSED;
                        continue_button.ResetClicked();
                    }
                }

            }
            else if (game_state == GAME_OVER) {
                home_button.SetRect(SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2 + 75, 50, 50, "home");
                if (home_button.CheckClick(g_event)) {
                    game_state = START_SCREEN;
                    game_timer.stop();
                    last_time_val = 0;
                    sound_manager.LoadMusic("audio//background_music.mp3");
                    if (music_state) sound_manager.PlayMusic(-1);
                    home_button.ResetClicked();
                }
                restart_button.SetRect(SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2 + 75, 50, 50, "restart");
                if (restart_button.CheckClick(g_event)) {
                    game_state = PLAYING;
                    sound_manager.LoadMusic("audio//battle.mp3");
                    if (music_state) sound_manager.PlayMusic(-1);
                    ResetGame(threats_list, p_player, boss_threat);
                    restart_button.ResetClicked();
                }
            }
        }


        SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
        SDL_RenderClear(g_screen);
        loadBackground.Render(g_screen);

        if (game_state == START_SCREEN) {
            titleText.RenderText(g_screen, (SCREEN_WIDTH - titleText.GetWidth()) / 2, 50);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_Rect startButton = { (SCREEN_WIDTH - 150) / 2, SCREEN_HEIGHT - 250, 150, 50 };
            SDL_RenderFillRect(g_screen, &startButton);
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            SDL_RenderDrawRect(g_screen, &startButton);
            startText.RenderText(g_screen, startButton.x + (startButton.w - startText.GetWidth()) / 2,
                                startButton.y + (startButton.h - startText.GetHeight()) / 2);

            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_Rect optionButton = { (SCREEN_WIDTH - 150) / 2, SCREEN_HEIGHT - 180, 150, 50 };
            SDL_RenderFillRect(g_screen, &optionButton);
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            SDL_RenderDrawRect(g_screen, &optionButton);
            Options.RenderText(g_screen, optionButton.x + (optionButton.w - Options.GetWidth()) / 2,
                              optionButton.y + (optionButton.h - Options.GetHeight()) / 2);

            music_button.Show(g_screen);


        }
        else if(game_state == OPTIONS)
        {
         volume_slider.Show(g_screen);
            int current_volume = sound_manager.GetVolume();
            if (current_volume != last_volume) {
                std::string volume_str = "Volume: " + std::to_string(current_volume * 100 / MIX_MAX_VOLUME) + "%";
                volume_text.SetText(volume_str);
                volume_text.LoadFromRenderText(font, g_screen);
                last_volume = current_volume;
            }
            continue_button.SetRect((SCREEN_WIDTH-100 ) / 2 , SCREEN_HEIGHT - 200, 50, 50, "continue");
            volume_text.RenderText(g_screen, SCREEN_WIDTH/2 -50,SCREEN_HEIGHT/2 -25 );
            home_button.Show(g_screen);
            if(check_continue == true)
            {
                continue_button.Show(g_screen);
            }

        }
        else if(game_state == GAME_OVER){
               loadGameOver.SetRect(SCREEN_WIDTH/2 -230, SCREEN_HEIGHT/2 - 350);
               loadGameOver.Render(g_screen);
               std :: string high_Score = "High Score: " + std :: to_string(high_score);
               HighScore.SetText(high_Score);
               HighScore.LoadFromRenderText(font , g_screen);
               HighScore.RenderText(g_screen , SCREEN_WIDTH/2 - 70 , SCREEN_HEIGHT/2 + 15);

               std :: string Score_  = "Score: " + std :: to_string(last_time_val);
               Score.SetText(Score_);
               Score.LoadFromRenderText(font , g_screen);
               Score.RenderText(g_screen , SCREEN_WIDTH/2 - 70 , SCREEN_HEIGHT/2 + 45);
               home_button.Show(g_screen);
               restart_button.Show(g_screen);


        }else {

            Map map_data = loadmap.getMap();
            if (game_state == PLAYING) {
                p_player.HandleBullet(g_screen);
                p_player.setMapXY(map_data.start_x, map_data.start_y);
                p_player.DoPlayer(map_data);

                if (boss_threat){
                    if (boss_threat->get_x_pos() < map_data.start_x - 80) {

                        boss_threat->set_x_pos(map_data.start_x -80);
                    }
                    boss_threat->SetMapXY(map_data.start_x, map_data.start_y);
                }
                boss_threat->ImpMoveType(g_screen);
                boss_threat->DoPlayer(map_data);

                last_time_val = game_timer.get_ticks() / 1000;

                if (p_player.get_check_time_skill()) {
                    if (skill_activation_time == 0) {
                        skill_activation_time = last_time_val;
                }
                if (last_time_val >= skill_activation_time + TIME_SKILL_2) {
                    p_player.set_check_time_skill(false);
                    skill_activation_time = 0;
                    }
                }
            }
            p_player.show(g_screen);

            loadmap.setMap(map_data);
            loadmap.DrawMap(g_screen);

            for (size_t i = 0; i < threats_list.size(); i++) {
                ThreatsObject* p_threat = threats_list.at(i);
                if (p_threat != NULL) {

                    if (p_threat->get_x_pos() < map_data.start_x - TILE_SIZE) {
                        if (p_threat->get_type_move() == ThreatsObject::FLYING_THREAT) {
                            max_x_pos_fly += 1000;
                            if (max_x_pos_fly < 20000 * TILE_SIZE) {
                                p_threat->set_x_pos(max_x_pos_fly);
                                p_threat->ResetBullet(g_screen);

                            } else {
                                p_threat->Free();
                                threats_list.erase(threats_list.begin() + i);
                                --i;
                                continue;
                            }
                        } else {
                            max_x_pos_dynamic += 1500;
                            if (max_x_pos_dynamic < 20000 * TILE_SIZE) {
                                p_threat->set_x_pos(max_x_pos_dynamic);
                                p_threat->ResetBullet(g_screen);
                            } else {
                                p_threat->Free();
                                threats_list.erase(threats_list.begin() + i);
                                --i;
                                continue;
                            }
                        }
                    }
                    p_threat->SetMapXY(map_data.start_x, map_data.start_y);
                    if (game_state == PLAYING) {
                        p_threat->ImpMoveType(g_screen);
                        p_threat->DoPlayer(map_data);
                        p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                    }
                    p_threat->show(g_screen);

                    if (game_state == PLAYING) {
                        SDL_Rect rect_player = p_player.GetRectFrame();
                        bool bCol1 = false;
                        std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
                        for (size_t jj = 0; jj < tBullet_list.size(); jj++) {
                            BulletObject* pt_bullet = tBullet_list.at(jj);
                            if (pt_bullet != NULL) {
                                bCol1 = SDLCommonFunc::CheckCollisionPlayer(rect_player, pt_bullet->GetRect());
                                if (bCol1) {
                                    p_threat->removeBullet(jj);
                                    break;
                                }
                            }
                        }

                        SDL_Rect rect_threat = p_threat->GetRectFrame();
                        SDL_Rect rect_boss = boss_threat->GetRectFrame();
                        bool bCol2 = SDLCommonFunc::CheckCollisionPlayer(rect_player, rect_threat);
                        bool bCol3 = SDLCommonFunc ::CheckCollisionPlayer(rect_player , rect_boss);
                        if (bCol1 || bCol2 || bCol3) {
                            sound_manager.PlaySoundEffect("game_over");
                            sound_manager.LoadMusic("audio/background_music.mp3");
                            if (music_state) sound_manager.PlayMusic(-1);
                            high_score = std :: max(last_time_val , high_score);
                            game_state = GAME_OVER;
                            game_timer.stop();
                        }
                    }
                }
            }
                boss_threat->show(g_screen);

            if (game_state == PLAYING) {
                std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
                for (size_t bl = 0; bl < bullet_arr.size(); bl++) {
                    BulletObject* p_bullet = bullet_arr.at(bl);
                    if (p_bullet != NULL) {
                        for (size_t t = 0; t < threats_list.size(); t++) {
                            ThreatsObject* obj_threat = threats_list.at(t);
                            if (obj_threat != NULL) {
                                SDL_Rect tRect;
                                tRect.x = obj_threat->GetRect().x;
                                tRect.y = obj_threat->GetRect().y;
                                tRect.w = obj_threat->get_width_frame();
                                tRect.h = obj_threat->get_height_frame();

                                SDL_Rect bRect = p_bullet->GetRect();
                                bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);
                                if (bCol) {
                                    p_player.removeBullet(bl);
                        if (obj_threat->get_type_move() == ThreatsObject::FLYING_THREAT){
                            max_x_pos_fly += 1000;
                            if (max_x_pos_fly < 20000 * TILE_SIZE) {
                                obj_threat->set_x_pos(max_x_pos_fly);
                                obj_threat->ResetBullet(g_screen);
                                } else {
                                    obj_threat->Free();
                                    threats_list.erase(threats_list.begin() + t);
                                    --t;
                                    break;
                                }
                            } else {
                                max_x_pos_dynamic += 1500;
                                if (max_x_pos_dynamic < 20000 * TILE_SIZE) {
                                    obj_threat->set_x_pos(max_x_pos_dynamic);
                                    obj_threat->ResetBullet(g_screen);

                                } else {
                                    obj_threat->Free();
                                    threats_list.erase(threats_list.begin() + t);
                                    --t;
                                    break;
                                }
                            }
                            break;
                                }
                            }
                        }
                    }
                }

                pause_button.Show(g_screen);
            }

            std::string str_score = "score: " + std::to_string(last_time_val);
            time_game.SetText(str_score);
            time_game.LoadFromRenderText(font, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 140, 15);

            if (game_state == PAUSED) {
                TextObject pause_text;
                pause_text.SetText("Paused");
                pause_text.SetColor(TextObject::WHITE_TEXT);
                pause_text.LoadFromRenderText(font, g_screen);
                pause_text.RenderText(g_screen, (SCREEN_WIDTH - pause_text.GetWidth()) / 2,
                                     (SCREEN_HEIGHT - pause_text.GetHeight()) / 2);
                continue_button.Show(g_screen);
                restart_button.Show(g_screen);
                home_button.Show(g_screen);
                setting_button.Show(g_screen);
            }
        }

        SDL_RenderPresent(g_screen);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND;
        if (real_imp_time < time_one_frame) {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time >= 0)
                SDL_Delay(delay_time);
        }
    }

    for (size_t i = 0; i < threats_list.size(); i++) {
        ThreatsObject* p_threat = threats_list.at(i);
        if (p_threat) {
            p_threat->Free();
            p_threat = NULL;
        }
    }
    threats_list.clear();
    if (boss_threat) {
        boss_threat->Free();
        delete boss_threat;
    }
    sound_manager.Free();
    close();
    return 0;
}
