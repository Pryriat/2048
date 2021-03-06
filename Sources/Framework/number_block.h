﻿#define Windows
/*
@autor zhangab
*/
#ifndef NUMBER_BLOCK_H_INCLUDED
#define NUMBER_BLOCK_H_INCLUDED

#include <bits/stdc++.h>

class number_block {
  public :
    number_block() {}
    // 以系统时间为种进行随机, difficulty :1 2 3对应只生成2, 24, 248
    unsigned int generate_number(int difficulty) {
        srand((unsigned)time(NULL));
        return 1 << (rand() % difficulty + 1);
    }
    // 初始化
    number_block(int difficulty, bool canBeMove = true, unsigned int x = 1, unsigned int y = 1) {
        is_moving = canBeMove;
        this -> x = x;
        this -> y = y;
        number = generate_number(difficulty);
    }
    // 修改相应的属性
    void modify_x(unsigned int x) {
        this -> x = x;
    }
    void modify_y(unsigned int y) {
        this -> y = y;
    }
    void modify_number(unsigned int number) {
        this -> number = number;
    }
    void modify_is_moving(bool is_moving) {
        this -> is_moving = is_moving;
    }
    // 获取相应属性
    unsigned int get_x() {
        return x;
    }
    unsigned int get_y() {
        return y;
    }
    unsigned int get_number() {
        return number;
    }
    unsigned int get_is_moving() {
        return is_moving;
    }
  private :
    // 为了维护数据的安全性,私有类成员只允许通过公有借口进行间接访问
    bool is_moving; // 移动
    unsigned int number; // 数字
    unsigned int x; // 二维横坐标
    unsigned int y; // 二维纵坐标
};

#endif // NUMBER_BLOCK_H_INCLUDED
