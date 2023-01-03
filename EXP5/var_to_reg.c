#include <stdio.h>
#include <stdlib.h>
#include "exp.c"

//把这一大堆破函数汇总成一个针对单个连通分量的函数就好啦~
//注意寄存器溢出，可以增设两条IR指令:
// CLEAN $s0(把指定寄存器的值存入内存)
// RECOVER $s0(把值恢复到指定寄存器)
//按照栈的办法管理

char* regs[25] = {"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", \
"t9", "s0", "s1", "s2", "s3","s4", "s5", "s6", "s7", "s8", "v0", "v1", \
"a0", "a1", "a2", "a3"};
// 对全局的变量进行记录，最多64个（因为位向量最长64位）
// 用这个数据结构开一个长64的列表
// 变量的index就是位向量中对应的位数
typedef struct var_info
{
    char* var_name; //变量名字
    char* reg_name; //分配寄存器的名字
    int var_use[64];//这个变量在那些语句用过，记录语句的index
    int var_use_cnt;//用了几次
}var_info;

//一个全局使用的变量记录器
typedef struct all_vars
{
    var_info all[64];
    int cnt;
}all_vars;


// 基本块的内容
typedef struct basic_block
{
    int pros[30]; // 前驱
    int pros_cnt; // 前驱个数
    int subs[2];  // 后继(后继理论上的最大个数就俩)
    int start;    // 语句块第一句话的index
    int end;      // 语句块最后一句的index
    unsigned long long in;   // 计算活性分析的集合，用64bit位向量记录
    unsigned long long out;  // 计算活性分析的集合，用64bit位向量记录
    unsigned long long def;  // 计算活性分析的集合，用64bit位向量记录
    unsigned long long use;  // 计算活性分析的集合，用64bit位向量记录
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
    char* target_lable = NULL;
    for (int i = 0; i < block_cnt; i++)
    {
        ptr = find_op(ir, block_lst[i].end);
        if(ptr->code == I_IF){
            target_lable = ptr->opers->next->next->o_value.name;
        }
        if (ptr->code == I_GOTO)
        {
            target_lable = ptr->opers->o_value.name;
        }
        if(target_lable != NULL){
             for (int j = 0; j < lable_cnt; j++){
                if(!strcmp(lable_name[j], target_lable)){
                    block_lst[i].subs[1] = lable_locs[j];
                    block_lst[j].pros[block_lst[j].pros_cnt++] = i;
                    break;
                }
            }
        }
    }

    return block_cnt;
}

int init_all_vars(all_vars* vars){
    vars->cnt = 0;
    for(int i = 0; i < 64; i++){
        vars->all[i].reg_name = NULL;
        vars->all[i].var_name = NULL;
        vars->all[i].var_use_cnt = 0;
    }
}

// 分析活跃流，并且给出变量信息记录
// 只用分析单个连通分量
int live_var_analyser(IR_list *ir, basic_block *block_lst, all_vars* vars, int start, int end)
{

}


int insert_clean(){

}

int insert_recover(){

}

var_info* find_var_by_name(char* name, all_vars* vars){
    for(int i = 0; i < vars->cnt; i++){
        if(!strcmp(name, vars->all[i].var_name)){
            return &(vars->all[i]);
        }
    }

    printf("Unexpected ERROR: No such variable (at find_var_by_name).\n");
    return NULL;
}
// 根据活跃变量分析结果分配寄存器
// 逐语句的看，一张表记录分配关系
int reg_alloc(IR_list *ir, basic_block *block_lst, int block_num, all_vars* vars, int start, int end){
    
    operation* op_ptr = find_op(ir, start);
    operand* rand_ptr = op_ptr->opers;
    int rand_num = op_ptr->op_num;
    while(start <= end){
        
    }
}
