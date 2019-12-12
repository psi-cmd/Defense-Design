
//游戏整体逻辑都在这里
extern void Print_Dec(int n, SDL_Rect *pos);

extern void wave(int m, int n);

extern void pWinorLose(SDL_Texture *pic);

bool In_Rect(int x, int y, SDL_Rect *rect);

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
    startbg = loadTexture(startbgfile);
    starticon = loadTexture(starticonfile);
    pause_pic = loadTexture(pausefile);
    re = loadTexture(refile);
    statef = loadTexture(statefile);
    defeat = loadTexture(defeatfile);
    victory = loadTexture(victoryfile);
    again = loadTexture(againfile);
    quit = loadTexture(quitfile);
    int i;
    for (i = 0; i < 10; i++) numf[i] = loadTexture(numffile[i]);

}

void refresh() {
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, map, nullptr, nullptr);  //渲染器加载地图
    SDL_RenderCopy(gRenderer, pause_pic, nullptr, &pausepos);  //暂停按钮
    SDL_RenderCopy(gRenderer, statef, nullptr, &statepos);  //血量栏
//    pWinorLose(defeat);   //输赢
    wave(2, 7);
    game->Enemy_Wave();
    game->Detect();
    game->Render();
    SDL_RenderPresent(gRenderer);
}

bool In_Rect(int x, int y, SDL_Rect *rect) {
    return x > rect->x && y > rect->y && x - rect->x < rect->w && y - rect->y < rect->h;
}

void Print_Dec(int n, SDL_Rect *pos) {
    int a, b, c, d;
    a = (n / 100) % 10;
    b = (n / 10) % 10;
    c = n % 10;
    d = n / 1000;
    if (n > 999) {
        SDL_RenderCopy(gRenderer, numf[d], nullptr, pos);
        SDL_RenderCopy(gRenderer, numf[a], nullptr, pos + 1);
        SDL_RenderCopy(gRenderer, numf[b], nullptr, pos + 2);
        SDL_RenderCopy(gRenderer, numf[c], nullptr, pos + 3);
    } else if (n > 99) {
        SDL_RenderCopy(gRenderer, numf[a], nullptr, pos);
        SDL_RenderCopy(gRenderer, numf[b], nullptr, pos + 1);
        SDL_RenderCopy(gRenderer, numf[c], nullptr, pos + 2);
    } else if (n > 9) {
        SDL_RenderCopy(gRenderer, numf[b], nullptr, pos);
        SDL_RenderCopy(gRenderer, numf[c], nullptr, pos + 1);
    } else {
        SDL_RenderCopy(gRenderer, numf[c], nullptr, pos);
    }
}

void wave(int m, int n)  //打印第几波
{
    SDL_RenderCopy(gRenderer, numf[m], nullptr, wavepos);
    SDL_RenderCopy(gRenderer, numf[n], nullptr, wavepos + 1);
}

void pWinorLose(SDL_Texture *pic)  //打印失败标志
{
    SDL_RenderCopy(gRenderer, pic, nullptr, resultpos);
    SDL_RenderCopy(gRenderer, again, nullptr, resultpos + 1);
    SDL_RenderCopy(gRenderer, quit, nullptr, resultpos + 2);
}