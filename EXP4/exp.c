//不想看报错
#include "exp.h"
#define I_AND 198964
#define I_OR 198965
#define I_NOT 198966
int isIO = 0;//是否为read或者write



//返回是否报错，若只有一个参数，第二个参数写1
int check_error(int a, int b)
{
    if (a == -1 || b == -1)
    {
        return 1;
    }
    return 0;
}

//返回对应类型的宏
int find_type(treeNode *n)
{
    char *name = n->subtype.IDVal;
    if (ifExistStruct(*struct_table, name))
    {

        return charToInt(name, *struct_table);
    }
    else if (ifExistFunc(*fun_table, name))
    {
        return 5;
    }
    else
    {
        return getNodeVar(var_domain_ptr->tVar, name).varType;
    }
}


//处理Exp节点
operand* Exp_s(treeNode *exp)
{
    //处理Exp
    /*Exp ->
      Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp

    | LP Exp RP
    | MINUS Exp
    | NOT Exp

    | ID LP Args RP 函数
    | ID LP RP

    | Exp LB Exp RB 数组
    | Exp DOT ID 结构体

    | ID
    | INT
    | FLOAT
    */

    /*
    中间代码版：
     I_ASSIGN,       //赋值操作
     I_ADD,          //加法操作
     I_SUB,          //减法操作
     I_MUL,          //乘法操作
     I_DIV,          //除法操作
     I_AS_ADDR,       //x := &y
     I_AS_VALUE,      //x := *y
     I_VALUE_ASSIGN,  //*x := y
     I_GOTO,          //无条件跳转到标号x
     I_IF,            //若满足关系则跳转
     I_RETURN,        //函数返回
     I_DEC,           //内存空间申请
     I_ARG,           //函数传实参
     I_CALL,          //函数调用
     I_READ,          //从控制台读取
     I_WRITE          //向控制台打印
    */
   
    treeNode *tn2 = getchild(exp, 1);
    treeNode *tn1 = getchild(exp, 0);

    //1-处理变量或者数或者IO   ID，INT，FLOAT，read, write
    if (tn2 == NULL)   return id_int_float(tn1);
    else
    {
        treeNode *tn3 = getchild(exp, 2);
    //2- 处理 Exp  <operator>  Exp
        if (tn3 != NULL)
        {
            treeNode *tn4 = getchild(exp, 3);
            if (tn4 == NULL &&tn3->nodeType == N_EXP && tn2->nodeType != N_LB)  
                exp_o_exp(tn1, tn2, tn3, exp);
        }

    //3- LP exp RP 调用一次exp，返回这个exp的东西,不做别的处理
        if(tn1->nodeType == N_LP)   
            return Exp_s(getchild(exp, 1));

    //3- MINUS Exp，NOT Exp
        if (tn1->nodeType == N_MINUS || tn1->nodeType == N_NOT)    
            return o_exp(tn1, tn2, tn3, exp);
    
    //4-函数部分   ID LP Args RP 有参函数  ID LP RP 无参函数
        if (tn1->nodeType == N_ID)
        { 
        //先处理IO，把read当普通的无参函数，write当不需要传参的有参函数看待
        //ID LP Args RP 有参函数 
            if (tn3->nodeType == N_ARGS)
                return fun_with_args(tn1, tn2, tn3, getchild(exp, 3));
        //无参函数
            else
            {
                return fun_no_args(tn1, tn2, tn3);
            }
        }
        else
        {
            treeNode *tn4 = getchild(exp, 3);
            if (tn4 == NULL)
            {
    //5-结构体部分Exp DOT ID----结构体存在----域名存在----返回这个域名的type
                if (tn1->nodeType == N_EXP &&
                    tn2->nodeType == N_DOT &&
                    tn3->nodeType == N_ID)
                        return exp_st(tn1, tn2, tn3);
                
            }
            else
            {
                ;
    //6-数组部分;| Exp LB Exp RB 数组
                if (tn1->nodeType == N_EXP &&
                    tn2->nodeType == N_LB &&
                    tn3->nodeType == N_EXP)
                     return exp_ar(tn1, exp);
            }
        };
    }
    return init_operand(VARIABLE, temp_op(flag)->o_value.name, 0, 0);
}


//返回是不是这个type
int check_type(treeNode *n, int type)
{
    return n->nodeType == type;
}

int op_type(int c){
    switch(c){
        case N_ASSIGNOP:
            return I_ASSIGN;
            break;
        case N_PLUS:
            return I_ADD;
            break;
        case N_STAR:
            return I_MUL;
            break;
        case N_MINUS:
            return I_SUB;
        case N_AND:
            return I_AND;
        case N_OR:
            return I_OR;
        case N_NOT:
            return I_NOT;
        default:
            return -1;
    }
}

//1-处理id int或者float的情况
operand* id_int_float(treeNode *tn1)
{
    if (tn1->nodeType == N_ID)
        { 
            exp_re = find_type(tn1); //找到了,返回这个ID代表的类型
            return init_operand(VARIABLE, getNodeVarStack(var_domain_ptr, tn1->subtype.IDVal).ir_name, 0, 0);
        }
    else 
        { //返回一个立即数类型的operand
            exp_re = D_INT;
            return init_operand(IMMEDIATE, NULL, tn1->subtype.intVal, 0);
            ;
        }
}

//2-处理 exp <operator> exp
operand* exp_o_exp(treeNode *tn1, treeNode *tn2, treeNode *tn3, treeNode *exp)
{
    treeNode *Expnode1 = tn1;
    treeNode *Expnode2 = tn3;

    if(tn3->child->nodeType == N_ID)//为read
    {
        if( !strcmp(tn3->child->subtype.IDVal, "read"))
        {
            printf("read");
            operand_list *opl = init_operand_list();
            add_operand(opl, Exp_s(Expnode1));
            new_op(lst_of_ir, I_READ, *opl);
            isIO = 0;
            return Exp_s(Expnode1);
        }
    }
    Exp_s(Expnode2);//exp_re已修改
    operand* o = Exp_s(Expnode1);
    //非关系运算  返回操作数(随便哪个exp)的类型

        if (check_type(tn2, N_AND) ||
            check_type(tn2, N_OR) ||
            check_type(tn2, N_RELOP))
        { //关系运算,返回INT
            exp_re = D_INT;
        }

        //处理运算
        operation* op = binary(exp);
        add_op(lst_of_ir, op);//加入这个运算
        operand* opr = temp_op(flag);
        flag = 0;//生成但是暂时没有用到，置于0
        return opr;
}


operand* o_exp(treeNode *tn1, treeNode *tn2, treeNode *tn3, treeNode *exp)
{
    treeNode *expnode = tn2;
    Exp_s(expnode);//修改exp_s
    if (check_type(expnode, N_NOT))
    { // NOT结果返回int
        exp_re = D_INT;
    }
    operation* op = unary(exp);
    add_op(lst_of_ir, op);//加入这个运算
    operand* opr = temp_op(flag);
    flag = 0;//生成但是暂时没有用到，置于0
    return opr;
}

//处理单元运算，输入树的节点，输出生成的operation指针
operation* unary(treeNode *t)
{
    treeNode *t0 = getchild(t, 0);
    operand_list* oplst = init_operand_list();
    int type = op_type(t0->nodeType);
    /*if(type > 1989)//为not
    {
        return and_or_not(t, type);
    }
    else//不是关系运算
    {*/
        operand* opr2 = Exp_s(getchild(t, 1));
        flag = 1;
        operand* opr1 = init_operand(IMMEDIATE, NULL, 0, 0);
        flag = 1;
        operand* opr0 = temp_op(flag);
        flag = 1;    
        add_operand(oplst, opr0);
        add_operand(oplst, opr1);
        add_operand(oplst, opr2); 
        return init_op(type, *oplst);   
    //}

}

//处理二元运算，输入树的节点，输出生成的operation指针
operation* binary(treeNode *t)
{
    treeNode *t1 = getchild(t, 1);
    operand_list* oplst = init_operand_list();
    int type = op_type(t1->nodeType);
    if(type == -1)
    {
        oplst = bool_(t, 3);//为关系运算，处理关系运算
        return init_op(I_BOOL, *oplst);
    }
    /*else if(type > 1989)
    {
        return and_or_not(t, type);
    }
    else//不是关系运算
    {*/
        operand* opr2 = Exp_s(t->child->sibling->sibling);
        flag = 1;
        operand* opr1 = Exp_s(t->child);
        flag = 1;
        operand* opr0 = temp_op(flag);
        flag = 1;    
        add_operand(oplst, opr0);
        add_operand(oplst, opr1);
        add_operand(oplst, opr2); 
        return init_op(type, *oplst);   
    //}
}

//处理关系运算，返回一个参数表（因为布尔运算的符号不算符号，而是直接当操作数来用了）
operand_list* bool_(treeNode *t, int opnum){
    treeNode* n = t->child;
    operand_list* oplst = init_operand_list();
    if(opnum == 3)
    {
        operand* opr2 = Exp_s(n->sibling->sibling);
        operand* opr0 = Exp_s(n);        
        operand* opr1 = init_operand(VARIABLE, getchild(t, 1)->subtype.IDVal, 0, 0);
        add_operand(oplst, opr0);
        add_operand(oplst, opr1);
        add_operand(oplst, opr2);
    }
    return oplst;
}


int Exp_o(treeNode *exp, char* label)
{   
    if(getchild(exp, 1)->nodeType == N_RELOP)
    {
        operand_list *oplst = NULL;
        oplst = bool_(exp, 3);
        new_operand(oplst, VARIABLE, label, 0, 0);
        new_op(lst_of_ir, I_IF, *oplst);
    }
    /*else
    {//处理and or not//不处理了 假装没有
        int type;
        if(count_child(exp) == 3) type = getchild(exp, 1)->nodeType;
        else type = I_NOT;
        operand_list *oplst = and_or_not(exp, op_type(type));
        new_operand(oplst, VARIABLE, label, 0, 0);
        new_op(lst_of_ir, oplst);
    }*/
    return lst_of_ir->length;
}

operand* fun_no_args(treeNode *tn1, treeNode *tn2, treeNode *tn3)
{
    char *funcname = tn1->subtype.IDVal;
    operand_list *opl = init_operand_list();
    add_operand(opl, temp_op(flag));
    new_operand(opl, VARIABLE, funcname, 0, 0);
    flag = 1;
    new_op(lst_of_ir, I_CALL, *opl);
    exp_re = getNodeFunc(*fun_table, funcname).returnType;
    return temp_op(0);
}

operand* fun_with_args(treeNode *tn1, treeNode *tn2, treeNode *tn3, treeNode *tn4)
{
    char *funcname = tn1->subtype.IDVal;
    treeNode *cntnode = tn3;
    int cnt = 0;    
    operand_list* opl = init_operand_list();
    if(!strcmp(funcname, "write"))
    {
        printf("%s", funcname);
        add_operand(opl, Exp_s(tn3->child));
        new_op(lst_of_ir, I_WRITE, *opl);
        return Exp_s(tn3->child);
    }
    while (1)
    {   //计算所有实参的数目
        cnt += 1;
        treeNode *tempcntnode = getchild(cntnode, 2);

        //获取实参
        add_operand(opl, Exp_s(getchild(cntnode, 0)));

        if (tempcntnode == NULL)
        {
            break;
        }
        cntnode = getchild(cntnode, 2);
    }
    operand_list *opl2 = init_operand_list();
    add_operand(opl2, temp_op(flag));
    new_operand(opl2, VARIABLE, funcname, 0, 0);
    flag = 1;
    new_op(lst_of_ir, I_ARG, *opl);
    new_op(lst_of_ir, I_CALL, *opl2);
    
    exp_re = getNodeFunc(*fun_table, funcname).returnType;
    return temp_op(0);
}

operand* exp_st(treeNode *tn1, treeNode *tn2, treeNode *tn3)
{
    operand* op0 = Exp_s(tn1);
    int offset = struct_offset(struct_table, getchild(tn1, 0)->subtype.IDVal, tn3->subtype.IDVal);
    operand_list *oplst= init_operand_list();
    add_operand(oplst, temp_op(flag));//ti
    add_operand(oplst, op0);//vi
    flag = 0;
    if(offset == 0)
    {
        new_op(lst_of_ir, I_ASSIGN, *oplst);
    }
    else
    {
        new_operand(oplst, ADDRESS, NULL, offset, 0);//地址偏移
        new_op(lst_of_ir, I_AS_ADDR, *oplst);
    }
    char ret_char[5] = "*";
    //strcat(ret_char, "*");
    strcat(ret_char, temp_op(flag)->o_value.name);//*ti
    flag = 1;
    return init_operand(VARIABLE, ret_char, 0, 0);
}

operand* exp_ar(treeNode *tn1, treeNode *exp)
{
        //返回元素的类型
    exp_re = getNodeVarStack(var_domain_ptr, get_ar_name(tn1)).arrayVarType;
    operand_list *opl = init_operand_list();
    add_operand(opl, temp_op(flag));
    flag = 1;
    new_operand(opl, VARIABLE, get_ar_name(tn1), 0, 0); 
    int* dimlen = getNodeVarStack(var_domain_ptr, get_ar_name(tn1)).len_of_dims;
    new_operand(opl, IMMEDIATE, NULL, byte_len(exp_re)*arr_offset(dimlen, exp, 0, 0), 0);
    new_op(lst_of_ir, I_AS_ADDR, *opl);

    

    char ret_char[5] = "*";
    strcat(ret_char, temp_op(0)->o_value.name);//*ti
    flag = 1;
    return init_operand(VARIABLE, ret_char, 0, 0);
}

int byte_len(int type)
{
    return 4;
}

char* get_ar_name(treeNode *tn1)
{
    treeNode *t = tn1;
    while(t->child != NULL)
    {
        t = t->child;
    }
    return get_ir(t, &(var_domain_ptr->tVar));
}

int arr_offset(int *dimlen, treeNode* t, int n, int tag)
{
    if(getchild(t, 0)->nodeType == N_ID)//base case
    {
        return dimlen[n]*getchild(t, 2)->subtype.intVal;
    }
    else
    {
        if(!tag)
        {
            return getchild(t, 2)->child->subtype.intVal + arr_offset(dimlen, t->child, n, 1);
        }
        else
        {
            return dimlen[n]*(getchild(t, 2)->child->subtype.intVal) + arr_offset(dimlen, t->child, n + 1, 1);
        }
    }
}