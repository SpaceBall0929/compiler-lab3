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
    unsigned long long bit_vector; // 变量位向量
    int in_mem;                    // 溢出时在数组中的位置
    int end;                       // 存一下这个变量在块内哪里终结
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

    // 如果分块没有结束的话给收个尾
    if (block_lst[block_cnt].start < end + 1)
    {
        block_lst[block_cnt++].end = end;
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
        vars->all[i].end = -1;
        vars->all[i].bit_vector = (1 << i);
        vars->all[i].in_mem = -1;
    }
}

// 分析活跃流，并且给出变量信息记录
// 只用分析单个连通分量
// 先定义一个函数，用来更新基本块的in和out的值
void update_block(basic_block *block, basic_block *blocks)
{
    // 先计算出新的in值
    unsigned long long new_in = block->out;
    // 把所有前驱节点的out值与new_in求并
    for (int i = 0; i < block->pros_cnt; i++)
    {
        new_in |= blocks[block->pros[i]].out;
    }
    // 如果new_in值发生了改变，则需要重新计算out值
    if (new_in != block->in)
    {
        block->in = new_in;
        block->out = (block->use | (block->out & ~block->def));
    }
}
int live_var_analyser(IR_list *ir, basic_block *blocks, all_vars *vars, int start, int end, int lst_len)
{
    // 循环，直到所有基本块的in和out集合都不再发生改变
    int flag = 1;
    while (flag)
    {
        flag = 0; // 用来记录是否有基本块的in或out集合发生改变
        for (int i = 0; i < lst_len; i++)
        {
            int pre_in = blocks[i].in;
            int pre_out = blocks[i].out;
            update_block(&blocks[i], blocks);
            // 如果基本块的in或out集合发生了改变，则需要继续循环
            if (blocks[i].in != pre_in || blocks[i].out != pre_out)
            {
                flag = 1;
            }
        }
    }
}

int insert_clean(IR_list *ir, int insert_index, char *reg, int temp_save)
{
    operand_list *lst = init_operand_list();
    new_operand(lst, IMMEDIATE, 0, temp_save, 0);
    new_operand(lst, VARIABLE, reg, 0, 0);
    insert_op(ir, I_CLEAN, *lst, insert_index);
    del_operand_content(lst);
    return 0;
}

int insert_recover(IR_list *ir, int insert_index, char *reg, int temp_save)
{
    operand_list *lst = init_operand_list();
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

// 保存分析状态的数据结构：
typedef struct reg_alloc_info
{
    unsigned int reg_state;
    int LRU[NUM_OF_REG];
    char *var_in_reg[NUM_OF_REG];
    unsigned int overflow_bit_map;
    int next_block;
} reg_alloc_info;

typedef struct alloc_quene
{
    reg_alloc_info quene[64];
    int start;
    int end;
} alloc_quene;

int init_alloc_info(alloc_quene *initor)
{
    initor->start = 0;
    initor->end = 1;
    initor->quene[0].next_block = 0;
    initor->quene[0].overflow_bit_map = 0;
    initor->quene[0].reg_state = 0;
    for (int i = 0; i < NUM_OF_REG; i++)
    {
        initor->quene[0].LRU[i] = -1;
        initor->quene[0].var_in_reg[i] = NULL;
    }

    return 0;
}

int copy_alloc_info(reg_alloc_info *src, reg_alloc_info *dest)
{
    dest->next_block = src->next_block;
    dest->overflow_bit_map = src->overflow_bit_map;
    dest->reg_state = src->reg_state;
    for (int i = 0; i < NUM_OF_REG; i++)
    {
        dest->LRU[i] = src->LRU[i];
        dest->var_in_reg[i] = src->var_in_reg[i];
    }
    return 0;
}

// 分配寄存器的小函数
int find_reg_from_LRU(int *lru_lst)
{
    int max = -1;
    int index = -1;
    for (int i = 0; i < NUM_OF_REG; i++)
    {
        if (lru_lst[i] == -1)
        {
            lru_lst[i] = 0;
            return -1 * i - 1;
        }
        if (lru_lst[i] > max)
        {
            index = i;
            max = lru_lst[i];
        }
    }
    lru_lst[index] = 0;
    // var_in_reg[index] = NULL;
    return index + 1;
}

// 对整个连通分量的分析必须考虑到存在分支和回边的问题
// 需要使用队列管理，必须采用广度优先！
// 在外面再套一层逐个函数分析的...
int all_block_reg_alloc(IR_list *ir, basic_block *block_lst, all_vars *vars)
{
    alloc_quene alloc_helper;
    reg_alloc_info info_now;
    init_alloc_info(&alloc_helper);
    int *subs;
    do
    {
        copy_alloc_info(&(alloc_helper.quene[(alloc_helper.start) % 64]), &info_now);
        alloc_helper.start += 1;
        subs = single_block_reg_alloc(ir, &block_lst[info_now.next_block], vars, &info_now, block_lst);
        if (subs == NULL)
        {
            continue;
        }
        if (subs[0] != -1)
        {
            info_now.next_block = subs[0];
            copy_alloc_info(&info_now, &(alloc_helper.quene[(alloc_helper.end) % 64]));
            alloc_helper.end += 1;
        }
        if (subs[1] != -1)
        {
            info_now.next_block = subs[1];
            copy_alloc_info(&info_now, &(alloc_helper.quene[(alloc_helper.end) % 64]));
            alloc_helper.end += 1;
        }
    } while (alloc_helper.start != alloc_helper.end);

    return 0;
}

// 对单个块的分析
int *single_block_reg_alloc(IR_list *ir, basic_block *block, all_vars *vars, reg_alloc_info *reg_info, basic_block *block_lst)
{
    if (block->reg_flag)
    {
        // 已经分析过了，结束分析
        return NULL;
    }

    int start = block->start;
    int end = block->end;
    var_info *info_now = NULL;

    // 根据IN集合的情况进行占用解除
    // 选出需要解除占用的变量
    unsigned long long all_of_outs = 0;
    for (int i = 0; i < block->pros_cnt; i++)
    {
        all_of_outs = all_of_outs | block_lst[block->pros[i]].out;
    }
    all_of_outs = all_of_outs ^ block->in;
    // 解除占用
    for (int i = 0; i < 64; i++)
    {
        if (all_of_outs & 1)
        {
            reg_info->reg_state = reg_info->reg_state ^ find_reg_by_name(vars->all[i].reg_name);
            reg_info->var_in_reg[i] = NULL;
        }
        all_of_outs = all_of_outs >> 1;
    }

    // 根据IN∪DEF - OUT分析在本块中终结的变量的结束位点
    all_of_outs = (block->in | block->def) & (~block->out);
    var_info *outs_var_name[25];
    int out_cnt = 0;
    int temp_vec = all_of_outs;
    for (int i = 0; i < 64; i++)
    {
        if (temp_vec & 1)
        {
            outs_var_name[out_cnt] = &(vars->all[i]);
        }
        temp_vec = temp_vec >> 1;
    }
    // 为了做到这件事，倒着预览块内语句
    operation *op_ptr = find_op(ir, end);
    operand *to_del = init_operand(IMMEDIATE, NULL, 0, 0);
    operand *rand_ptr = to_del;
    rand_ptr->next = op_ptr->opers;
    int z = end;
    int preview_cnt = 0;
    while (preview_cnt < out_cnt)
    {
        if (rand_ptr->next == NULL)
        {
            op_ptr = op_ptr->front;
            rand_ptr = op_ptr->opers;
            z -= 1;
        }
        else
        {
            rand_ptr = rand_ptr->next;
        }

        if (rand_ptr->o_type != VARIABLE)
        {
            continue;
        }

        for (int i = 0; i < out_cnt; i++)
        {
            if (!strcmp(rand_ptr->o_value.name, outs_var_name[i]->var_name))
            {
                outs_var_name[i]->end = z;
                preview_cnt++;
                break;
            }
        }
    }

    // 逐个语句的看，一句句的分配寄存器
    op_ptr = find_op(ir, start);
    rand_ptr = to_del;
    rand_ptr->next = op_ptr->opers;
    int to_new_LRU = 1;
    operand* temp_ptr;
    while (start <= end)
    {
        if (rand_ptr->next == NULL)
        {
            op_ptr = op_ptr->next;
            rand_ptr = op_ptr->opers;
            start++;
            to_new_LRU = 1;
            for (int j = 0; j < NUM_OF_REG; j++)
            {
                if (reg_info->LRU[j] != -1)
                {
                    reg_info->LRU[j]++;
                }
            }
        }
        else
        {
            rand_ptr = rand_ptr->next;
        }

        if (rand_ptr->o_type != VARIABLE)
        {
            continue;
        }
        // if (rand_ptr->next != NULL)
        // {
        //     rand_ptr = rand_ptr->next;
        // }
        // else
        if(to_new_LRU)
        {
            
            // LRU更新在这里，必须把三地址码中已经写入寄存器的LRU信息改掉，不然会有冲突
            temp_ptr = rand_ptr;
            while (temp_ptr != NULL)
            {
                
                
                
                
                // if (rand_ptr->o_type == VARIABLE)
                // {
                //     for (int j = 0; j < NUM_OF_REG; j++)
                //     {
                //         if (!strcmp(reg_info->var_in_reg, rand_ptr->o_value.name))
                //         {
                //             reg_info->LRU[j] = 0;
                //         }
                //     }
                // }
                // rand_ptr = rand_ptr->next;
            }
            rand_ptr = op_ptr->opers;
            to_new_LRU = 0;
        }
 

        // 找到当前指令中的变量
        info_now = find_var_by_name(rand_ptr->o_value.name, vars);
        //找不到就建立新的
        
        // 变量是否已分配寄存器？
        unsigned int temp = reg_info->reg_state;
        if (info_now->reg_name != NULL)
        {
            unsigned int temp_reg = find_reg_by_name(info_now->reg_name);
            // 注意：这个寄存器中有很小的可能还有其他数据，这里必须有个严谨的处理
            if (strcmp(reg_info->var_in_reg[temp_reg], rand_ptr->o_value.name))
            {
            }

            // 是，则从varinfo抄出寄存器名字
            rand_ptr->o_value.name = info_now->reg_name;
            // 判断此处是否取消寄存器占用，更新寄存器占用情况
            // 变量不在OUT集合中并且为最后一次使用
            if ((all_of_outs & info_now->bit_vector == 0) &&
                start == info_now->end)
            {
                reg_info->reg_state = reg_info->reg_state ^ 1 << temp_reg;
                reg_info->LRU[temp_reg] = -1;
                reg_info->var_in_reg[temp_reg] = NULL;
            }
        }
        else
        {
            // 第一步 腾地方，没有空闲寄存器的话，需要用一下clean暂存
            int new_reg = find_reg_from_LRU(reg_info->LRU);
            if (new_reg > 0)
            {
                // 存储一个旧数值
                int temp_bit_map = reg_info->overflow_bit_map;
                int i = 0;
                for (; i < 32; i++)
                {
                    if (!temp_bit_map & 1)
                    {
                        insert_clean(ir, start, regs[new_reg], i);
                        var_info *info_temp = find_var_by_name(reg_info->var_in_reg[new_reg], vars);
                        info_temp->in_mem = i;
                        break;
                    }
                }
                if (i == 32)
                {
                    printf("ERROR: too much overflow!(IN single_block_reg_alloc)\n");
                }
            }
            else
            {
                new_reg *= -1;
            }
            new_reg--;

            // 第二步 占地方，如果是已经被暂存的就恢复，如果不是，就直接占上就行
            if (info_now->in_mem != -1)
            {
                insert_recover(ir, start, regs[new_reg], info_now->in_mem);
                info_now->in_mem = -1;
            }

            reg_info->reg_state = reg_info->reg_state ^ (1 << new_reg);
            reg_info->var_in_reg[new_reg] = info_now->var_name;
            info_now->reg_name = regs[new_reg];
            rand_ptr->o_value.name = regs[new_reg];
        }
        // 来一套前面的轮换处理流程
    }

    free(to_del);
    block->reg_flag = 1;
    return &block->subs;
}
