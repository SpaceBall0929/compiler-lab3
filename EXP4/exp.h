#ifndef EXP_H
#define EXP_H
#include <stdio.h>
#include "DomainStack.c"
#include "Ir.c"

//是否有用到这个临时变量,没用到=0,用到了=1
int flag = 0;

//exp的返回类型
int exp_re;

// 中间代码使用的表
IR_list *lst_of_ir;

// 用到的变量作用域，函数和结构体表
stackNode *var_domain_ptr;
SymbolTableFunc *fun_table;
SymbolTableStruct *struct_table;

//处理Exp节点
operand* Exp_s(treeNode *exp);
//分别处理exp的几种情况
    //1-处理id int或者float的情况
    operand* id_int_float_IO(treeNode *tn1);

    //2-处理 exp <operator> exp
    operand* exp_o_exp(treeNode *tn1, treeNode *tn2, treeNode *tn3, treeNode *exp);
        //处理二元运算，输入树的节点，输出生成的operation指针
        operation* binary(treeNode *t);
        //处理关系运算，返回一个参数表（因为布尔运算的符号不算符号，而是直接当操作数来用了）
        operand_list* bool(treeNode *t, int opnum);
        //处理and or not这三种情况
        operation* and_or_not(treeNode*t, int type);

    //3-处理MINUS Exp，NOT Exp,lp exp rp直接在exp里处理
    operand* o_exp(treeNode *tn1, treeNode *tn2, treeNode *tn3, treeNode *exp);
        //处理单元运算，输入树的节点，输出生成的operation指针
        operation* unary(treeNode *t);
    //4-处理函数 无参&有参
    operand* fun_no_args(treeNode *tn1, treeNode *tn2, treeNode *tn3);
    operand* fun_with_args(treeNode *tn1, treeNode *tn2, treeNode *tn3, treeNode *tn4);
    //5-处理结构体
    operand* exp_st(treeNode *tn1, treeNode *tn2, treeNode *tn3);
    //6-处理数组
    operand* exp_ar(treeNode *tn1, treeNode *exp);
        //获取数组的ir名
        char* get_ar_name(treeNode *tn1);
        //计算偏移量(要手动乘byte_len)
        int arr_offset(int *dimlen, treeNode* t, int n, int);
//返回是不是这个type
int check_type(treeNode* t, int i);

//返回operator的类型，若为关系型返回-1，只在当前是操作符的时候调用。
int op_type(int c);

//获取这个变量类型占多少byte
int byte_len(int type);

//处理if和while条件里的exp节点,返回这个的index值
int Exp_o(treeNode *exp, char* label);
//生成变量名字的函数
char *var_name_gen();
#endif
