/* Copyright 2018 Yuzhao Hong
 */
#include "framework_base.h"
#include "public.h"
/* 生成数字块
 */
number_block* framework::generate_block() {
    return new number_block(difficulty, true, COLUMN/2, ROW-1);   //Nov 8th: 将ROW改为 ROW-1 By Han

}

/* 合并操作
 * 首先合并具有相同数字的数字块，把这些数字块所在的位置都置为空白区域
 * 然后放置一个新的数字块
 */
void framework::merge() {
    bool flag = true;
    while(flag) {
        flag = merge_block();
        place_new_block();
    }
}

bool framework::merge_block() {
    bool flag = false;
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
            flag = true;
            new_number *= 2;
            set_block_blank(nx, ny);
        }
    }
    set_block_blank(x, y);
    moving_block->modify_number(new_number);
    return flag;
}

void framework::place_new_block() {
    int x = moving_block->get_x();
    int y = moving_block->get_y();
    // 判断移动块能否下落一个位置
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
    return x < 0 || x >= COLUMN || y < 0 || y >= ROW;
}

bool framework::is_blank(int x, int y) {
    return this->game_blocks[x][y].is_none == true;
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
    unsigned int x = pNumBlock->get_x();
    unsigned int y = pNumBlock->get_y();    //Nov.8th: 将此处的get_y()-1改为get_y() by.Han
    //pNumBlock->modify_y(6);               //Nov.8th: 将此处的y值修改代码注释掉
    this->game_blocks[x][y].is_none = false;
    this->game_blocks[x][y].is_uncombined = false;
    this->game_blocks[x][y].block = pNumBlock;
}

void framework::printGameBoard() {
#ifdef Windows
    system("cls");
#endif
#ifdef Linux
    system("clear");
#endif
    for(int y = ROW - 1; y >= 0; y--) {
        prinSeperationLine();
        printf("%d ", y);
        printf("|");
        for(int x = 0; x < COLUMN; x++) {
            if (is_blank(x, y)) {
                printf("    ");
            } else if (is_uncombined(x, y)) {
                printf("   #");
            } else {
                printf("%4d", game_blocks[x][y].block->get_number());
            }
            printf("|");
        }
        printf("\n");
    }
    prinSeperationLine();
    addColumnNumber();
}

void framework::prinSeperationLine() {
    for(int i = 0; i < COLUMN; i++) {
        for(int j = 0; j < 5; j++) {
            printf("-");
        }
    }
    printf("---\n");
}

void framework::addColumnNumber() {
    printf("  ");
    for(int i = 0; i < COLUMN; i++) {
        printf("    %d", i);
    }
    printf("\n");
}

void framework::setMarkZero()   // Han 用于重置游戏，将分数置0
{
    mark = 0;
}
