/*
Author:Chernobyl
Date:2018/10/19
Ver:Alpha
*/

#ifndef SUTIATION_JUDGEMENT_H_INCLUDED
#define SUTIATION_JUDGEMENT_H_INCLUDED

#include"framework_base.h"


bool framework::end_judge(number_block* moving_block)
{
    unsigned int current_x = moving_block->get_x();
    unsigned int current_y = moving_block->get_y();

    //超出上界判断
    if ((this->game_blocks[current_x][current_y - 1].is_none == false) && current_y == column - 1)
    {
        return true;
    }

    //填满空格判断
    for (current_y = 0; current_y < row; ++current_y)
    {
        for (current_x = 0; current_x < column; ++current_x)
        {
            if (game_blocks[current_y][current_x].is_none)
            {
                return false;
            }
        }
    }
    if (current_y == row && current_x == column)
    {
        return true;
    }
    return false;
}

void framework::control(unsigned char control_flag, number_block* moving_block)
{
    unsigned int current_x = moving_block->get_x();
    unsigned int current_y = moving_block->get_y();
    switch (control_flag)
    {
    case 75://方向左
    {
        if (current_x == 0)//
        {//边界判断
            return;
        }

        if (!this->game_blocks[current_y][current_x - 1].is_none || this->game_blocks[current_y][current_x - 1].is_uncombined)
        {//左侧块为空块或为障碍块
            return;
        }

        game_blocks[current_y][current_x].is_none = true;
        game_blocks[current_y][current_x - 1].is_none = false;
        game_blocks[current_y][current_x - 1].block = moving_block;
        moving_block->modify_x(current_x - 1);
    }
    break;

    case 77://方向右
    {
        if (current_x == this->column - 1)
            return;
        else if (!this->game_blocks[current_y][current_x + 1].is_none || this->game_blocks[current_y][current_x + 1].is_uncombined)
        {//右侧块不为空或为障碍块
            return;
        }
        this->game_blocks[current_y][current_x].is_none = true;
        this->game_blocks[current_y][current_x + 1].is_none = false;
        this->game_blocks[current_y][current_x + 1].block = moving_block;
        moving_block->modify_x(current_x + 1);
    }
    break;

    case 80://方向下
    {
        int tmp = 0;
        if (current_y == 0)
        {
            return;
        }
        if (!this->game_blocks[current_y - 1][current_x].is_none || this->game_blocks[current_y - 1][current_x].is_uncombined)
        {
            return;
        }
        while (!this->game_blocks[tmp][current_x].is_none)
        {
            tmp++;
        }
        this->game_blocks[tmp][current_x].is_none = false;
        this->game_blocks[tmp][current_x].is_uncombined = false;
        this->game_blocks[tmp][current_x].block = moving_block;
        this->game_blocks[current_y][current_x].is_none = true;
        moving_block->modify_y(tmp);
    }
    break;

    }
}

void framework::time_drop(number_block* moving_block)
{
    unsigned int current_x = moving_block->get_x();
    unsigned int current_y = moving_block->get_y();
    if (!moving_block->get_is_moving())
    {
        return;
    }
    else if (!this->game_blocks[current_y - 1][current_x].is_none)
    {
        return;
    }
    moving_block->modify_y(current_y - 1);
    this->game_blocks[current_y][current_x].is_none = true;
    this->game_blocks[current_y - 1][current_x].is_none = false;
    this->game_blocks[current_y - 1][current_x].is_uncombined = false;
    this->game_blocks[current_y - 1][current_x].block = moving_block;
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
#endif
