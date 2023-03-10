// game_boxman.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <graphics.h>
#include <conio.h>
#include "database.h"
#include <vector>

#define BG_WIDTH 800
#define BG_HEIGHT 650
#define BLK_SIZE 50

#define STARTX 100
#define STARTY 100

#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_RIGHT 'd'
#define KEY_LEFT 'a'
#define KEY_QUIT 'q'

#define ROWS 9
#define COLUMNS 12

using namespace std;
//int ROWS;
//int COLUMNS;
 
int map_vec[ROWS][COLUMNS] = {
{0,0,0,0,0,0,0,0,0,0,0,0},
{0,1,0,1,1,1,1,1,1,1,0,0},
{0,1,4,1,0,2,1,0,2,1,0,0},
{0,1,0,1,0,1,0,0,1,1,1,0},
{0,1,0,2,0,1,1,4,1,1,1,0},
{0,1,1,1,0,3,1,1,1,4,1,0},
{0,1,2,1,1,4,1,1,1,1,1,0},
{0,1,0,0,1,0,1,1,0,0,1,0},
{0,0,0,0,0,0,0,0,0,0,0,0}
}; 

//vector<vector<int>> map_vec;


enum blocks {
   WALL ,FLOOR,DESTINATION,MAN,BOX,HIT,ALL
};

enum directions {
    UP,DOWN,RIGHT,LEFT
};

//images
IMAGE images[ALL];

//man position
struct position
{
    int row;
    int col;

};

position manpos;

//all destinations
vector<pair<int, int>> dess;

void ChangeMap(position* pos, blocks blk)
{
    map_vec[pos->row][pos->col] = blk;
    putimage(STARTX + pos->col * BLK_SIZE, STARTY + pos->row * BLK_SIZE, &images[blk]);
}

//game control
void GameplayMove(directions dir) {

    position nextpos1 = manpos, nextpos2 = manpos;

    switch (dir)
    {
    case UP:
        nextpos1.row = manpos.row - 1;
        nextpos2.row = manpos.row - 2;
        break;
    case DOWN:
        nextpos1.row = manpos.row + 1;
        nextpos2.row = manpos.row + 2;
        break;
    case RIGHT:
        nextpos1.col = manpos.col + 1;
        nextpos2.col = manpos.col + 2;
        break;
    case LEFT:
        nextpos1.col = manpos.col - 1;
        nextpos2.col = manpos.col - 2;
        break;
    default:
        break;
    }

    //move to nextpos
    if (map_vec[nextpos1.row][nextpos1.col] == FLOOR)
    {
        ChangeMap(&nextpos1, MAN);
        ChangeMap(&manpos, FLOOR);
        manpos = nextpos1;
    }
    else if (map_vec[nextpos1.row][nextpos1.col] == BOX)
    {
        if (map_vec[nextpos2.row][nextpos2.col] == FLOOR)
        {
            ChangeMap(&nextpos2, BOX);
            ChangeMap(&nextpos1, MAN);
            ChangeMap(&manpos, FLOOR);
            manpos = nextpos1;
        }
        else if (map_vec[nextpos2.row][nextpos2.col] == DESTINATION)
        {
            ChangeMap(&nextpos2, BOX);
            ChangeMap(&nextpos1, MAN);
            ChangeMap(&manpos, FLOOR);
            manpos = nextpos1;
        }
    }
    else if (map_vec[nextpos1.row][nextpos1.col] == DESTINATION)
    {
        ChangeMap(&nextpos1, MAN);
        ChangeMap(&manpos, FLOOR);
    }



}

bool gamecheck()
{
    bool check = false;
    position pos;

    for (int i = 0; i < dess.size(); i++)
    {
        pos.row = dess[i].first;
        pos.col = dess[i].second;
        if (map_vec[dess[i].first][dess[i].second] == FLOOR)
        {
            ChangeMap(&pos, DESTINATION);
        }
        if (map_vec[dess[i].first][dess[i].second] == DESTINATION)
        {
            check = true;
        }
    }
    return check;
}

void success(IMAGE* bg)
{
    cout << "congrats!" << endl;
}

bool login(userinfo &user)
{
    string tmppwd;
    cout << "username:";
    cin >> user.name;
    cout << "password:";
    cin >> tmppwd;

    fetch_user_info(user);
    if (user.pwd == tmppwd)
    {
        return true;
    }
    return false;
}


int main()
{
    //login check
    userinfo user;
    mapinfo map;
    
    while (!login(user))
    {

    }

    //get map
    if (fetch_map_info(map, user.level) == false)
    {
        cout << "get map info error" << endl;
        system("pause");
        return 0;
    }

    //init map
    //ROWS = map.row;
    //COLUMNS = map.col;

    //int count = 0;
    //for (int i = 0; i < ROWS; i++)
    //{
    //    vector<int> tmp;
    //    for (int j = 0; j < COLUMNS; j++)
    //    {
    //        tmp.push_back(map.data[i * COLUMNS + j]);
    //        cout << map.data[i * COLUMNS + j];
    //    }
    //    map_vec.push_back(tmp);
    //}
    


    //size
    initgraph(800, 650);
    
    //background
    IMAGE img_background;
    loadimage(&img_background, _T("./resource/blackground.bmp"), BG_WIDTH, BG_HEIGHT, true);
    putimage(0, 0, &img_background);

    //load images
    loadimage(&images[WALL], _T("./resource/wall_right.bmp"), BLK_SIZE, BLK_SIZE, true);
    loadimage(&images[FLOOR], _T("./resource/floor.bmp"), BLK_SIZE, BLK_SIZE, true);
    loadimage(&images[DESTINATION], _T("./resource/des.bmp"), BLK_SIZE, BLK_SIZE, true);
    loadimage(&images[MAN], _T("./resource/man.bmp"), BLK_SIZE, BLK_SIZE, true);
    loadimage(&images[BOX], _T("./resource/box.bmp"), BLK_SIZE, BLK_SIZE, true);
    loadimage(&images[HIT], _T("./resource/box.bmp"), BLK_SIZE, BLK_SIZE, true);



    //display blocks get manpos
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            putimage(STARTX + BLK_SIZE * j, STARTY + BLK_SIZE * i, &images[map_vec[i][j]]);
            if (map_vec[i][j] == 3)
            {
                manpos.col = j;
                manpos.row = i;
            }
            if (map_vec[i][j] == DESTINATION)
            {
                dess.push_back(make_pair(i, j));
            }
        }
    }

    //play
    bool playstatus = true;

    do {
        if (_kbhit())
        {
            char key = _getch();
            switch (key)
            {
            case KEY_UP:
                GameplayMove(UP);
                break;
            case KEY_DOWN:
                GameplayMove(DOWN);
                break;
            case KEY_RIGHT:
                GameplayMove(RIGHT);
                break;
            case KEY_LEFT:
                GameplayMove(LEFT);
                break;
            case KEY_QUIT:
                playstatus = false;
                break;
            default:
                break;
            }

            if (gamecheck() == false)
            {
                success(&img_background);
                return 0;
            }
        }
        Sleep(100);
    } while (playstatus == true);

    //system("pause");
    return   0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
