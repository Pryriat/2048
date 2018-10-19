/*
@autor zhangab
*/
#ifndef NUMBER_BLOCK_H_INCLUDED
#define NUMBER_BLOCK_H_INCLUDED

#include <bits/stdc++.h>

class number_block {
  public :
    number_block() {}
    // 生成数字2,4,8,未实现根据难度调节数字,有待修正
    unsigned int generate_number(int difficulty) {
        return 1 << (rand() % 3 + 1);
    }
    // 初始化
    number_block(int difficulty, bool canBeMove = true, int x = 1, int y = 1) {
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
