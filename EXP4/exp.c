//不想看报错
#include "exp.h"


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

    //1-处理变量或者数   ID，INT，FLOAT
    if (tn2 == NULL)   return id_int_float(tn1);
    else
    {
        treeNode *tn3 = getchild(exp, 2);
    //2- 处理 Exp  <operator>  Exp
        if (tn3 != NULL)
        {
            treeNode *tn4 = getchild(exp, 3);
            if (tn4 == NULL &&tn3->nodeType == N_EXP && tn2->nodeType != N_LB)  
                exp_o_exp(tn1, tn2, tn3);
        }

    //3- LP exp RP 调用一次exp，返回这个exp的东西,不做别的处理
        if(tn1->nodeType == N_LP)   
            return Exp_s(getchild(exp, 1));

    //3- MINUS Exp，NOT Exp
        if (tn1->nodeType == N_MINUS || tn1->nodeType == N_NOT)    
            return o_exp(tn1, tn2, tn3);
    
        //第三部分;
        /*
        | ID LP Args RP 有参函数
        | ID LP RP 无参函数

        | Exp LB Exp RB 数组
        | Exp DOT ID 结构体;
        */
        //函数部分：判断第一个是不是ID;需要检查这个函数的存在性,得到函数的params交给下一层检查,并且查看这个ID是不是函数类型
        if (tn1->nodeType == N_ID)
        { //当前为函数，需要去检查该函数是否已定义
            int tag = 0;
            fflush(stdout);

            char *funcname = tn1->subtype.IDVal;

            int queryresult = ifExistFunc(*fun_table, funcname);        //在全局里面搜索;
            dataNodeFunc func_node = getNodeFunc(*fun_table, funcname); //搜素这个函数节点
            // char *tfun, *f_fun = NULL;
            char tfun[100];
            strcpy(tfun, funcname);
            strcat(tfun, "(");
            char efun[100];
            strcpy(efun, funcname);
            strcat(efun, "(");

            // printf("%s\n%s\n", tfun, efun);

            dataNodeVar *argNode = func_node.args;
            // printf("%d",argNode==NULL);
            while (argNode != NULL)
            {
                strcat(tfun, find_type2(argNode->varType, argNode->varName));
                argNode = argNode->next;
                if (argNode != NULL)
                    strcat(tfun, ", ");
            }
            strcat(tfun, ")");
            fflush(stdout);

            if (tn3->nodeType == N_ARGS)
            { //检查args的数量;

                int cnt = 0;
                treeNode *cntnode = tn3;
                while (1)
                { //计算所有实参的数目
                    cnt += 1;
                    treeNode *tempcntnode = getchild(cntnode, 2);

                    //获取实参
                    int type1 = Exp_s(getchild(cntnode, 0));
                    // if(getchild(cntnode, 0)->nodeType == )
                    strcat(efun, find_type2(type1, getchild(cntnode, 0)->subtype.IDVal));

                    if (tempcntnode == NULL)
                    {
                        break;
                    }
                    // cnt+=1;

                    strcat(efun, ", "); //加个逗号
                    cntnode = getchild(cntnode, 2);
                }

                if (func_node.args == NULL)
                { //函数本身没有形参，但此时有实参
                    /*error_msg(9, exp->line_no, funcname); //错误类型9，函数实参形参不匹配
                    return -1;*/
                    // printf("func没参数\n");
                    tag = exp->line_no;
                }
                else
                {
                    // Args -> Exp COMMA Args
                    //| Exp;
                    if (cnt != getArgNum(*fun_table, funcname))
                    {
                        /*error_msg(9, exp->line_no, funcname); //错误类型9，函数实参形参个数不匹配
                        return -1;*/
                        tag = exp->line_no;
                    }
                    if (!tag)
                    {
                        int argresult = Arg_s(tn3, func_node.args, funcname);
                        if (argresult != 0)
                        {
                            /*error_msg(9, tn3->line_no, funcname); //错误类型9，函数实参形参类型不匹配
                            return result;*/
                            tag = tn3->line_no;
                        }
                        else
                        {
                            return ret_type;
                        }
                    }
                }
            }
            else
            {
                
                if (func_node.args != NULL)
                { //函数有形参，但此时没有实参
                    /*error_msg(9, exp->line_no, funcname); //错误类型9，函数实参形参个数不匹配
                    return -1;*/
                    tag = exp->line_no;
                }
                else
                {
                    return ret_type;
                }
            }
            if (tag)
            {
                strcat(efun, ")");
                printf("Error type %d at Line %d: ", 9, tag);
                printf("Function \"%s\" is not applicable for arguments\"%s\". \n", efun, tfun);
            }
        }
        else
        {
            treeNode *tn4 = getchild(exp, 3);

            if (tn4 == NULL)
            {
                //结构体部分Exp DOT ID----结构体存在----域名存在----返回这个域名的type
                if (tn1->nodeType == N_EXP &&
                    tn2->nodeType == N_DOT &&
                    tn3->nodeType == N_ID)
                {
                    int exptype = Exp_s(tn1);

                    if (!check_error(exptype, 1)) //已保证结构体存在
                    {
                        
                        dataNodeStruct stru_node = *getNodeStruct(*struct_table, tn1->child->subtype.IDVal);
                        /*111if(IF_DEBUG_PRINT)*/ printf("type: %d\n", exptype);
                        if (exptype < 6)
                        {                                      //当前Exp不是结构体
                            error_msg(13, exp->line_no, NULL); //错误类型13，对非结构体变量使用“.”
                            return -1;
                        }
                        else
                        {
                            //搜索域名;
                            if (ifExistStructDomain(*struct_table, tn3->nodeType, tn3->subtype.IDVal))
                            {
                                //找到了!
                                result = find_type(tn3);
                                return result;
                            }
                            else
                            {
                                //域名不存在;
                                error_msg(14, exp->line_no, tn3->subtype.IDVal); //错误类型14，该域没有在访问结构体中未定义
                                return -1;
                            }
                        };
                    }
                    else
                    {
                        return -1; //把错误往上传（已经报过错）
                    }
                }
            }
            else
            {
                ;
                //数组部分;| Exp LB Exp RB 数组
                if (tn1->nodeType == N_EXP &&
                    tn2->nodeType == N_LB &&
                    tn3->nodeType == N_EXP)
                {
                    int type1 = Exp_s(tn1);
                    int type3 = Exp_s(tn3);
                    if (check_error(type1, type3))
                    {
                        return -1;
                    }
                    if (type1 != 2) // type1不是数组
                    {
                        error_msg(10, exp->line_no, tn1->child->subtype.IDVal); //错误类型10，对非数组变量进行数组访问
                        return -1;
                    }
                    else
                    {
                        if (type3 == D_INT)
                        { // Exp是整数
                            ;
                        }
                        else
                        { // Exp不是整数
                            if (tn3->child->nodeType == N_ID)
                                error_msg(12, exp->line_no, tn3->child->subtype.IDVal); //错误类型12，数组访问符中出现非整数
                            else
                            {
                                printf("Error type %d at Line %d: ", 12, exp->line_no);
                                printf("\"%.2f\" is not an integer.\n", tn3->child->subtype.floatVal);
                            }
                            return -1;
                        }
                    }
                    //返回元素的类型
                    dataNodeVar var_node = getNodeVar(var_domain_ptr->tVar, tn3->child->subtype.IDVal);
                    result = var_node.varType;
                    return result;
                }
            }
        };
    }
    return result;
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
        else if (tn1->nodeType == N_INT)
        { //返回一个立即数类型的operand
            exp_re = D_INT;
            return init_operand(IMMEDIATE, NULL, tn1->subtype.intVal, 0);
            ;
        }
}

//2-处理 exp <operator> exp
operand* exp_o_exp(treeNode *tn1, treeNode *tn2, treeNode *tn3)
{
    treeNode *Expnode1 = tn1;
    treeNode *Expnode2 = tn3;

    int exp1type = Exp_s(Expnode1);
    int exp2type = Exp_s(Expnode2);
    if (!check_error(exp1type, exp2type))
    {
        //非关系运算  返回操作数(随便哪个exp)的类型
        exp_re = exp1type;

        if (check_type(tn2, N_AND) ||
            check_type(tn2, N_OR) ||
            check_type(tn2, N_RELOP))
        { //关系运算,返回INT
            exp_re = D_INT;
        }

        //处理运算
        operation* op = binary(exp);
        add_op(lst_of_ir, op);//加入这个运算
        operand* opr = init_operand(VARIABLE, temp_op(flag), 0, 0);
        flag = 0;//生成但是暂时没有用到，置于0
        return opr;
}
}

operand* o_exp(treeNode *tn1, treeNode *tn2, treeNode *tn3)
{
    treeNode *expnode = tn2;
    int exp1type = Exp_s(expnode);
    exp_re = exp1type;
    if (check_type(expnode, N_NOT))
    { // NOT结果返回int
        exp_re = D_INT;
    }
    operation* op = unary(exp);
    add_op(lst_of_ir, op);//加入这个运算
    operand* opr = init_operand(VARIABLE, temp_op(flag), 0, 0);
    flag = 0;//生成但是暂时没有用到，置于0
    return opr;
}

//处理单元运算，输入树的节点，输出生成的operation指针
operation* unary(treeNode *t)
{
    treeNode *t0 = getchild(t, 0);
    operand_list* oplst = init_operand_list();
    int type = op_type(t0->nodeType);
    if(type == -1)
    {
        operand_list o_lst = bool(t, 2);//为关系运算，处理关系运算
        return init_op(I_BOOL, o_lst, 2);
    }
    else//不是关系运算
    {
        operand* opr2 = Exp_s(getchild(t, 1));
        flag = 1;
        operand* opr1 = init_operand(IMMEDIATE, NULL, 0, 0);
        flag = 1;
        operand* opr0 = temp_op(flag);
        flag = 1;    
        add_operand(oplst, opr0);
        add_operand(oplst, opr1);
        add_operand(oplst, opr2); 
        return init_op(type, *oplst, 3);   
    }

}

//处理二元运算，输入树的节点，输出生成的operation指针
operation* binary(treeNode *t)
{
    treeNode *t1 = getchild(t, 1);
    operand_list* oplst = init_operand_list();
    int type = op_type(t1->nodeType);
    if(type == -1)
    {
        operand_list o_lst = bool(t, 3);//为关系运算，处理关系运算
        return init_op(I_BOOL, o_lst, 3);
    }
    else//不是关系运算
    {
        operand* opr2 = Exp_s(t->child->sibling->sibling);
        flag = 1;
        operand* opr1 = Exp_s(t->child);
        flag = 1;
        operand* opr0 = temp_op(flag);
        flag = 1;    
        add_operand(oplst, opr0);
        add_operand(oplst, opr1);
        add_operand(oplst, opr2); 
        return init_op(type, *oplst, 3);   
    }
}

//处理关系运算，返回一个参数表（因为布尔运算的符号不算符号，而是直接当操作数来用了）
operand_list bool(treeNode *t, int opnum){
    treeNode* n = t->child;
    operand_list* oplst = init_operand_list();
    if(opnum == 3)
    {
        operand* opr2 = Exp_s(n->sibling->sibling);
        operand* opr0 = Exp_s(n);        
        operand* opr1 = init_operand(VARIABLE, n->subtype.IDVal, 0, 0);
        add_operand(oplst, opr0);
        add_operand(oplst, opr1);
        add_operand(oplst, opr2);
    }
    else if(opnum == 2)
    {
        operand* opr2 = Exp_s(n->sibling);
        operand* opr1 = init_operand(VARIABLE, n->subtype.IDVal, 0, 0);
        add_operand(oplst, opr1);
        add_operand(oplst, opr2);        
    }
}


int Exp_o(treeNode *exp)
{
    int child_num = count_child(exp);
     operand_list *oplst = NULL;
    switch(child_num)
    {
        //关于if里出现NOT怎么打印有待商榷
        case(2)://NOT exp
            oplst = bool(exp, 2);
            break;
        case(3)://exp relop|and|or exp
            oplst = bool(exp, 3);
            break;
    }
    new_op(lst_of_ir, I_IF, oplst, 3);
    return lst_of_ir->length;
}