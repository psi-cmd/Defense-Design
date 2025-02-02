#ifndef START_TOWER_H
#define START_TOWER_H

#define MTower_pic 5
#define CTower_pic 5
#define MTower_Man_pic 10
#define Tower_point 7

static SDL_Rect TowerPosition[Tower_point] = {580, 181, 90, 50,
                                       580, 240, 90, 50,
                                       379, 204, 90, 50,
                                       313, 359, 90, 50,
                                       401, 519, 90, 50,
                                       491, 499, 90, 50,
                                       512, 662, 90, 50};

static SDL_Rect choicepos[Tower_point] = {570, 134, 115, 147,
                                   570, 193, 115, 147,
                                   369, 157, 115, 147,
                                   303, 312, 115, 147,
                                   391, 472, 115, 147,
                                   481, 452, 115, 147,
                                   502, 615, 115, 147,};

enum TowerType {
    None,
    Arrow,
    Barracks,
    Magic,
    Cannon
};

std::string MtowerPicture[MTower_pic] = {
        R"(..\image\Mtower0.png)",
        R"(..\image\Mtower1.png)",
        R"(..\image\Mtower2.png)",
        R"(..\image\Mtower3.png)",
        R"(..\image\Mtower4.png)"
};

std::string CtowerPicture[CTower_pic] = {
        R"(..\image\Ctower0.png)",
        R"(..\image\Ctower1.png)",
        R"(..\image\Ctower2.png)",
        R"(..\image\Ctower3.png)",
        R"(..\image\Ctower1.png)"
};

std::string MtowerMan_pic[MTower_Man_pic] = {  //不要忘了逗号！
        R"(..\image\M1.png)",
        R"(..\image\M2.png)",
        R"(..\image\M3.png)",
        R"(..\image\M4.png)",
        R"(..\image\M5.png)",
        R"(..\image\M6.png)",
        R"(..\image\M7.png)",
        R"(..\image\M8.png)",
        R"(..\image\M9.png)",
        R"(..\image\M10.png)"
};


SDL_Texture *MtowerTexture[MTower_pic];
SDL_Texture *CtowerTexture[CTower_pic];

SDL_Texture *MtowerManTexture[MTower_Man_pic];

SDL_Texture *TowerStaticTexture[5];  //第一个位置是空地。

class Tower {
public:
    Tower(int n);

    void attack();

    void buildTower(TowerType);  //见她，响应其他函数。
    void render();  //显示
    void Update_Rect(SDL_Rect *r);  //更新矩形框大小。
    bool in_range(SDL_Rect target);

    void Mtower_Init();

    void Ctower_Init();

    TowerType type = None;
    Enemy **target = nullptr;

    int attackX;
    int attackY;  //射击中心
    bool build_menu=false;
    SDL_Point Center;
    SDL_Rect self_rect;    //对应坐标
    int tower_level = false;
private:
    int state = 0;
    float HtoW = 5.0 / 9;
    SDL_Texture *Texture_Now = TowerStaticTexture[0];
    SDL_Texture *Man_in_Tower = nullptr;
    SDL_Rect Man_rect;  //人右上角
    int range = 0;
    int CDstart = 0;
    int PicOrder = 0;
};

/*class Mtower : public Tower
{
private:
    int speed = 2;
    float HtoW = 7.0/8;
};*/
#endif //START_TOWER_H
