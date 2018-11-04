/* Copyright 2018 Yuzhao Hong
 */
#include "framework_base.h"

/* 生成数字块
 */
number_block* framework::generate_block() {
    return new number_block(difficulty, true, column/2, row);
}

/* 合并操作
 * 首先合并具有相同数字的数字块，把这些数字块所在的位置都置为空白区域
 * 然后放置一个新的数字块
 */
void framework::merge() {
    merge_block();
    place_new_block();
}

void framework::merge_block() {
    int x = moving_block->get_x();
    int y = moving_block->get_y();
    int new_number = moving_block->get_number();
    int dx[] = {-1, 0, 1}, dy[] = {0, -1, 0};
    // 枚举左下右三个格子
    for (int i = 0; i < 3; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        // 判断格子是否合法以及是否能和移动块合并
        if (!is_crossed(nx, ny) && !is_blank(nx, ny) &&
                !is_uncombined(nx, ny) && is_same_number(x, y, nx, ny)) {
            // 数字翻倍并合并
            new_number *= 2;
            set_block_blank(nx, ny);
        }
    }
    set_block_blank(x, y);
    moving_block->modify_number(new_number);
}

void framework::place_new_block() {
    int x = moving_block->get_x();
    int y = moving_block->get_y();
    // 判断移动块是否下落一个
    if (!is_crossed(x, y - 1) && is_blank(x, y - 1)) {
        moving_block->modify_y(y - 1);
        game_blocks[x][y-1].is_none = false;
        game_blocks[x][y-1].block = moving_block;
    } else {
        game_blocks[x][y].is_none = false;
        game_blocks[x][y].block = moving_block;
    }
}

bool framework::is_crossed(int x, int y) {
    return 0 <= x && x < column && 0 <= y && y < row;
}

bool framework::is_blank(int x, int y) {
    return game_blocks[x][y].is_none == true;
}

bool framework::is_uncombined(int x, int y) {
    return game_blocks[x][y].is_uncombined == true;
}

bool framework::is_same_number(int x, int y, int nx, int ny) {
    return game_blocks[x][y].block->get_number() ==
        game_blocks[nx][ny].block->get_number();
}

void framework::set_block_blank(int x, int y) {
    game_blocks[x][y].is_none = true;
}

void framework::setMovingBlock(number_block *pNumBlock) //Han  修改类内成员变量moving_block
{
    if(pNumBlock == nullptr)
    {
        return ;
    }
    
    moving_block = pNumBlock;
}

void framework::printGameBoard() {
    for(int y = row - 1; y >= 0; y--) {
        for(int x = 0; x < column; x++) {
            if (is_blank(x, y)) {
                printf("   ");
            } else if (is_uncombined(x, y)) {
                printf("   #");
            } else {
                printf("%4d", game_blocks[x][y].block->get_number());
            }
            printf(" ");
        }
        printf("\n");
    }
}
