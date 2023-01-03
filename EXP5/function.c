#include "exp.h"


//index表示定义的位置，arg_flag表示参数个数
int fun_dec(int index, int arg_flag);
//index表示call的位置，arg_flag表示参数个数
int fun_call(int index, int arg_flag);

/*
    调用函数部分：
    保存活跃变量
    存入参数
    调用
    弹出参数（如果参数个数>4)
    恢复活跃变量
*/
int fun_call(int index, int arg_flag)
{
    char *funcname = find_op(lst_of_ir, index)->opers->o_value.name;
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
    将\$ra压栈
    \$fp压栈并设置好新的\$fp
    保存s寄存器
    取出参数
    恢复寄存器
    恢复栈顶
    跳转回去
*/
int fun_dec(int index, int arg_flag)
{

}