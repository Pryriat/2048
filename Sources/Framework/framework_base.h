#include"number_block.h"
class framework
{
public:
    framework(int difficulty) :difficulty(difficulty) {};
    bool end_judge(number_block* moving_block);//游戏结束条件判断
    void control(const unsigned char control_flag, number_block* moving_block);//方向键控制方法，参数为按键对应的ascii和指向正在下落块的指针
    void time_drop(number_block* moving_block);//随时间自动下落
    unsigned int return_mark();//返回得分
    number_block* generate_block(); // 生成数字块
    void merge(); // 合并
private:
    unsigned int difficulty;
    struct framework_block_item//框架内每个方块的元素类型
    {
        number_block* block;//数字类
        bool is_none;//空白区域标识符
        bool is_uncombined;//障碍块标识符
    };
    const static unsigned int row = 5;
    const static unsigned int column = 5;
    unsigned int mark = 0;
    framework_block_item game_blocks[row][column];
    number_block* moving_block;

    void merge_block();
    void place_new_block();
    bool is_crossed(int x, int y);
    bool is_blank(int x, int y);
    bool is_uncombined(int x, int y);
    bool is_same_number(int x, int y, int nx, int ny);
    void set_block_blank(int x, int y);
};
