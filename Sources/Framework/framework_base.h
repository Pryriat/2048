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
    bool is_end = false;//游戏结束状态
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
    void End();//游戏结束后的资源释放函数

	//Han
    number_block* next_block;   //下一个生成方块
    void setMovingBlock(number_block * pNumBlock);  //修改类内成员变量moving_block
    void setMarkZero();     //用于重置游戏，将分数置0
    void genNextBlock();    //更新下一个方块

private:
    unsigned int difficulty;
    const static unsigned int ROW = 7;
    const static unsigned int COLUMN = 5;
    unsigned int mark = 0;
    framework_block_item game_blocks[COLUMN][ROW];

    bool merge_block();  // 进行“合并”块，这里加双引号的原因是有可能只有一个块进行自我“合并”
    void place_new_block();  // 放置“合并”后的块
    void updateMark();  // 更新分数
    bool is_crossed(int x, int y);  // 判断是否越界
    bool is_blank(int x, int y);  // 判断是否为空白块
    bool is_uncombined(int x, int y);  // 判断是否为障碍块
    bool is_same_number(int x, int y, int nx, int ny);  // 判断两个块是否具有相同数字
    void set_block_blank(int x, int y);  // 将某一位置设置为空白块
    void prinSeperationLine();  // 输出分隔线
    void addColumnNumber();  // 添加列号
};
