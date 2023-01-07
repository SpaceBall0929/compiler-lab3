#include <stdio.h>
#include <stdlib.h>
#include "exp.c"

// 把这一大堆破函数汇总成一个针对单个连通分量的函数就好啦~
// 注意寄存器溢出，可以增设两条IR指令:
//  CLEAN $s0 #0(把指定寄存器的值存入数组[0]位置)
//  RECOVER $s0 #1(把存在数组[1]位置的值恢复到指定寄存器)
// 按照栈的办法管理

char *regs[19] = {"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8",
                  "t9", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8"};
// 对全局的变量进行记录，最多64个（因为位向量最长64位）
// 用这个数据结构开一个长64的列表
// 变量的index就是位向量中对应的位数
typedef struct var_info
{
    char *var_name;                // 变量名字
    char *reg_name;                // 分配寄存器的名字
    int var_use[64];               // 这个变量在那些语句用过，记录语句的index
    int var_use_cnt;               // 用了几次
    unsigned long long bit_vector; // 变量位向量
    int in_mem;                    // 溢出时在数组中的位置
} var_info;

// 一个全局使用的变量记录器
typedef struct all_vars
{
    var_info all[64];
    int cnt;
} all_vars;

// 基本块的内容
typedef struct basic_block
{
    int pros[30];           // 前驱
    int pros_cnt;           // 前驱个数
    int subs[2];            // 后继(后继理论上的最大个数就俩)
    int start;              // 语句块第一句话的index
    int end;                // 语句块最后一句的index
    //int reg_flag;
    unsigned long long in;  // 计算活性分析的集合，用64bit位向量记录
    unsigned long long out; // 计算活性分析的集合，用64bit位向量记录
    unsigned long long def; // 计算活性分析的集合，用64bit位向量记录
    unsigned long long use; // 计算活性分析的集合，用64bit位向量记录
} basic_block;

// 初始化一个basic_block序列，来方便后面的操作
int init_block_lst(basic_block *block_lst, int lst_len)
{
    for (int i = 0; i < lst_len; i++)
    {
        block_lst->start = -1;
        block_lst->end = -1;
        block_lst->subs[0] = -1;
        block_lst->subs[1] = -1;
        block_lst->pros_cnt = 0;
        block_lst->in = 0;
        block_lst->out = 0;
        block_lst->def = 0;
        block_lst->use = 0;
        //block_lst->reg_flag = 0;
    }
}

// 排序用到的函数
// int cmpfunc(const void *a, const void *b)
// {
//     return (*(int *)a - *(int *)b);
// }

// 划分基本快
// 注意，由于多函数情况的存在，整个程序很可能出现多个联通分量...
// 必须一个一个连通分量的处理才可以，这个函数只支持处理单个连通分量
// 返回一共划分出几个基本快，十分重要捏
// start应当是FUNCTION标签后一句，end应当是整个函数最后一句
int block_divide(IR_list *ir, basic_block *block_lst, int start, int end)
{
    block_lst[0].start = 0;
    int block_cnt = 0;

    // 标签叫什么以及标签在哪里
    char *lable_name[30];
    int lable_locs[30];
    int lable_cnt = 0;
    for (int i = 0; i < 30; i++)
    {
        lable_locs[i] = -1;
    }

    // 考虑到一个分割点可能会同时是标签语句之前和条件语句之后
    // 这里用一个flag来管理此事
    int already_cut = 1;

    // 遍历用临时变量
    operation *ptr = find_op(ir, start);
    // 找到入口，按照入口分割
    for (int i = 0; i < end - start + 1; i++)
    {
        switch (ptr->code)
        {
        case I_LABLE:
            if (!already_cut)
            {
                block_lst[block_cnt].subs[0] = block_cnt + 1;
                block_lst[block_cnt++].end = i - 1;
                block_lst[block_cnt].start = i;
                block_lst[block_cnt].pros[0] = block_cnt - 1;
                block_lst[block_cnt].pros_cnt += 1;
                already_cut = 0;
            }
            // 记录标签，方便后面的跳转前驱后继
            lable_locs[lable_cnt] = block_cnt;
            lable_name[lable_cnt++] = ptr->opers->o_value.name;
            break;
        case I_IF:
            block_lst[block_cnt].subs[0] = block_cnt + 1;
            block_lst[block_cnt + 1].pros[0] = block_cnt;
            block_lst[block_cnt].pros_cnt += 1;
        case I_GOTO:
            block_lst[block_cnt++].end = i;
            block_lst[block_cnt].start = i + 1;
            already_cut = 1;
            break;
        default:
            already_cut = 0;
            break;
        }
    }

    // 按照给定的标签信息，进行前驱后继的完善
    char *target_lable = NULL;
    for (int i = 0; i < block_cnt; i++)
    {
        ptr = find_op(ir, block_lst[i].end);
        if (ptr->code == I_IF)
        {
            target_lable = ptr->opers->next->next->o_value.name;
        }
        if (ptr->code == I_GOTO)
        {
            target_lable = ptr->opers->o_value.name;
        }
        if (target_lable != NULL)
        {
            for (int j = 0; j < lable_cnt; j++)
            {
                if (!strcmp(lable_name[j], target_lable))
                {
                    block_lst[i].subs[1] = lable_locs[j];
                    block_lst[j].pros[block_lst[j].pros_cnt++] = i;
                    break;
                }
            }
        }
    }

    return block_cnt;
}

int init_all_vars(all_vars *vars)
{
    vars->cnt = 0;
    for (int i = 0; i < 64; i++)
    {
        vars->all[i].reg_name = NULL;
        vars->all[i].var_name = NULL;
        vars->all[i].var_use_cnt = 0;
        vars->all[i].bit_vector = (1 << i);
        vars->all[i].in_mem = -1;
    }
}

// 分析活跃流，并且给出变量信息记录
// 只用分析单个连通分量

// 先定义一个函数，用来更新基本块的in和out的值
void update_block(basic_block *block, basic_block *block_lst)
{
    // 先计算出新的in值
    unsigned long long new_in = block->out;
    // 把所有前驱节点的out值与new_in求并
    for (int i = 0; i < block->pros_cnt; i++)
    {
        new_in |= block_lst[block->pros[i]].out;
    }
    // 如果new_in值发生了改变，则需要重新计算out值
    if (new_in != block->in)
    {
        block->in = new_in;
        block->out = (block->use | (block->out & ~block->def));
    }
}

int live_var_analyser(IR_list *ir, basic_block *block_lst, all_vars *vars, int lst_len, int start, int end)
{
    // 循环，直到所有基本块的in和out集合都不再发生改变
    int flag = 1;
    while (flag)
    {
        flag = 0; // 用来记录是否有基本块的in或out集合发生改变
        for (int i = 0; i < lst_len; i++)
        {
            int pre_in = block_lst[i].in;
            int pre_out = block_lst[i].out;
            update_block(&block_lst[i], block_lst);
            // 如果基本块的in或out集合发生了改变，则需要继续循环
            if (block_lst[i].in != pre_in || block_lst[i].out != pre_out)
            {
                flag = 1;
            }
        }
    }
}



int insert_clean()
{
}

int insert_recover()
{
}

var_info *find_var_by_name(char *name, all_vars *vars)
{
    for (int i = 0; i < vars->cnt; i++)
    {
        if (!strcmp(name, vars->all[i].var_name))
        {
            return &(vars->all[i]);
        }
    }

    printf("Unexpected ERROR: No such variable (at find_var_by_name).\n");
    return NULL;
}

unsigned int find_reg_by_name(char *name)
{
    for (int i = 0; i < 19; i++)
    {
        if (name == regs[i])
        {
            return i;
        }
    }
    printf("Unexpected ERROR: No such register(at find_reg_by_name).\n");
    return -1;
}
// 根据活跃变量分析结果分配寄存器
// 逐语句的看，一张表记录分配关系
int reg_alloc(IR_list *ir, basic_block *block_lst, int block_num, all_vars *vars, int start, int end)
{

    operation *op_ptr = find_op(ir, start);
    operand *rand_ptr = op_ptr->opers;
    // int rand_num = op_ptr->op_num;
    var_info *info_now = NULL;
    unsigned int reg_state = 0;
    int LRU[19] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int overflow_cnt = 0;
    while (start <= end)
    {
        if (rand_ptr->o_type != VARIABLE)
        {
            if (rand_ptr->next != NULL)
            {
                rand_ptr = rand_ptr->next;
            }
            else
            {
                op_ptr = op_ptr->next;
                rand_ptr = op_ptr->opers;
                start += 1;
            }
            continue;
        }

        // 找到当前指令中的变量
        info_now = find_var_by_name(rand_ptr->o_value.name, vars);

        //预先腾出一部分寄存器：
        //有def 不在out里的是否有考虑
        //有in

        // 变量是否已分配寄存器？
        // 增加对于活跃流的思考
        unsigned int temp = reg_state;
        if (info_now->reg_name != NULL)
        {
            unsigned int temp_reg = find_reg_by_name(info_now->reg_name);
            // 是，则从varinfo抄出寄存器名字，并且判断此处是否取消寄存器占用，更新寄存器占用情况
            rand_ptr->o_value.name = info_now->reg_name;
            // LRU更新
            for (int i = 0; i < 19; i++)
            {
                if (temp & 1 == 1)
                {
                    LRU[i] += 1;
                }
                temp = temp >> 1;
            }
            LRU[temp_reg] = 0;
            // 变量不在OUT集合中并且为最后一次使用
            if ((block_lst->out & info_now->bit_vector == 0) &&
                start == info_now->var_use[info_now->var_use_cnt - 1])
            {
                reg_state -= 1 << temp_reg;
                LRU[temp_reg] = -1;
            }
        }
        else
        {
            // 否，则分配一个新的寄存器，在全部寄存器已满的情况下，计算一个最合适的寄存器进行溢出
            if (temp < 524287)
            {
                for (int i = 0; i < 19; i++)
                {
                    if (temp & 1 == 1)
                    {
                        info_now->reg_name = regs[i];
                        rand_ptr->o_value.name = info_now->reg_name;
                        reg_state += (1 << i);
                        break;
                    }
                    temp = temp >> 1;
                }
            }
            // 寄存器溢出在这里
            // 溢出采取类似页面置换的LRU算法
            else
            {

            }
            
        }
    }
}
