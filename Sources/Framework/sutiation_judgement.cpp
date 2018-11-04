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
void framework::key_control()
{
    int input = 0;
    while (1)
    {
        if (_kbhit())
        {
            control(_getch());
        }
    }
    return;
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
    return;
}

void framework::time_drop()
{
    unsigned int current_x = this->moving_block->get_x();
    unsigned int current_y = this->moving_block->get_y();
    if (!this->moving_block->get_is_moving())
    {
        return;
    }
    else if (!this->game_blocks[current_x][current_y-1].is_none)
    {
        return;
    }
    this->moving_block->modify_y(current_y - 1);
    this->game_blocks[current_x][current_y].is_none = true;
    this->game_blocks[current_x][current_y-1].is_none = false;
    this->game_blocks[current_x][current_y-1].is_uncombined = false;
    this->game_blocks[current_x][current_y-1].block = this->moving_block;
    return;
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
            this->game_blocks[x][y].block = NULL;
            this->game_blocks[x][y].is_none = true;
            this->game_blocks[x][y].is_uncombined = false;
        }
    }
    generate_block();
    std::thread mv(key_control);
    std::thread td(time_drop);

    while (!end_judge())
    {
        mv.join();
        td.join();
    }
}
#endif
