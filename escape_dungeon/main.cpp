#include "CommonFunc.h"
#include "GameMap.h"
#include "Time.h"
#include "PlayerObject.h"
#include "ThreatsObject.h"
BaseObject loadBackground;



bool InitData() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        logErrorAndExit("SDL_image could not initialize", IMG_GetError());
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("Escape_Dungeon",
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
    return true;
}

bool loadbackground() {
    bool des = loadBackground.LoadImg("img//background.jpg", g_screen);
    if (!des) return false;
    return true;
}

void close() {
    loadBackground.Free();
    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);
    g_window = nullptr;
    g_screen = nullptr;
    IMG_Quit();
    SDL_Quit();
}

std :: vector<ThreatsObject*> MakeThreatdList()
{
    std :: vector<ThreatsObject*> list_threats;
    ThreatsObject* dynamic_threats = new ThreatsObject[20];
    for(int i = 0; i < 20 ;i++)
     {
         ThreatsObject* p_threat = (dynamic_threats + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("img//test1.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i * 500);

            // xac dinh vi tri mat dat
            int tile_y = (SCREEN_HEIGHT / TILE_SIZE) - 2;
            int ground_y = tile_y * TILE_SIZE;
            p_threat->set_y_pos(ground_y - p_threat->get_height_frame());

            // dat vung di chuyen
            int pos1 = p_threat->get_x_pos() - 100.0;
            int pos2 = p_threat->get_x_pos() + 100.0;
            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);


            list_threats.push_back(p_threat);

         }
     }

    return list_threats;

}

int main(int argc, char* argv[]) {
    Time fps_timer;
    if (!InitData()) return -1;
    if (!loadbackground()) return -1;

    GameMap loadmap;
    loadmap.LoadMap("map//map01.dat");
    loadmap.LoadTiles(g_screen);
    PlayerObject p_player;
    p_player.LoadImg("img//player_right.png" , g_screen);
    p_player.set_clips();
     std:: vector<ThreatsObject*> threats_list = MakeThreatdList();


    bool quit = false;
    while (!quit) {
            fps_timer.start();
        while (SDL_PollEvent(&g_event)) {
            if (g_event.type == SDL_QUIT) quit = true;
            if (g_event.type == SDL_KEYDOWN) {
            if (g_event.key.keysym.sym == SDLK_u) {
                quit = true;
            }
        }

            p_player.HandelInputAction(g_event , g_screen);
        }
        SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
        SDL_RenderClear(g_screen);
        loadBackground.Render(g_screen);

        Map map_data = loadmap.getMap();
        p_player.HandleBullet(g_screen);
        p_player.setMapXY(map_data.start_x , map_data.start_y);
        p_player.DoPlayer(map_data);
        p_player.show(g_screen);

        loadmap.setMap(map_data);
        loadmap.DrawMap(g_screen);

        for(int i = 0;i < threats_list.size() ; i++)
        {
            ThreatsObject * p_threat = threats_list.at(i);
            if(p_threat != NULL)
            {
                p_threat->SetMapXY(map_data.start_x , map_data.start_y);
                p_threat->ImpMoveType(g_screen);
                p_threat->DoPlayer(map_data);
                p_threat->show(g_screen);
            }

        }

        SDL_RenderPresent(g_screen);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND;

        if(real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >= 0)
            SDL_Delay(delay_time);
        }
    }
    close();
    return 0;
}
