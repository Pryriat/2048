/*
Author:Chernobyl
Date:2018/10/19
Ver:Alpha
*/

#ifndef SUTIATION_JUDGEMENT_H_INCLUDED
#define SUTIATION_JUDGEMENT_H_INCLUDED

#include"framework_base.h"
#include<thread>

#ifdef Windows
#include<conio.h>
#include<windows.h>
#endif

#ifdef Linux
#include <unistd.h>
#include <termios.h>
char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
}
#endif

void framework::key_control()//键盘控制主函数
{
    int input = 0;//获取键盘输入
    while (1)
    {
#ifdef Windows
        if (_kbhit())//敲击键盘
        {
            if (_getch() == 224)//获取方向键特殊字符
                control(_getch());
        }
#endif

#ifdef Linux
        if (getch() == 224)//获取方向键特殊字符
            control(getch());
#endif
    }
    return;
}

bool framework::is_generate()//判断是否生成新方块
{
    if(this->moving_block->get_y() == 0)//移动方块触底
        setMovingBlock(generate_block());//生成新方块
    else
    {
        for (int x = 0; x < 5; x++)
            if (!this->game_blocks[x][6].is_none && !this->game_blocks[x][5].is_none)//如果某一列第五第六行同时不为空，游戏结束
                return false;
        setMovingBlock(generate_block());//否则生成新方块
    }
    printGameBoard();//生成后重新渲染
    return true;
}
bool framework::end_judge()//游戏结束判断
{
    while (this->lock_stream.test_and_set());//获取锁
    unsigned int x = this->moving_block->get_x();
    unsigned int y = this->moving_block->get_y();
    if (y == 0)
        is_generate();//移动方块触底，进入生成新方块函数
    else if (!this->game_blocks[x][y - 1].is_none)//方块落到其他方块上
    {
        this->merge();//调用合并函数
        if (!is_generate())//如果合并后游戏结束，释放锁并返回true
        {
            this->lock_stream.clear();
            return true;
        }
            
    }
    this->lock_stream.clear();//生成新方块成功，返回false
    return false;
}

void framework::control(unsigned char control_flag)
{
    while (this->lock_stream.test_and_set());//获取锁
    unsigned int current_x = this->moving_block->get_x();
    unsigned int current_y = this->moving_block->get_y();
    switch (control_flag)
    {
    case 75://方向左
    {
        if (current_x == 0)
        {//边界判断
            this->lock_stream.clear();//释放锁
            return;
        }

        if (!this->game_blocks[current_x-1][current_y].is_none || this->game_blocks[current_x-1][current_y].is_uncombined)
        {//左侧块非空或为障碍块
            this->lock_stream.clear();//释放锁
            return;
        }

        game_blocks[current_x][current_y].is_none = true;
        game_blocks[current_x-1][current_y].is_none = false;
        game_blocks[current_x-1][current_y].block = this->moving_block;
        this->moving_block->modify_x(current_x-1);
        printGameBoard();
    }
    break;

    case 77://方向右
    {
        if (current_x == this->COLUMN - 1)//右边界判断
        {
            this->lock_stream.clear();//释放锁
            return;
        }
        else if (!this->game_blocks[current_x+1][current_y].is_none || this->game_blocks[current_x+1][current_y].is_uncombined)
        {//右侧块不为空或为障碍块
            this->lock_stream.clear();//释放锁
            return;
        }
        this->game_blocks[current_x][current_y].is_none = true;
        this->game_blocks[current_x+1][current_y].is_none = false;
        this->game_blocks[current_x+1][current_y].block = this->moving_block;
        this->moving_block->modify_x(current_x+1);
        printGameBoard();
    }
    break;

    case 80://方向下
    {
        unsigned int tmp = 0;
        if (current_y == 0)//触底判断
        {
            this->lock_stream.clear();//释放锁
            return;
        }
        if (!this->game_blocks[current_x][current_y-1].is_none || this->game_blocks[current_x][current_y-1].is_uncombined)
        {//如果方块下为非空块或为障碍块
            this->lock_stream.clear();//释放锁
            return;
        }
        while (!this->game_blocks[current_x][tmp].is_none)
        {//从最方块所在列的最下一行开始遍历，找到为空的第一行
            tmp++;
        }
        this->game_blocks[current_x][current_y].is_none = true;
        this->moving_block->modify_y(tmp);
        this->game_blocks[current_x][tmp].block = this->moving_block;
        this->game_blocks[current_x][tmp].is_none = false;
        this->game_blocks[current_x][tmp].is_uncombined = false;
        printGameBoard();
        merge();//按下下键比触底或与其他方块邻接，调用合并函数
    }
    break;

    default:
        break;
    }
    this->lock_stream.clear();//释放锁
    return;
}

void framework::time_drop()//随时间下落函数，单独线程执行
{
    while (1)
    {
        while (this->lock_stream.test_and_set());//获取锁
        unsigned int current_x = this->moving_block->get_x();
        unsigned int current_y = this->moving_block->get_y();
        if (current_y == 0)//如果触底，返回继续循环
        {
            this->lock_stream.clear();//释放锁
            continue;
        }
        if (!this->moving_block->get_is_moving())
        {//移动块因某种原因处于未移动状态，返回继续循环
            this->lock_stream.clear();//释放锁
            continue;
        }
        else if (!this->game_blocks[current_x][current_y - 1].is_none)
        {//方块下接触到其他方块，返回继续循环
            this->lock_stream.clear();//释放锁
            continue;
        }
        this->moving_block->modify_y(current_y - 1);
        this->game_blocks[current_x][current_y].is_none = true;
        this->game_blocks[current_x][current_y - 1].is_none = false;
        this->game_blocks[current_x][current_y - 1].is_uncombined = false;
        this->game_blocks[current_x][current_y - 1].block = this->moving_block;
        printGameBoard();
        this->lock_stream.clear();
#ifdef Windows
        Sleep(1000);//延时1s
#endif
#ifdef Linux
        system("sleep 1");
#endif

    }
}

unsigned int framework::return_mark()
{//返回得分
    return this->mark;
}

framework_block_item* framework::current_status()
{//返回数组指针
    return &(this->game_blocks[0][0]);
}

void framework::Start()
{//游戏流程函数
    for(int x = 0; x < this->COLUMN; x++)//初始化，将所有块的属性赋初始值
    {
         for(int y = 0; y < this->ROW; y++)
        {
            this->game_blocks[x][y].is_none = true;
            this->game_blocks[x][y].is_uncombined = false;
        }
    }
    setMovingBlock(generate_block());//生成第一个移动块
    printGameBoard();//输出界面
    std::thread mv(std::bind(&framework::key_control, this));//载入监听方向键的线程
    std::thread td(std::bind(&framework::time_drop, this));
    while (!end_judge());//循环条件为游戏未结束
    mv.detach();//游戏结束，退出线程
    td.detach();
#ifdef Windows
    system("pause");
#endif
#ifdef Linux
    getchar();
#endif
    return;
}
#endif
