#include"number_block.h"
#include <atomic> //多线程原子实现
typedef struct framework_block_item//框架内每个方块的元素类型
{
    number_block* block = NULL;//数字类
    bool is_none = true;//空白区域标识符
    bool is_uncombined = false;//障碍块标识符
}framework_block_item;

class framework
{
public:
    number_block* moving_block;
    std::atomic_flag lock_stream = ATOMIC_FLAG_INIT;//多线程修改状态
    void Start();//初始化与游戏流程函数
    framework(int difficulty) :difficulty(difficulty) {};
    bool end_judge();//游戏结束条件判断
    void control(const unsigned char control_flag);//方向键控制方法，参数为按键对应的ascii和指向正在下落块的指针
    void key_control();//键盘控制方法，仅用于控制台
    bool is_generate();//是否生成方块
    void time_drop();//随时间自动下落
    unsigned int return_mark();//返回得分
    framework_block_item* current_status();//返回游戏方块数组指针

    number_block* generate_block(); // 生成数字块
    void merge(); // 合并
    void printGameBoard(); // 输出游戏局面

	//Han
    void setMovingBlock(number_block * pNumBlock);  //  修改类内成员变量moving_block


private:
    unsigned int difficulty;
    const static unsigned int ROW = 7;
    const static unsigned int COLUMN = 5;
    unsigned int mark = 0;
    framework_block_item game_blocks[COLUMN][ROW];

    bool merge_block();
    void place_new_block();
    bool is_crossed(int x, int y);  // 判断是否越界
    bool is_blank(int x, int y);  // 判断是否为空白块
    bool is_uncombined(int x, int y);  // 判断是否为障碍块
    bool is_same_number(int x, int y, int nx, int ny);  // 判断两个块是否具有相同数字
    void set_block_blank(int x, int y);  // 将某一位置设置为空白块
    void prinSeperationLine();  // 输出分隔线
    void addColumnNumber();  // 添加列号
};
