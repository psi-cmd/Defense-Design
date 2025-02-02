/*
// * 进度：
// * 窗口就绪，塔位置就绪，正在进行图片刷新机制。
// * 刷新ok，开始放置
// */
//总启动文件，统筹所有.cpp文件，完成初始化，窗口载入和各类图片载入功能。即游戏底层。
#include <SDL_image.h>
#include <SDL.h>
#include <string>
#include "main.h"
#include "enemy.cpp"
#include "tower.cpp"
#include "game_logic.cpp"
#include "Game.cpp"
#include "bullet.cpp"
#include "shell.cpp"
//#include "picture_change.cpp"

using namespace std;

bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {  //初始化可视媒体模块
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        success = false;
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            cout << "Warning: Linear texture filtering not enabled!" << endl;
        }
        gWindow = SDL_CreateWindow("Defense-Design", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, SDL_WINDOW_SHOWN);  //创建窗体
        if (gWindow == nullptr) {
            cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);  //对screen创造刷新器
            if (gRenderer == nullptr) {
                cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
                    success = false;
                }
            }
        }
    }

    /*if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        success = false;
    }*/
    return success;
}

void close() {
    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    SDL_DestroyTexture(map);  //干掉图片
    map = nullptr;  //释放指针
    SDL_DestroyWindow(gWindow);  //干掉窗口
    gWindow = nullptr;  //释放指针
    IMG_Quit();
    SDL_Quit();  //退出
}

SDL_Texture *loadTexture(const std::string &path) {  //最终要演变为一个全局加载函数。
    SDL_Texture *newTexture = nullptr;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        cout << "Unable to load image %s! SDL_image Error: " << path.c_str() << IMG_GetError() << endl;
    } else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == nullptr) {
            cout << "Unable to create texture from %s! SDL Error: " << path.c_str() << SDL_GetError() << endl;
        }
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool loadTextures(std::string *picture_path, SDL_Texture **texture_array, int n) {
    for (int i = 0; i < n; ++i) {
        texture_array[i] = loadTexture(picture_path[i]);
        SDL_SetTextureBlendMode(texture_array[i], SDL_BLENDMODE_BLEND);
        if (texture_array[i] == nullptr) {
            printf("Failed to load texture image!\n");
            return false;
        }
    }
    return true;
}

void loadSingle() {
    map = loadTexture(mapfile);
    if (map == nullptr) {
        std::cout << "Unable to load image %s! SDL Error: " << "image\\map_demo.png" << SDL_GetError();
    }

    TowerStaticTexture[0] = loadTexture(towergroundfile);
    if (TowerStaticTexture[0] == nullptr) {
        std::cout << "Unable to load image %s! SDL Error: " << "image\\raw.png" << SDL_GetError();
    }

    MBullet_Texture = loadTexture(MBullet_Pic);
    if (MBullet_Texture == nullptr) {
        std::cout << "Unable to load image %s! SDL Error: " << "image\\bullet.png" << SDL_GetError();
    }

    CShell_Texture = loadTexture(CShell_Pic);
    if (CShell_Texture == nullptr) {
        std::cout << "Unable to load image %s! SDL Error: " << "image\\shell.png" << SDL_GetError();
    }

    startbg = loadTexture(startbgfile);
//    starticon = loadTexture(starticonfile);
    pause_pic = loadTexture(pausefile);
    re = loadTexture(refile);
    statef = loadTexture(statefile);
    defeat = loadTexture(defeatfile);
    victory = loadTexture(victoryfile);
    again = loadTexture(againfile);
    quit = loadTexture(quitfile);
    nextwave = loadTexture(nextwavefile);
    for (int i = 0; i < 10; i++) numf[i] = loadTexture(numffile[i]);
    for (int i = 0; i < 5; i++) smog[i] = loadTexture(smogfile[i]);
    for (int i = 0; i < 3; i++) choice_ring[i] = loadTexture(choicefile[i]);

}


bool loadArray() {
    loadSingle();
    int success = true;
    loadTextures(DevilPicture, DevilTexture, Enemy1_pic);
    loadTextures(MtowerPicture, MtowerTexture, MTower_pic);
    loadTextures(CtowerPicture, CtowerTexture, CTower_pic);
    loadTextures(MtowerMan_pic, MtowerManTexture, MTower_Man_pic);
    loadTextures(Smoke_Pic, Smoke_Texture, 10);
    TowerStaticTexture[1] = MtowerTexture[0];
    TowerStaticTexture[2] = CtowerTexture[0];
    return success;
}

int WinMain(int argc, char **argv) {
    srand(time(nullptr));
    if (!init()) {
        cout << "failed to initialize!" << endl;
    } else {
        if (!loadArray()) {
            cout << "Failed to load media!" << endl;
        }  //所有显示都给渲染器了。
    }
    game->Tower_init();
    start = _time = SDL_GetTicks();
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, startbg, nullptr, nullptr);  //渲染开始背景
    SDL_RenderCopy(gRenderer, starticon, nullptr, &startpos);  //渲染开始按钮
//again:
    while (!Quit)   //游戏进入界面循环
    {
        if (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {  //按窗口右上角的叉
                Quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                SDL_GetMouseState(&(Mouse_Point.x), &(Mouse_Point.y));
                if (SDL_PointInRect(&Mouse_Point, &difficultypos[0])){
                    game->Diff = Casual;
                    game->money = 425;
                    game->life = 10;
                    for (int i=0;i<WAVE;i++) game->Enemy_num[i] = 2*(i+1);
                    game->VEL = 3;
                } else if (SDL_PointInRect(&Mouse_Point, &difficultypos[1])){
                    game->Diff = Normal;
                    game->money = 310;
                    game->life = 5;
                    game->Interval *= 0.5;
                    for (int i=0;i<WAVE;i++) game->Enemy_num[i] = 5*(i+1);
                    game->VEL = 5;
                } else if (SDL_PointInRect(&Mouse_Point, &difficultypos[2])){
                    game->Diff = Veteran;
                    game->money = 225;
                    game->life = 1;
                    game->Interval *= 0.4;
                    for (int i=0;i<WAVE;i++) game->Enemy_num[i] = 7*(i+1);
                    game->VEL = 10;
                } else continue;
                break;
            }
        }
        SDL_RenderPresent(gRenderer);
    }
    while (!Quit) {  //游戏主循环
        while (SDL_PollEvent(&e) != 0 || pause) {  //事件池，没事就返回0
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {  //按窗口右上角的叉
                close();
                return 0;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN || e.key.keysym.sym == SDLK_p) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    SDL_GetMouseState(&(Mouse_Point.x), &(Mouse_Point.y));
                    if (SDL_PointInRect(&Mouse_Point, &pausepos)) {
                        _time = SDL_GetTicks() - _time;
                        pause = true;
                        SDL_RenderCopy(gRenderer, re, nullptr, &repos);
                        SDL_RenderPresent(gRenderer);
                    }
                    else if (SDL_PointInRect(&Mouse_Point, &nextwavepos)) {
                        game->money +=game->wave_timer/5;
                        game->wave_timer = 400+200*game->Wave;
                        game->Enemy_Count = game->Enemy_num[game->Wave++];
                    }
                    game->Tower_Build();
                }
                Mouse_Point.x = 0;
                Mouse_Point.y = 0;
            }
            while (!Quit && pause) {
                if (game->life <= 0) {
                    pWinorLose(defeat);
                    SDL_RenderPresent(gRenderer);
                }
                if (game->win){
                    pWinorLose(victory);
                    SDL_RenderPresent(gRenderer);
                }
                if (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                        close();
                        return 0;
                    }
                    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                        SDL_GetMouseState(&(Mouse_Point.x), &(Mouse_Point.y));
                        if ((game->life <= 0 && SDL_PointInRect(&Mouse_Point, &resultpos[1])) || (game->win && SDL_PointInRect(&Mouse_Point, &resultpos[1]))){
                            game->Restart();
//                            goto again;
                        }
                        if ((game->life <= 0 && SDL_PointInRect(&Mouse_Point, &resultpos[2])) || (game->win && SDL_PointInRect(&Mouse_Point, &resultpos[2]))){
                            close();
                            return 0;
                        }
                        if (game->life > 0 && SDL_PointInRect(&Mouse_Point, &repos)) {
                            _time = SDL_GetTicks() - _time;
                            pause = false;
                            break;
                        }
                    }
                    Mouse_Point.x = 0;
                    Mouse_Point.y = 0;
                }
            }
        }
        if (SDL_GetTicks() - _time >= 60) {
            _time = SDL_GetTicks();
            refresh();
        }
    }
    close();
    return 0;
}