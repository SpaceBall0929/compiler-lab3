#include "exp.h"
/*
    未完工部分：
    参数特多的情况
*/

//index表示定义的位置，arg_flag表示参数个数
int fun_pdec(int index, int arg_flag);
//index表示call的位置，arg_flag表示参数个数
int fun_call(int index, int arg_flag, char*);

/*
    调用函数部分：
    保存活跃变量(提供接口，由组员在调用函数前保存)
    存入参数
    调用
    弹出参数（如果参数个数>4)
    恢复活跃变量
*/
int fun_call(int index, int arg_flag, char *funcname)
{
    //char *funcname = find_op(lst_of_ir, index)->opers->o_value.name;
    //保存活跃变量

    //存入参数
    index += 1;
    if(arg_flag)
    {
        //有参数
        for(int i = 0; i < arg_flag; i++)
        {
            add_move(index, i);
            index += 1;
        }
        if(arg_flag > 4)
        {
            //参数较多,移动到栈里
        }
    }

    //调用
    index += 1;
    add_jal(index, funcname);

    //弹出参数（如果参数个数>4)

    //恢复活跃变量
    index += 1;
    return index;
}
//获取参数（arg_no表示第几个参数,从1开始）
operand* get_arg(int index, int arg_no)
{
    if(!arg_no) return NULL;
    operation* op = find_op(lst_of_ir, index);
    operand* opr = op->opers;
    for(int i = 0; i < arg_no - 1; i++)
    {
        opr = opr->next;
    }
    return opr;
}

//获取第n个参数寄存器的名字
char* get_reg(int n)
{
    switch(n){
        case 0: return "a0";
        case 1: return "a1";
        case 2: return "a2";
        case 3: return "a3";
    }
}

//保存活跃变量,reg表示寄存器名称，offset表示栈的偏移
void sw_live(int index, char* reg, int offset, int m_index)
{
    operand_list* opl = init_operand_list();
    new_operand(opl, VARIABLE, reg, 0, 0);
    new_operand(opl, IMMEDIATE, NULL, offset, 0);
    //new_operand(opl, VARIABLE, "$sp", 0, 0);
    insert_op(lst_of_ir, I_SW, *opl, m_index);
}

//恢复活跃变量，reg表示寄存器名称，offset表示栈的偏移
void lw_live(int index, char* reg, int offset, int m_index)
{
    operand_list* opl = init_operand_list();
    new_operand(opl, VARIABLE, reg, 0, 0);
    new_operand(opl, IMMEDIATE, NULL, offset, 0);
    new_operand(opl, VARIABLE, "$sp", 0, 0);
    insert_op(lst_of_ir, I_LW, *opl, m_index);
}
//参数（寄存器里的），move
void add_move(int index, int i)
{
    operand_list* opl = init_operand_list();
    add_operand(opl, get_arg(index, i));
    new_operand(opl, VARIABLE, get_reg(i), 0, 0);
    insert_op(lst_of_ir, I_MOVE, *opl, index);
}

void add_jal(int index, char* funcname)
{
    operand_list* opl = init_operand_list();
    new_operand(opl, VARIABLE, funcname, 0, 0);
    insert_op(lst_of_ir, I_JAL, *opl, index);
}

/*
    定义函数部分：
    *******以下为Prologue部分*********
    将\$ra压栈
    \$fp压栈并设置好新的\$fp
    保存s寄存器
    取出参数
    *******以下为Epilogue部分*********
    恢复寄存器
    恢复栈顶
    跳转回去
*/

int size = 0;

//在函数定义开头调用
int fun_pdec(int index, int arg_flag)
{
    //分配栈的空间
    size = get_offset();

    //将\$ra压栈
    index += 1;
    sw_ra(size - 4, index);

    //\$fp压栈并设置好新的\$fp
    index += 1;
    sw_fp(size - 8, index);
    index += 1;
    addi_fp(size, index);

    //保存s寄存器
    index += 1;
    sw_sreg(index, size - 12);

    //取出参数
    index += 1;
        if(arg_flag > 4)
        {
            //参数较多,在栈里
        }
    return index;
}

//函数定义结束调用
int fun_edec(int index, int arg_flag)
{
    //恢复寄存器
    lw_sreg(index, size - 12);

    //恢复ra
    index += 1;
    lw_ra(size - 4, index);

    //恢复fp和栈顶
    index += 1;
    lw_fp(size - 8, index);
    index += 1;
    addi_sp(size, index);

    //跳转回去
    index += 1;
    jr_ra(index);
    return index;
}

//获取栈的分配空间并加入语句**********
int get_offset()
{
    int size = 44;
    return size;
}

//将\$ra压栈
void sw_ra(int offset, int index)
{
    operand_list *opl = init_operand_list();
    new_operand(opl, VARIABLE, "$ra", 0, 0);
    new_operand(opl, IMMEDIATE, NULL, offset, 0);
    insert_op(lst_of_ir, I_SW, *opl, index);
}

void sw_fp(int offset, int index)
{
    operand_list *opl = init_operand_list();
    new_operand(opl, VARIABLE, "$fp", 0, 0);
    new_operand(opl, IMMEDIATE, NULL, offset, 0);
    insert_op(lst_of_ir, I_SW, *opl, index);
}

void addi_fp(int offset, int index)
{
    operand_list *opl = init_operand_list();
    new_operand(opl, VARIABLE, "$fp", 0, 0);
    new_operand(opl, VARIABLE, "$sp", 0, 0);
    new_operand(opl, IMMEDIATE, NULL, offset, 0);
    insert_op(lst_of_ir, I_ADDI, *opl, index);
}
void lw_ra(int offset, int index)
{
    operand_list *opl = init_operand_list();
    new_operand(opl, VARIABLE, "$ra", 0, 0);
    new_operand(opl, IMMEDIATE, NULL, offset, 0);
    insert_op(lst_of_ir, I_LW, *opl, index);
}

void lw_fp(int offset, int index)
{
    operand_list *opl = init_operand_list();
    new_operand(opl, VARIABLE, "$fp", 0, 0);
    new_operand(opl, IMMEDIATE, NULL, offset, 0);
    insert_op(lst_of_ir, I_LW, *opl, index);
}

void addi_sp(int offset, int index)
{
    operand_list *opl = init_operand_list();
    new_operand(opl, VARIABLE, "$sp", 0, 0);
    new_operand(opl, VARIABLE, "$sp", 0, 0);
    new_operand(opl, IMMEDIATE, NULL, offset, 0);
    insert_op(lst_of_ir, I_ADDI, *opl, index);
}
void jr_ra(int index)
{
    operand_list *opl = init_operand_list();
    new_operand(opl, VARIABLE, "$ra", 0, 0);
    insert_op(lst_of_ir, I_JR, *opl, index);
}

void sw_sreg(int index, int offset)
{
    int o = offset;
    for(int i = 0; i < 8; i ++)
    {
        sw_1s(index, i, o);
        o -= 4;
        index += 1;
    }
}

void sw_1s(int index, int reg, int offset)
{
    operand_list *opl = init_operand_list();
    new_operand(opl, VARIABLE, get_sreg(reg), 0, 0);
    new_operand(opl, IMMEDIATE, NULL, offset, 0);
    new_operand(opl, VARIABLE, "$sp", 0, 0);
    insert_op(lst_of_ir, I_SW, *opl, index);
}

//获取第n个s寄存器的名字
char* get_sreg(int n)
{
    switch(n){
        case 0: return "s0";
        case 1: return "s1";
        case 2: return "s2";
        case 3: return "s3";
        case 4: return "s4";
        case 5: return "s5";
        case 6: return "s6";
        case 7: return "s7";
        case 8: return "s8";
    }
}

void lw_sreg(int index, int offset)
{
    int o = offset;
    for(int i = 0; i < 8; i ++)
    {
        lw_1s(index, i, o);
        o -= 4;
        index += 1;
    }
}

void lw_1s(int index, int reg, int offset)
{
    operand_list *opl = init_operand_list();
    new_operand(opl, VARIABLE, get_sreg(reg), 0, 0);
    new_operand(opl, IMMEDIATE, NULL, offset, 0);
    new_operand(opl, VARIABLE, "$sp", 0, 0);
    insert_op(lst_of_ir, I_LW, *opl, index);
}