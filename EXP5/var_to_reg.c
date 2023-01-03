#include <stdio.h>
#include <stdlib.h>
#include "exp.c"

// 溢出和恢复，以及函数的相关处理需要增加指令
// 由于这里分块是定值，所以我们先用一些表记录下来这些增加的内容
// 不然的话，动态分块实在太麻烦

typedef struct basic_block
{
    int pros[30]; // 前驱
    int pros_cnt; // 前驱个数
    int subs[2];  // 后继(后继理论上的最大个数就俩)
    int start;    // 语句块开始的位置
    int end;      // 语句块结束的位置
    int in[30];   // 计算活性分析的集合
    int out[30];  // 计算活性分析的集合
    int def[30];  // 计算活性分析的集合
    int use[30];  // 计算活性分析的集合
    int in_cnt;   // 对相关集合的计数
    int out_cnt;  // 对相关集合的计数
    int def_cnt;  // 对相关集合的计数
    int use_cnt;  // 对相关集合的计数
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
        block_lst->def_cnt = 0;
        block_lst->use_cnt = 0;
        block_lst->in_cnt = 0;
        block_lst->out_cnt = 0;
    }
}

// 排序用到的函数
// int cmpfunc(const void *a, const void *b)
// {
//     return (*(int *)a - *(int *)b);
// }

// 划分基本快
int block_divide(IR_list *ir, basic_block *block_lst, int lst_len)
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
    operation *ptr = ir->head;
    // 找到入口，按照入口分割
    for (int i = 0; i < ir->length; i++)
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
}

//变量记录器：变量名和index对应，然后

int live_var_analyser(IR_list *ir, basic_block *block_lst)
{
    //初始化def和use

}
