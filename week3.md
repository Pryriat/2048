# 需求改进&原型设计 
## 一.需求完善 
### （一）系统功能（补充）

#### 图形界面

根据需求与组内讨论结果，现归纳图形界面方面需要的设计与相应功能：

##### 数据-图形界面中间类：

1. 数字方块类 NumBlock

- 方块数字 num，若为0则表示为空方块，在具体的图形界面中不显示

- 方块数字查看方法 getBlockNum()

- 方块数字设置方法 setBlockNum()

- 该方块在棋盘数组中的下标 location，初步定为存储行号、列号的结构体

- 该方块相对于程序窗口的位置 position ，可能用QPointF实现

- 方块类声明

```cpp
class number_block {
  public :
    number_block() {}
    // 以系统时间为种进行随机, difficulty :1 2 3对应只生成2, 24, 248
    unsigned int generate_number(int difficulty) {
        srand((unsigned)time(NULL));
        return 1 << (rand() % difficulty + 1);
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
```

2. 5*7棋盘 Board:

- 二维数组 board\[ \]\[ \] ，以存储哪些方块为空方块、数字方块、下落方块，在具体的代码实现中可能以一级指针的形式声明于类中。

- 获取二维数组信息的方法 getBoard()

- 填入二维数组信息的方法 setBoard()

- 游戏结束条件判断 bool end_judge(number_block* moving_block)

- 方向键控制方法，参数为按键对应的ascii和指向正在下落块的指针 void control(const unsigned char control_flag, number_block* moving_block)

- 随时间自动下落 void time_drop(number_block* moving_block)

3. 游戏图形引擎 2048TetrisEnigine

- 移动：

  - 左移 moveLeft()
  - 右移 moveRight()
  - 下移 moveDown()

- “下一个方块”的数据获取 GetNextBlock()

- 二维数组棋盘的图形映射方法 BoardDisplay()

- 游戏计时 time

- 积分记录 score

- 积分上传方法 scoreUpload() ，将玩家ID与分数一起上传至服务器

- 获取积分方法 unsigned int return_mark()

##### 界面主窗口与排名界面

窗口分割控件：初定为QSplitter

文字显示： QLabel

按钮控件：QPushButton或QGraphicsItem

##### 游戏界面：

需重写的函数：

- 按键按一次 keyRelease()
- 按键长按 keyPress()
- 图形绘制 paint()

需要的控件：

- 按钮控件：QPushButton或QGraphicsItem
- 方块控件：初定为QPixmap或QTransfrom，加载数值方块对应的png，像素尺寸初定为20*20，并根据棋盘类Board的数据定时进行位置更新
- 退出游戏时的对话框控件：QMessageBox



#### 分数统计
#### 障碍块
#### 难度调整
#### 在线排名
### （二）四象限分析法

#### 作用：帮助你清晰定位软件的功能特点和将来要进行研发的创新点，如图所示：

#### 说明：


- 杀手功能：针对某一应用需求我们的竞争对手和用户已经决定了的必要满足的需求，我们的创新功能也在这一部分。



- 外围功能：辅助性的功能，比如良好的用户体验界面等。



- 必要需求：能够准确解决用户主要功能的需求，比如游戏中的控制计分等功能。



- 辅助需求：非必要需求，比如更改界面风格，增加背景音乐等功能。

-------------

#### 解决方法：



- 维持----以最低成本维持此功能



- 抵消----快速地达到“足够好”、“和竞争对手差不多”



- 优化----花大力气做到并保持行业最好



- 差异化----产生同类产品比不了的功能或优势



- 不做----砍掉一个功能，有时我们不需要做所有的功能

### （三）进度计划（WBS,Work Breakdown Structure）


## 二.原型改进

## 三.系统设计
### （一）架构设计
### （二）数据库设计

## 四.Alpha任务分配计划
召开迭代计划会议，为下周进入Sprint作准备。具体内容如下：
1.	依据项目组能提供的总时间、功能模块的优先级以及模块之间的依赖关系，在Product Backlog中选取待实现的功能项。(5分）
2.	对已选择的功能项再做进一步分解，分解为1-10小时左右的任务，构成Sprint Backlog。在PM的协助下，编码的同学对任务进行认领。（5分）
3.	以甘特图的方式拟定迭代冲刺计划。（10分）
（示例覆盖了全周期，本次迭代计划只关注alpha阶段即可）


## 五.测试计划
*说明：测试将在游戏基本实现后再进行*
