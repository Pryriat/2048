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
}

void framework::control(unsigned char control_flag, number_block* moving_block)
{
	unsigned int current_x = moving_block->get_x();
	unsigned int current_y = moving_block->get_y();
	switch (control_flag)
	{
		case 75://方向左
		{
			if (!this->game_blocks[current_y][current_x - 1].is_none)//左侧块不为空
			{
				return;
			}
			game_blocks[current_y][current_x].is_none = true;
			game_blocks[current_y][current_x - 1].is_none = false;
			game_blocks[current_y][current_x - 1].block = moving_block;
			moving_block->modify_x(current_x - 1);
		}
		break;
	}

}

void framework::time_drop()
{
	;
}

unsigned int framework::return_mark()
{
	return this->mark;
}
#endif
