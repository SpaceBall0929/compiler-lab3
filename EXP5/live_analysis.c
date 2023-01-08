#include "var_to_reg.c"
//****************************************************************
// 分析活跃流，并且给出变量信息记录
#define USE 2333
#define DEF 7788
//判断语句是否要变
int have_cal(operation *op)
{
    if(op->op_num == I_ARG || op->op_num == I_GOTO || op->op_num == I_CALL || op->op_num == I_PARAM)
    return 0;
    return op->op_num;
}

//判断某变量是定值还是引用
int check_group(int i, operation *op)
{
    if(i > 0) return USE;
    else return DEF;
}

//加入use集或def集
int use_or_def(var_info *v, int i, operation *op, basic_block b)
{
    // 如果是使用，则将变量加入 use 集合
    if (check_group(i, op) == USE)
    {
        b.use |= v->bit_vector;
    }
    // 如果语是定义，则将变量加入 def 集合
    if (check_group(i, op) == DEF) 
    {
        // 检查变量在 B 中是否已被引用过
        if ((b.use & v->bit_vector) == 0) {
            // 如果没有被引用过，则加入 def 集合
            b.def |= v->bit_vector;
        }
    }
}

//某变量是否在变量表里
var_info* find_vars(all_vars* vars, char* name)
{
    for (int i = 0; i < vars->cnt; i++)
    {
        if (!strcmp(name, vars->all[i].var_name))
        {
            return &(vars->all[i]);
        }
    }
    return NULL;
}

operand* find_opd(operation * op, int i)
{
    operand *opr = op->opers;
    if(i == 0) return opr;
    for(int j=0; j < i; j++)
    {
        opr = opr->next;
    }
    return opr;
}
//新建变量
int var_cnt = 0;
var_info * new_var(operand* o, all_vars * vars)
{
    vars->all[var_cnt++].var_name = o->o_value.name;
    return &vars->all[var_cnt-1];
}

//是不是有效的变量名
char* check_name(char * name)
{
    if(name[0] == 'v' || name[0] == 't' || name[0] == '*') return name;
    else return NULL;
}

//遍历每个变量
void traverse_var(operation *op, all_vars* vars, basic_block b)
{
    int opd_cnt = op->op_num;
    for(int i = 0; i < opd_cnt; i++)
    {
        char* c = check_name(find_opd(op, i)->o_value.name);
        if(!c) continue;
        var_info* v = find_vars(vars, c);
        if(!v)
        {
            //变量没加入，新建一个
            v = new_var(op, vars);
        }
        use_or_def(v, i, op, b);
    }
}

// 为每个基本块计算 use 和 def 集合
void calc_def_use(int len, basic_block* blocks, all_vars *vars) {
    IR_list *ir = lst_of_ir;
  for (int B = 0; B < len; B++) {
    // 遍历基本块中的所有语句
    for (int i = blocks[B].start; i <= blocks[B].end; i++) {
        operation *op = find_op(ir, i);
        //遍历每个变量
        if(have_cal(op))
        traverse_var(op, vars, blocks[B]);
    }
  }
}

// 迭代计算 IN 和 OUT 集合(从前往后的，跟ppt上相反)
void calc_in_out(int lst_len, basic_block *basic_block) 
{
    int changed = 1; // 标记是否发生了改变
    while (changed) {
        changed = 0;
        for (int B = 0; B < lst_len; B++) {
            // 先将 out 集的值保存下来，用于比较是否发生了改变
            unsigned long long old_out = basic_block[B].out;
            // 遍历 B 的所有前驱基本块 P
            for (int i = 0; i < basic_block[B].pros_cnt; i++) 
            {
                int P = basic_block[B].pros[i];
                // 将 P 的 out 集合合并到 B 的 in 集中
                basic_block[B].in |= basic_block[P].out;
            }
            // 将 B 的 use 集合拷贝到 out 集中
            basic_block[B].out = basic_block[B].use;
            // 将 out 集中的变量与 B 的 def 集合取差集
            basic_block[B].out &= ~basic_block[B].def;
            // 如果 out 集发生了改变，则需要继续进行计算
            if (basic_block[B].out != old_out) 
            {
                changed = 1;
            }
        }
    }
}

int live_var_analyser(int lst_len, basic_block *basic_block, all_vars *vars) 
{
    calc_def_use(lst_len, basic_block, vars);
    calc_in_out(lst_len, basic_block);
    return 0;
}


int sigle_func_reg_alloc(IR_list *ir, int start, int end){
    basic_block block_lst[25];
    all_vars vars;
    init_block_lst(block_lst, 25);
    init_all_vars(&vars);
    block_divide(ir, block_lst, start, end);
    live_var_analyser(25, block_lst, &vars);
    all_block_reg_alloc(ir, block_lst, 25, &vars);

    return 0;
}

int all_func_reg_alloc(IR_list *ir){
    int start = 0;
    int end = 0;
    operation* temp_ptr = ir->head;
    if(temp_ptr->code != I_FUNC){
        printf("ERROR: NOT A FUNCITON IN INTER REPERSENTATION (in all_func_reg_alloc())\n");
    }
    do{
        temp_ptr = temp_ptr->next;
        end++;
        if(temp_ptr->code == I_FUNC){
          sigle_func_reg_alloc(ir, start, end - 1);  
            start = end;
        }
    }while(temp_ptr != ir->tail); 
}


