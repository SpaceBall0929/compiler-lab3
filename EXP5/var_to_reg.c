#include <stdio.h>
#include <stdlib.h>
#include "exp.c"
#define NUM_OF_REG 19

// 把这一大堆破函数汇总成一个针对单个连通分量的函数就好啦~
// 注意寄存器溢出，可以增设两条IR指令:
//  CLEAN $s0 #0(把指定寄存器的值存入数组[0]位置)
//  RECOVER $s0 #1(把存在数组[1]位置的值恢复到指定寄存器)
// 按照栈的办法管理

char *regs[NUM_OF_REG] = {"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8",
                  "t9", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8"};
// 对全局的变量进行记录，最多64个（因为位向量最长64位）
// 用这个数据结构开一个长64的列表
// 变量的index就是位向量中对应的位数
typedef struct var_info
{
    char *var_name;                // 变量名字
    char *reg_name;                // 分配寄存器的名字
    int var_last_use;               // 这个变量最后一次使用的IR的INDEX(INDEX最大值)
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
    int reg_flag;           // 记录该块是否已被分析完成（考虑到基本块存在回边的问题）
} basic_block;

// 初始化一个basic_block序列，来方便后面的操作
int init_block_lst(basic_block *block_lst, int lst_len)
{
    for (int i = 0; i < lst_len; i++)
    {
        block_lst[i].start = -1;
        block_lst[i].end = -1;
        block_lst[i].subs[0] = -1;
        block_lst[i].subs[1] = -1;
        block_lst[i].pros_cnt = 0;
        block_lst[i].in = 0;
        block_lst[i].out = 0;
        block_lst[i].def = 0;
        block_lst[i].use = 0;

    }
}

// 划分基本快
// 注意，由于多函数情况的存在，整个程序很可能出现多个联通分量...
// 必须一个一个连通分量的处理才可以，这个函数只支持处理单个连通分量
// 返回一共划分出几个基本快，十分重要捏
// start应当是FUNCTION标签后一句，end应当是整个函数最后一句
int block_divide(IR_list *ir, basic_block *block_lst, int start, int end)
{
    block_lst[0].start = start;
    block_lst[0].pros_cnt = 0;
    int block_cnt = 0;

    // 标签叫什么以及标签在哪里
    char *lable_name[30];
    int lable_locs[30];
    int lable_cnt = 0;
    // for (int i = 0; i < 30; i++)
    // {
    //     lable_locs[i] = -1;
    // }

    // 考虑到一个分割点可能会同时是标签语句之前和条件语句之后
    // 这里用一个flag来管理此事
    int already_cut = 1;

    // 遍历用临时变量
    operation *ptr = find_op(ir, start);
    // 找到入口，按照入口分割
    for (int i = start; i < end + 1; i++)
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
            block_lst[block_cnt + 1].pros_cnt += 1;
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

    //如果分块没有结束的话给收个尾
    if(block_lst[block_cnt].start < end + 1){
        block_lst[block_cnt].end = end;
        block_cnt += 1;
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
        else if (ptr->code == I_GOTO)
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
        target_lable = NULL;
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
        vars->all[i].var_last_use = 0;
        vars->all[i].bit_vector = (1 << i);
        vars->all[i].in_mem = -1;
    }
}

// 分析活跃流，并且给出变量信息记录
// 只用分析单个连通分量
int live_var_analyser(IR_list *ir, basic_block *block_lst, all_vars *vars, int start, int end)
{
}

int insert_clean()
{
}

int insert_recover(IR_list *ir, int insert_index, char* reg, int temp_save)
{
    operand_list* lst = init_operand_list();
    new_operand(lst, IMMEDIATE, 0, temp_save, 0);
    new_operand(lst, VARIABLE, reg, 0, 0);
    insert_op(ir, I_RECOVER, *lst, insert_index);
    del_operand_content(lst);
    return 0;
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
    for (int i = 0; i < NUM_OF_REG; i++)
    {
        if (!strcmp(name, regs[i]))
        {
            return 1 << i;
        }
    }
    printf("Unexpected ERROR: No such register(at find_reg_by_name).\n");
    return -1;
}
// 根据活跃变量分析结果分配寄存器
// 逐语句的看，一张表记录分配关系

//保存分析状态的数据结构：
typedef struct reg_alloc_info
{
    unsigned int reg_state;
    int LRU[NUM_OF_REG];
    int overflow_cnt;
    int next_block;
}reg_alloc_info;

typedef struct alloc_stack
{
    reg_alloc_info stack[64];
    int len;
}alloc_stack;



int init_alloc_info(alloc_stack* initor){
    initor->len = 1;
    initor->stack[0].next_block = 0;
    initor->stack[0].overflow_cnt = 0;
    initor->stack[0].reg_state = 0;
    for(int i = 0; i < NUM_OF_REG; i++){
        initor->stack[0].LRU[i] = -1;
    }

    return 0;
}

int copy_alloc_info(reg_alloc_info* src, reg_alloc_info* dest){
    dest->next_block = src->next_block;
    dest->overflow_cnt = src->overflow_cnt;
    dest->reg_state = src->reg_state;
    for(int i = 0; i < NUM_OF_REG; i++){
        dest->LRU[i] = dest->LRU[i];
    }
    return 0;
}

//对整个连通分量的分析必须考虑到存在分支和回边的问题
//需要使用栈管理
//在外面再套一层逐个函数分析的...
int all_block_reg_alloc(IR_list *ir, basic_block *block_lst, all_vars *vars){
    alloc_stack alloc_helper;
    reg_alloc_info info_now;
    init_alloc_info(&alloc_helper);
    int* subs;
    do{
        copy_alloc_info(&(alloc_helper.stack[--alloc_helper.len]), &info_now);
        subs = single_block_reg_alloc(ir, &block_lst[info_now.next_block], vars, &info_now);
        if(subs == NULL){
            continue;
        }
        if(subs[0] != -1){
            info_now.next_block = subs[0];
            copy_alloc_info(&info_now, &(alloc_helper.stack[alloc_helper.len++]));
        }
        if(subs[1] != -1){
            info_now.next_block = subs[1];
            copy_alloc_info(&info_now, &(alloc_helper.stack[alloc_helper.len++]));
        }
    }while (alloc_helper.len != 0);
    
    return 0;
}

//对单个块的分析
int* single_block_reg_alloc(IR_list *ir, basic_block *block, all_vars *vars, reg_alloc_info* reg_info)
{
    if(block->reg_flag){
        //已经分析过了，结束分析
        return NULL;
    }


    int start = block->start;
    int end = block->end;
    operation *op_ptr = find_op(ir, start);
    operand *rand_ptr = op_ptr->opers;
    var_info *info_now = NULL;
    
    //根据IN集合的情况进行占用解除
    



    //分析在本块中终结的变量的结束位点
    

    
    //逐个语句的看，一句句的分配内存
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

        // 变量是否已分配寄存器？
        unsigned int temp = reg_info->reg_state;
        if (info_now->reg_name != NULL)
        {
            unsigned int temp_reg = find_reg_by_name(info_now->reg_name);
            // 是，则从varinfo抄出寄存器名字，并且判断此处是否取消寄存器占用，更新寄存器占用情况
            rand_ptr->o_value.name = info_now->reg_name;
            // LRU更新
            for (int i = 0; i < NUM_OF_REG; i++)
            {
                if (temp & 1 == 1)
                {
                    reg_info->LRU[i] += 1;
                }
                temp = temp >> 1;
            }
            reg_info->LRU[temp_reg] = 0;
            // 变量不在OUT集合中并且为最后一次使用
            if ((block->out & info_now->bit_vector == 0) &&
                start == info_now->var_last_use)
            {
                reg_info->reg_state -= 1 << temp_reg;
                reg_info->LRU[temp_reg] = -1;
            }
        }
        else
        {
            //本次语句要用到的寄存器先保护起来，不要让LRU算法给他毁掉了
            
            //数值y已经被存储起来啦
            if(info_now->in_mem != -1){
                //先找一个寄存器...
                //避免一件事，下个语句要用的寄存器被用掉了
                
                insert_recover(ir, start, , info_now->in_mem);
                info_now->in_mem = -1;


            }
            
            
            // 否，则分配一个新的寄存器，在全部寄存器已满的情况下，计算一个最合适的寄存器进行溢出
            if (temp < 524287)
            {
                for (int i = 0; i < NUM_OF_REG; i++)
                {
                    if (temp & 1 == 1)
                    {
                        info_now->reg_name = regs[i];
                        rand_ptr->o_value.name = info_now->reg_name;
                        reg_info->reg_state += (1 << i);
                        break;
                    }
                    temp = temp >> 1;
                }
            }
            // 寄存器溢出在这里
            // 溢出采取类似页面置换的LRU算法
            else
            {
                int max_lru = 0;
                int max_index = 0;
                for(int i = 0; i < NUM_OF_REG; i++){
                    if(reg_info->LRU[i] > max_lru){
                        max_lru = reg_info->LRU[i];
                        max_index = i;
                    }
                }
                
                
                
                insert_clean();
            }
            
        }
    }

    block->reg_flag = 1;
    return &block->subs;
}
