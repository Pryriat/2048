/*
Author:Chernobyl
Date:2018/10/19
Ver:Alpha
*/

#ifndef SUTIATION_JUDGEMENT_H_INCLUDED
#define SUTIATION_JUDGEMENT_H_INCLUDED

#include"framework_base.h"
#include<thread>
#include<conio.h>
#include<windows.h>
void framework::key_control()
{
    int input = 0;
    while (1)
    {
        if (_kbhit())
        {
            while (this->lock_stream.test_and_set());
            control(_getch());
            this->lock_stream.clear();
        }
    }
    return;
}

bool framework::is_generate()
{
    unsigned int current_x = this->moving_block->get_x();
    unsigned int current_y = this->moving_block->get_y();
    if (this->game_blocks[current_x][current_y - 1].is_none)
        return false;
    return true;
}
bool framework::end_judge()
{
    unsigned int current_x = this->moving_block->get_x();
    unsigned int current_y = this->moving_block->get_y();

    //超出上界判断
    if ((this->game_blocks[current_x][current_y - 1].is_none == false) && current_y == this->row - 1)
    {
        return true;
    }

    //填满空格判断
    for (current_x = 0; current_x < this->column; ++current_x)
    {
        for (current_y = 0; current_y < this->row; ++current_y)
        {
            if (game_blocks[current_x][current_y].is_none)
            {
                return false;
            }
        }
    }
    if (current_x == this->column && current_y == this->row)
    {
        return true;
    }
    return false;
}

void framework::control(unsigned char control_flag)
{
    unsigned int current_x = this->moving_block->get_x();
    unsigned int current_y = this->moving_block->get_y();
    switch (control_flag)
    {
    case 75://方向左
    {
        if (current_x == 0)//
        {//边界判断
            return;
        }

        if (!this->game_blocks[current_x-1][current_y].is_none || this->game_blocks[current_x-1][current_y].is_uncombined)
        {//左侧块为空块或为障碍块
            return;
        }

        game_blocks[current_x][current_y].is_none = true;
        game_blocks[current_x-1][current_y].is_none = false;
        game_blocks[current_x-1][current_y].block = this->moving_block;
        this->moving_block->modify_x(current_x-1);
    }
    break;

    case 77://方向右
    {
        if (current_x == this->column-1)
            return;
        else if (!this->game_blocks[current_x+1][current_y].is_none || this->game_blocks[current_x+1][current_y].is_uncombined)
        {//右侧块不为空或为障碍块
            return;
        }
        this->game_blocks[current_x][current_y].is_none = true;
        this->game_blocks[current_x+1][current_y].is_none = false;
        this->game_blocks[current_x+1][current_y].block = this->moving_block;
        this->moving_block->modify_x(current_x+1);
    }
    break;

    case 80://方向下
    {
        int tmp = 0;
        if (current_y == 0)
        {
            return;
        }
        if (!this->game_blocks[current_x][current_y-1].is_none || this->game_blocks[current_x][current_y-1].is_uncombined)
        {
            return;
        }
        while (!this->game_blocks[current_x][tmp].is_none)
        {
            tmp++;
        }
        this->game_blocks[current_x][tmp].is_none = false;
        this->game_blocks[current_x][tmp].is_uncombined = false;
        this->game_blocks[current_x][tmp].block = this->moving_block;
        this->game_blocks[current_x][current_y].is_none = true;
        this->moving_block->modify_y(tmp);
    }
    break;

    default:
        break;
    }
    system("cls");
    printGameBoard();
    return;
}

void framework::time_drop()
{
    while (1)
    {
        while (this->lock_stream.test_and_set());
        unsigned int current_x = this->moving_block->get_x();
        unsigned int current_y = this->moving_block->get_y();
        if (!this->moving_block->get_is_moving())
        {
            continue;
        }
        else if (!this->game_blocks[current_x][current_y - 1].is_none)
        {
            continue;
        }
        this->moving_block->modify_y(current_y - 1);
        this->game_blocks[current_x][current_y].is_none = true;
        this->game_blocks[current_x][current_y - 1].is_none = false;
        this->game_blocks[current_x][current_y - 1].is_uncombined = false;
        this->game_blocks[current_x][current_y - 1].block = this->moving_block;
    system("cls");
    printGameBoard();
    this->lock_stream.clear();
    Sleep(5000);
        }
}

unsigned int framework::return_mark()
{
    return this->mark;
}

framework_block_item* framework::current_status()
{
    return &(this->game_blocks[0][0]);
}

void framework::Start()
{
    for(int x = 0; x < this->column; x++)//初始化
    {
         for(int y = 0; y < this->row; y++)
        {
            this->game_blocks[x][y].is_none = true;
            this->game_blocks[x][y].is_uncombined = false;
        }
    }
    setMovingBlock(generate_block());
    std::thread mv(std::bind(&framework::key_control, this));
    std::thread td(std::bind(&framework::time_drop, this));
    while (!end_judge())
    {
       if(this->is_generate())
           setMovingBlock(generate_block());
    }
    mv.detach();
    td.detach();
}
#endif
