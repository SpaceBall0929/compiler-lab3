#include <stdio.h>
#include "DomainStack.c"

// 用到的变量作用域，函数和结构体表
stackNode *var_domain_ptr;
SymbolTableFunc *fun_table;
SymbolTableStruct *struct_table;

// 识别非终结符VarDec,收集变量的名字，收集是否是数组，返回一个初始化好的dataNodeVar
//判定specifier的指向：整形/浮点/结构体定义/结构体使用
void error_msg(int type, int line_no, char *content)
{ //报错
    printf("Error type %d at Line %d: ", type, line_no);
    switch (type)
    {
    case 1:
        printf("Undefined var \"%s\".\n", content);
        break;
    case 2:
        printf("Undefined function \"%s\".\n", content);
        break;
    case 3:
        printf("Redefined var \"%s\".\n", content);
        break;
    case 4:
        printf("Redefined function \"%s\".\n", content);
        break;
    case 5:
        printf("Type mismatched for assigment.\n");
        break;
    case 6:
        printf("The left-hand side of an assignment must be a var.\n");
        break;
    case 7:
        printf("Type mismatched for operands.\n");
        break;
    case 8:
        printf("Type mismatched for return.\n");
        break;
    case 9:
        printf("Function is not applicable for arguments.\n");
        break;
    case 10:
        printf("This is not an array.\n");
        break;
    case 11:
        printf("\"%s\" is not a function.\n", content);
        break;
    case 12:
        printf("This is not an integer.\n");
        break;
    case 13:
        printf("Illegal use of \".\".\n");
        break;
    case 14:
        printf("Non-existent field \"%s\".\n", content);
        break;
    case 15:
        printf("Redefined field \"%s\".\n", content);
        break;
    case 16:
        printf("Duplicated character \"%s\".\n", content);
        break;
    case 17:
        printf("Undefined structure \"%s\".\n", content);
        break;
    case 18:
        printf("Undefined function \"%s\".\n", content);
        break;
    case 19:
        printf("Inconsistent declaration of function \"%s\".\n", content);
        break;
    default:
        printf("Wrong semantic type:%s\n", content);
        //
        break;
    }
}


int specifier(treeNode *speci)
{
    switch (speci->child->nodeType)
    {
    case N_TYPE:
        if (speci->child->subtype.IDVal[0] == 'i')
        {
            return D_INT;
        }
        return D_FLOAT;

        break;

    case N_STRUCT_SPECI:
        if (speci->child->child->sibling->nodeType == N_TAG)
        {
            return D_STRUCT_DEC;
        }
        return D_STRUCT_DEF;
    default:
        printf("ERROR: Unexpected nodeType in the child of Specifier\n");
        return 0;
        break;
    }
}

dataNodeVar *var_dec(treeNode *dec_node, int var_type)
{
    dataNodeVar *new_var;
    treeNode *origrn = dec_node;
    dec_node = dec_node->child;
    int dimension = 0;
    int dimensionlen[10];
    while (dec_node->nodeType != N_ID)
    {
        dec_node = dec_node->child;
        dimensionlen[dimension++] = dec_node->sibling->sibling->subtype.intVal;
    }
    new_var = newNodeVar(dec_node->subtype.IDVal, var_type);
    if (dimension == 0)
    {
        return new_var;
    }

    new_var->numdim = dimension;
    new_var->len_of_dims = (int *)malloc(sizeof(int) * dimension);
    for (int i = 0; i < dimension; i++)
    {
        new_var->len_of_dims[i] = dimensionlen[dimension - 1 - i];
    }
    return new_var;
}

dataNodeVar *param_dec(treeNode *para)
{
    int type_def = specifier(para->child);
    if (type_def == D_STRUCT_DEF)
    {
        printf("ERROR: Unexpected treeNode: structure definition in the parameters of the function.\n");
        return NULL;
    }

    return var_dec(para->child->sibling, type_def);
}

//处理参数表
dataNodeVar *var_list(treeNode *arg_list)
{
    arg_list = arg_list->child;
    dataNodeVar *temp_node = param_dec(arg_list);
    dataNodeVar *ptr = temp_node;
    arg_list = arg_list->sibling;
    while (arg_list != NULL)
    {
        arg_list = arg_list->sibling->child;
        ptr->next = param_dec(arg_list);
        ptr = ptr->next;
        arg_list = arg_list->sibling;
    }
    ptr->next = NULL;

    return temp_node;
}

//处理FunDec
dataNodeFunc *fun_dec(treeNode *dec_node, int return_type)
{
    treeNode *temp_node = dec_node->child->sibling->sibling;
    dataNodeVar *arg_list = NULL;
    if (temp_node->nodeType == N_VAR_L)
    {
        arg_list = var_list(temp_node);
    }
    return newNodeFunc(dec_node->child->subtype.IDVal, return_type, 0, arg_list);
}



// dataNodeVar *dec_list(treeNode *decs, int var_type)
// {
//     dataNodeVar *temp_var;
//     dataNodeVar *return_vars;
//     decs = decs->child;
//     temp_var = var_dec(decs->child, var_type);
//     return_vars = temp_var;
//     if (decs->child->sibling != NULL)
//     {
//         if (charToInt(var_type, *struct_table) != Exp_s(decs->child->sibling->sibling))
//         {
//             error_msg(5, decs->line_no, decs->child->child->subtype.IDVal);
//         }
//     }
//     decs = decs->sibling;
//     while (decs != NULL)
//     {
//         decs = decs->sibling->child;
//         temp_var->next = var_dec(decs->child, var_type);
//         temp_var = temp_var->next;
//         if (decs->child->sibling != NULL)
//         {
//             if (charToInt(var_type, *struct_table) != Exp_s(decs->child->sibling->sibling))
//             {
//                 error_msg(5, decs->line_no, decs->child->child->subtype.IDVal);
//             }
//         }
//         decs = decs->sibling;
//     }

//     temp_var = NULL;
//     return return_vars;
// }

// int dec_list(treeNode* decs, char* var_type){
//     dataNodeVar* temp_var;

//     do{
//         decs = decs  -> child;
//         InsertVar(&(var_domain_ptr -> tVar), var_dec(decs -> child, var_type));
//         if(decs -> sibling != NULL){
//             if(charToInt(var_type, struct_table) !=Exp_s(decs -> sibling -> sibling)){
//                 error_msg(5, decs ->line_no, decs -> child -> child -> subtype.IDVal);
//             }
//         }
//         decs = decs -> sibling;
//         if(decs != NULL){
//             decs = decs -> sibling;
//         }
//     }while(decs != NULL);

//     return 0;
// }

// dataNodeVar *def_list(treeNode *defs)
// {
//     // int temp_type;
//     dataNodeVar *return_vars;
//     dataNodeVar *temp_ptr;
//     int def_type;
//     defs = defs->child;
//     if (defs == NULL)
//     {
//         return NULL;
//     }
//     def_type = specifier(defs->child);
//     return_vars = dec_list(defs->child->sibling, def_type);
//     temp_ptr = return_vars;
//     defs = defs->sibling->child;
//     while (defs != NULL)
//     {
//         temp_ptr = temp_ptr->next;
//         temp_ptr = dec_list(defs->child->sibling, def_type);
//         defs = defs->sibling->child;
//     }
//     temp_ptr = NULL;

//     return return_vars;
// }

// int comp_stmt(treeNode *comp_stmt, int expected_type)
// {
//     dataNodeVar *all_vars;
//     dataNodeVar *temp_for_del;
//     domainPush(var_domain_ptr);
//     all_vars = def_list(comp_stmt->child->sibling);
//     while (all_vars != NULL)
//     {
//         temp_for_del = all_vars;
//         InsertVar(&(var_domain_ptr->tVar), all_vars);
//         all_vars = all_vars->next;
//         free(temp_for_del);
//     }

//     Stmt_s(comp_stmt->child->sibling->sibling, expected_type);
//     domainPop(var_domain_ptr);

//     return 0;
// }

// int struct_specifier_def(treeNode *def_node)
// {
//     char *name = NULL;
//     def_node = def_node->child->sibling;
//     if (def_node->child != NULL)
//     {
//         name = def_node->sibling->child->subtype.IDVal;
//     }
//     dataNodeStruct *new_struc = newNodeStruct(name);
//     dataNodeVar *temp_for_del = new_struc->structDomains;
//     dataNodeVar *temp2;
//     insertStructDomain(new_struc, def_list(def_node->sibling->sibling));
//     InsertStruct(struct_table, *new_struc);
//     while (temp_for_del != NULL)
//     {
//         temp2 = temp_for_del;
//         temp_for_del = temp_for_del->next;
//         free(temp2);
//     }
//     free(new_struc);

//     return 0;
// }

int struct_specifier_dec(treeNode *dec_node)
{
    dec_node = dec_node->child->sibling->child;
    int temp = charToInt(dec_node->subtype.IDVal, *struct_table);
    if (temp == -1)
    {
        error_msg(17, dec_node->line_no, dec_node->subtype.IDVal);
    }
    return temp;
}

/**************************
 *       以下施工完了        *
 ***************************/
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

//返回是不是这个type
int check_type(treeNode *n, int type){
    return n->nodeType == type;
}

int Exp_s(treeNode *exp);

//处理Arg_s，判断实参和形参类型是否匹配（数量是否匹配已在exp中判断过）
int Arg_s(treeNode *args, dataNodeVar *params)
{
    /*Args ->
      Exp COMMA Args
    | Exp;
    */
    treeNode *expnode = getchild(args, 0);
    treeNode *tempnode = getchild(args, 1); //判断实参是否还有参数

    int temptype = Exp_s(expnode); //检查函数形参和实参类型是否匹配
    if (temptype == params->varType)
    {
        error_msg(9, args->line_no, NULL); //错误类型9，函数实参形参类型不匹配
        return -1;
    }

    if (tempnode != NULL)
    { //实参还有参数，继续检查下一个实参和形参是否匹配
        treeNode *argsnode = getchild(args, 2);
        return Arg_s(argsnode, params->next);
    }
    return 0;
}


//返回是否报错，若只有一个参数，第二个参数写1
int check_error(int a, int b){
    if(a == -1 || b == -1){
        return 1;
    }
    return 0;
}

//处理Exp节点，返回对应类型，如果有错误返回-1
int Exp_s(treeNode *exp)
{ //处理Exp
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
    if (exp == NULL)
    {
        return -1;
    };
    int result = -1;

    treeNode *tempnode1 = getchild(exp, 0);
    treeNode *tempnode2 = getchild(exp, 1);

    // ID, EXP DOT ID(结构体), Exp LB Exp RB (数组)
    if (tempnode1->nodeType == N_EXP)
    {
        if (tempnode2 != NULL && tempnode2->nodeType == N_ASSIGNOP)
        { // Exp ASSIGNOP Exp
            treeNode *tempnode11 = getchild(tempnode1, 0);
            treeNode *tempnode12 = getchild(tempnode1, 1);
            if (tempnode12 == NULL)
            {
                if (tempnode11->nodeType != N_ID)
                {                                     //左侧不是ID
                    error_msg(6, exp->line_no, NULL); //报错
                    return -1;
                }
            }
            else
            {
                treeNode *tempnode13 = getchild(tempnode1, 2);
                if (tempnode13 != NULL)
                {
                    treeNode *tempnode14 = getchild(tempnode1, 3);
                    if (tempnode14 == NULL)
                    { // Exp DOT ID(结构体)
                        if (tempnode11->nodeType == N_EXP &&
                            tempnode12->nodeType == N_DOT &&
                            tempnode13->nodeType == N_ID)
                        {
                            ; //正确
                        }
                        else
                        { //报错
                            error_msg(6, exp->line_no, NULL);
                            return -1;
                        }
                    }
                    else
                    { // EXP LB EXP RB (数组)
                        if (tempnode11->nodeType == N_EXP &&
                            tempnode12->nodeType == N_LB &&
                            tempnode13->nodeType == N_EXP &&
                            tempnode14->nodeType == N_RB)
                        {
                            ; //正确
                        }
                        else
                        { //报错
                            error_msg(6, exp->line_no, NULL);
                            return -1;
                        }
                    }
                }
                else
                { // tempnode13==NULL 报错
                    error_msg(6, exp->line_no, NULL);
                    return -1;
                }
            }
        }
    }

    // ID，INT，FLOAT
    if (tempnode2 == NULL)
    {
        if (tempnode1->nodeType == N_ID)
        { //检查该ID是否已定义  (local & global) 只要是变量就算ID!
            if (!ifExistVarStack(var_domain_ptr, tempnode1->subtype.IDVal) &&
                !ifExistFunc(*fun_table, tempnode1->subtype.IDVal) &&
                !ifExistStruct(*struct_table, tempnode1->subtype.IDVal))
            {
                error_msg(1, exp->line_no, tempnode1->subtype.IDVal); //错误类型1，变量未定义
                return -1;
            }
            else
            {
                result = find_type(tempnode1); //找到了,返回这个ID代表的类型
                // result = charToInt(tempnode1->character, *struct_table);
                return result;
            }
        }
        else if (tempnode1->nodeType == N_INT)
        { //返回int
            result = D_INT;
            return result;
            ;
        }
        else if (tempnode1->nodeType == N_FLOAT)
        { //处理float
            result = D_FLOAT;
            return result;
        };
    }
    else
    {
        treeNode *tempnode3 = getchild(exp, 2);
        //第一部分;
        if (tempnode3 != NULL)
        {
            treeNode *tempnode4 = getchild(exp, 3);
            if (tempnode4 == NULL &&
                tempnode3->nodeType == N_EXP &&
                tempnode2->nodeType != N_LB)
            { // 3元且第三项是Exp，如Exp AND Exp
                treeNode *Expnode1 = tempnode1;
                treeNode *Expnode2 = tempnode3;
                if (Expnode1->nodeType != N_EXP)
                {
                    printf("It isn't Exp xx Exp.\n");
                }
                int exp1type = Exp_s(Expnode1);
                int exp2type = Exp_s(Expnode2);
                if (!check_error(exp1type, exp2type))
                {
                    //检查类型是否匹配
                    //赋值号
                    if (exp1type != exp2type && tempnode2->nodeType == N_ASSIGNOP)
                    {                                     
                        error_msg(5, exp->line_no, NULL); //错误类型5，赋值号两侧类型不匹配
                        return -1;
                    }

                    if(check_type(tempnode2, N_AND) ||
                       check_type(tempnode2, N_OR) ||
                       check_type(tempnode2, N_RELOP))
                    {//关系运算,返回INT
                        return D_INT;
                    }

                    //不是赋值号，为运算符
                    if (exp1type != exp2type)
                    {                                     
                        error_msg(7, exp->line_no, NULL); //错误类型7，操作数类型不匹配
                        return -1;
                    }
                    else
                    {//返回操作数(随便哪个exp)的类型
                        result = exp1type;
                        return result;
                    }
                }
                else
                {
                    //返回-1,子exp里面有错,把-1往前传
                    return -1;
                }
            }
        }
        //第二部分;
        if (tempnode1->nodeType == N_LP ||
            tempnode1->nodeType == N_MINUS ||
            tempnode1->nodeType == N_NOT)
        { // LP Exp RP，MINUS Exp，NOT Exp
            treeNode *expnode = tempnode2;
            if (expnode->nodeType != N_EXP)
            { //第二个不是exp
                printf("The second part should be Exp!\n");
            }
            int exp1type = Exp_s(expnode);
            if(check_type(expnode, N_NOT) && !check_error(exp1type, 1))
            {//NOT结果返回int
                return D_INT;
            }
            result = exp1type;
            return result;
        }
        //第三部分;
        /*
        | ID LP Args RP 有参函数
        | ID LP RP 无参函数

        | Exp LB Exp RB 数组
        | Exp DOT ID 结构体;
        */
        //函数部分：判断第一个是不是ID;需要检查这个函数的存在性,得到函数的params交给下一层检查,并且查看这个ID是不是函数类型
        if (tempnode1->nodeType == N_ID)
        { //当前为函数，需要去检查该函数是否已定义
            char *funcname = tempnode1->subtype.IDVal;
            int queryresult = ifExistFunc(*fun_table, funcname);        //在全局里面搜索;
            dataNodeFunc func_node = getNodeFunc(*fun_table, funcname); //搜素这个函数节点
            int ret_type = func_node.returnType;                        //获取函数返回类型

            if (!queryresult)
            { //没找到或者不是定义;  或者不是函数
                if (ifExistStruct(*struct_table, funcname) ||
                    ifExistVarStack(var_domain_ptr, funcname))
                {
                    //当前ID不是函数名
                    error_msg(11, exp->line_no, funcname); //错误类型11，对普通变量调用函数，例如i()；
                    return -1;
                }
                else
                {
                    error_msg(2, exp->line_no, funcname); //错误类型2，函数未定义
                    return -1;
                }
            }

            if (tempnode3->nodeType == N_ARGS)
            {
                if (func_node.args == NULL)
                {                                     //函数本身没有形参，但此时有实参
                    error_msg(9, exp->line_no, NULL); //错误类型9，函数实参形参不匹配
                    return -1;
                }
                else
                {
                    /*Args -> Exp COMMA Args
                    | Exp;
                    */
                    //检查args的数量;
                    int cnt = 0;
                    treeNode *cntnode = tempnode3;
                    while (1)
                    { //计算所有实参的数目
                        cnt += 1;
                        treeNode *tempcntnode = getchild(cntnode, 1);
                        if (tempcntnode == NULL)
                        {
                            break;
                        }
                        // cnt+=1;
                        cntnode = getchild(cntnode, 2);
                    }
                    if (cnt != getArgNum(*fun_table, funcname))
                    {
                        error_msg(9, exp->line_no, NULL); //错误类型9，函数实参形参个数不匹配
                        return -1;
                    }
                    int argresult = Arg_s(tempnode3, func_node.args);
                    if (argresult != 0)
                    {
                        return result;
                    }
                    else
                    {
                        return ret_type;
                    }
                }
            }
            else
            {

                if (func_node.args != NULL)
                {                                     //函数有形参，但此时没有实参
                    error_msg(9, exp->line_no, NULL); //错误类型9，函数实参形参个数不匹配
                    return -1;
                }
                else
                {
                    return ret_type;
                }
            }
        }
        else
        {
            treeNode *tempnode4 = getchild(exp, 3);

            if (tempnode4 == NULL)
            {
                //结构体部分Exp DOT ID----结构体存在----域名存在----返回这个域名的type
                if (tempnode1->nodeType == N_EXP &&
                    tempnode2->nodeType == N_DOT &&
                    tempnode3->nodeType == N_ID)
                {
                    int exptype = Exp_s(tempnode1);

                    if (!check_error(exptype, 1))//已保证结构体存在
                    {
                        dataNodeStruct stru_node = getNodeStruct(*struct_table, tempnode1->subtype.IDVal);
                        if (exptype < 6)
                        {   //当前Exp不是结构体
                            error_msg(13, exp->line_no, NULL); //错误类型13，对非结构体变量使用“.”
                            return -1;
                        }
                        else
                        {
                            //搜索域名;
                            if (ifExistStructDomain(*struct_table, tempnode3->nodeType, tempnode3->subtype.IDVal))
                            {
                                //找到了!
                                result = find_type(tempnode3);
                                return result;
                            }
                            else
                            {
                                //域名不存在;
                                error_msg(14, exp->line_no, tempnode3->subtype.IDVal); //错误类型14，该域没有在访问结构体中未定义
                                return -1;
                            }
                        };
                    }
                    else
                    {
                        return -1;//把错误往上传（已经报过错）
                    }
                }
            }
            else
            {
                ;
                //数组部分;| Exp LB Exp RB 数组
                if (tempnode1->nodeType == N_EXP &&
                    tempnode2->nodeType == N_LB &&
                    tempnode3->nodeType == N_EXP)
                {
                    int type1 = Exp_s(tempnode1);
                    int type3 = Exp_s(tempnode3);
                    if (check_error(type1, type3))
                    {
                        return -1;
                    }
                    if (type1 != 2) // type1不是数组
                    {
                        error_msg(10, exp->line_no, NULL); //错误类型10，对非数组变量进行数组访问
                        return -1;
                    }
                    else
                    {
                        if (type3 == D_INT)
                        { // Exp是整数
                            ;
                        }
                        else
                        {                                      // Exp不是整数
                            error_msg(12, exp->line_no, NULL); //错误类型12，数组访问符中出现非整数
                            return -1;
                        }
                    }
                    //返回元素的类型
                    dataNodeVar var_node = getNodeVar(var_domain_ptr->tVar, tempnode3->subtype.IDVal);
                    result = var_node.varType;
                    return result;
                }
            }
        };
    }
    return result;
}




/*int StmtList_s(treeNode *stmt, int d_type)
{
    
    StmtList ->
      Stmt StmtList
    | 空
    Stmt ->
      Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    
    treeNode *Stmtnode = getchild(stmt, 0);
    treeNode *tempnode = getchild(stmt, 1);
    Stmt_s(Stmtnode, d_type);
    if (tempnode != NULL)
    {
        StmtList_s(tempnode, d_type);
    }
}*/

//处理stmt，CompSt返回-4，RETURN Exp SEMI返回Exp类型值，其他返回-3
int Stmt_s(treeNode *stmt, int d_type)
{
    /*
    Stmt -> Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    */

    treeNode *tempnode1 = getchild(stmt, 0);
    if (tempnode1->nodeType == N_COMPST)
    {
        /*//新开一个作用域,进入CompSt，然后溜
        domainStack ds;
        domainPush(ds);
        comp_stmt(tempnode1, d_type);
        domainPop(ds);*/
        return -4;
    }
    else if (tempnode1->nodeType == N_EXP)
    {//Exp SEMI
        int uselesstype = Exp_s(tempnode1);
    }
    else if (tempnode1->nodeType == N_RETURN)
    {//RETURN Exp SEMI 返回Exp类型值
        treeNode *expnode = getchild(stmt, 1);
        if (expnode->nodeType != N_EXP)
        {
            printf("Stmt_s bug: should be Exp!\n");
        }
        int returntype = Exp_s(expnode);
        if (!check_error(returntype, 1))
        {
            if (d_type != returntype)
            {
                error_msg(8, stmt->line_no, NULL); //错误类型8，函数返回类型不匹配
            }
            else
            {
                ; // exp里面已经因为NULL报错
            }
        }
        return returntype;
    }
    else if (tempnode1->nodeType == N_WHILE)
    {//WHILE LP Exp RP Stmt 返回-3
        treeNode *expnode = getchild(stmt, 2);
        treeNode *stmtnode = getchild(stmt, 4);
        int type = Exp_s(expnode);
        if (!check_error(type, 1))
        {
            if (type == 0)
            {
                ;
            }
            else
            {
                error_msg(7, stmt->line_no, NULL); //错误类型7，while条件操作数类型不匹配
            }
        }
        else
        {
            ; // Exp里面已经报过了
        }
        Stmt_s(stmtnode, d_type);
    }
    else if (tempnode1->nodeType == N_IF)
    {
        /*	| IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt  返回-3
    */
        treeNode *expnode = getchild(stmt, 2);
        if (expnode->nodeType != N_EXP)
        {
            printf("Stmt_s bug: should be Exp!\n");
        }
        treeNode *tempnode6 = getchild(stmt, 5); // ELSE
        int iftype = Exp_s(expnode);
        if (!check_error(iftype, 1))
        {
            if (iftype == 0)
            {
                ;
            }
            else
            {
                error_msg(7, stmt->line_no, NULL); //错误类型7，if条件操作数类型不匹配
                                                    return -1;
            }
        }
        if (tempnode6 == NULL)
        {
            treeNode *stmtnode1 = getchild(stmt, 4);

            Stmt_s(stmtnode1, d_type);
        }
        else
        {
            treeNode *stmtnode1 = getchild(stmt, 4);
            treeNode *stmtnode2 = getchild(stmt, 6);

            Stmt_s(stmtnode1, d_type);

            Stmt_s(stmtnode2, d_type);
        };
    }
    else
    {
        printf("Stmt_s error: Impossible to be here!\n");
    }
    return -3;
}





// int ext_def(treeNode *ExtDef, seqStack *stack, stackNode *domain)
// {

//     treeNode *type_node = ExtDef->child->child;
//     treeNode *core_node = ExtDef->child->sibling;
//     int def_type = specifier(type_node);

//     switch (def_type)
//     {
//     case D_STRUCT_DEC:
//         def_type = charToInt(type_node->child->sibling->child->subtype.IDVal, *struct_table);
//     case D_INT:
//     case D_FLOAT:
//         if (core_node->nodeType == N_EXT_DEC_L)
//         {
//             treeNode *temp_node;
//             do
//             {
//                 temp_node = core_node->child;
//                 core_node = temp_node->sibling->sibling;
//                 InsertVar(&(domain->tVar), var_dec(temp_node, def_type));
//             } while (core_node != NULL);
//         }
//         else
//         {
//             dataNodeFunc *abc = fun_dec(core_node, type_node->child->character);
//             if (core_node->sibling->nodeType != N_SEMI)
//             {
//                 abc->defined = 1;
//                 //返回类型不匹配在这里面报错
//                 comp_stmt(core_node->sibling, def_type);
//             }

//             InsertFunc(fun_table, abc);
//             free(abc);
//         }

//         break;
//     case D_STRUCT_DEF:
//         struct_specifier_def(type_node);
//         break;
//     default:
//         break;
//     }

//     return 0;
// }

int tree_analys(treeNode *mytree)
{
    //栈初始化部分
    treeNode *temp;
    seqStack myStack;
    seqStack *stack_ptr;
    stack_ptr = &myStack;
    initStack(stack_ptr);
    push(stack_ptr, mytree);

    //表初始化部分
    var_domain_ptr = createStackNode();
    tableFuncInit(fun_table);
    tableStructInit(struct_table);

    //用于存储变量信息
    int if_unfold = 1;
    int type_now = -1;
    // int in_local = 0;
    // int in_struct_def = 0;
    int exp_stmt_out = 0;
    char* temp_ID = NULL;
    dataNodeVar *var_ptr = NULL;
    dataNodeFunc *func_ptr = NULL;
    dataNodeStruct *struct_ptr = NULL;

    do
    {
        if (if_unfold)
        {
            reversed_insert(stack_ptr, pop(stack_ptr));
        }
        temp = top(stack_ptr);
        //根据收到的不同符号调用不同的处理函数
        switch (temp->nodeType)
        {
        //这里涉及的一系列节点都是不需要做特殊处理的，接着pop就好
        case N_PROGRAM:
            printf("Program detected\n");
            if_unfold = 1;
            break;
        
        case N_EXT_DEF_L:
            printf("ExtDefList detected\n");
            if_unfold = 1;
            break;
        case N_EXT_DEF:
            printf("ExtDef detected\n");
            if_unfold = 1;
            break;

        // specifier处理部分
        // specifier总是被期待返回一个类型值，所以在栈上统一解开（省点内存吧球球了）
        case N_SPECI:
            printf("Specifier detected, now finding the type...\n");
            if_unfold = 1;
            break;
        case N_TYPE:
            printf("Normal type in the Specifier\n");
            pop(stack_ptr);
            if_unfold = 0;
            if (temp->child->subtype.IDVal[0] == 'i')
            {
                type_now = D_INT;
                break;
            }
            type_now = D_FLOAT;
            break;
        case N_STRUCT_SPECI:
            printf("Structure type in the specifier\n");
            if (temp->child->child->sibling->nodeType == N_TAG)
            {
                type_now = struct_specifier_dec(temp);
                pop(stack_ptr);
                if_unfold = 0;
                break;
            }
            // type_now = struct_specifier_def(temp);
            if_unfold = 1;
            break;

        case N_STRUCT:
            printf("Struct definition detected\n");
            if_unfold = 0;
            pop(stack_ptr);
            break;

        case N_OPT_TAG:
            if_unfold = 0;
            pop(stack_ptr);
            struct_ptr = newNodeStruct(temp -> child -> subtype.IDVal);
            break;


        case N_EXT_DEC_L:
            printf("ExtDecList detected.\n");
            if_unfold = 1;
            break;

        case N_VAR_DEC:
            var_ptr = var_dec(temp, type_now);
            if(struct_ptr != NULL){
                insertStructDomain(struct_ptr, var_ptr);
            }else{
                InsertVar(&(var_domain_ptr->tVar), var_ptr);
                free_var(var_ptr);
            }
            var_ptr = NULL;
            pop(stack_ptr);
            if_unfold = 0;
            break;

        case N_SEMI:
        case N_COMMA:
            if (func_ptr != NULL)
            {
                InsertFunc(fun_table, func_ptr);
                free_func(func_ptr);
                func_ptr = NULL;
            }
            // if(var_dec != NULL){

            // }
            pop(stack_ptr);
            if_unfold = 0;
            break;

        case N_FUN_DEC:
            //注意！这里还不知道函数有没有定义，只知道有这样一个声明
            //这个声明暂存后，并没有提交到表中
            printf("Function declareration detected\n");
            func_ptr = fun_dec(temp, type_now);
            pop(stack_ptr);
            if_unfold = 0;
            break;

        case N_COMPST:
            printf("Compst detected");
            if_unfold = 1;
            break;

        case N_LC:
            if(struct_ptr != NULL){
                break;
            }
            printf("Create new domain\n");
            domainPush(var_domain_ptr);
            // in_local++;
            if_unfold = 0;
            pop(stack_ptr);
            break;

        case N_DEF_L:
            printf("DefList detected\n");
            if_unfold = 1;
            break;
        case N_DEF:
            printf("Def detected\n");
            if_unfold = 1;
            break;
        case N_DEC_L:
            printf("DecList detected\n");
            if_unfold = 1;
            break;
        case N_DEC:
            if_unfold = 1;
            break;
        case N_ASSIGNOP:
            if_unfold = 0;
            pop(stack_ptr);
            break;
        
        case N_EXP:
            exp_stmt_out = Exp_s(temp);
            if(type_now != exp_stmt_out && exp_stmt_out != -1){
                error_msg(5,temp->line_no, temp->subtype.IDVal);
            }
            if_unfold = 0;
            pop(stack_ptr);
            break;

        case N_STMT_L:
            printf("StmtList detected\n ");
            if_unfold = 1;
            break;

        case N_STMT:
            exp_stmt_out = Stmt_s(temp, type_now);
            switch (exp_stmt_out)
            {
            case -4:
                if_unfold = 1;
                break;
            case -3:
                pop(stack_ptr);
                if_unfold = 0;
                break;
            case -2:
                error_msg(8, temp->line_no, NULL);
                pop(stack_ptr);
                if_unfold = 0;
                break;
            case -1:
                printf("ERROR in the return exp.\n");
                pop(stack_ptr);
                if_unfold = 0;
                break;
            
            default:
                if(type_now != exp_stmt_out){
                    error_msg(8, temp->line_no, NULL);
                }
                pop(stack_ptr);
                if_unfold = 0;
                break;
            }
            break;
        case N_RC:
            if(struct_ptr != NULL){
                InsertStruct(struct_table, struct_ptr);
                free_struct(struct_ptr);
                struct_ptr == NULL;
                if_unfold = 0;
                pop(stack_ptr);
                break;
            }
            printf("Close the domain\n");
            domainPop(var_domain_ptr);
            // in_local--;
            if_unfold = 0;
            pop(stack_ptr);
            break;

        default:
            // 这里给出一个列表：
            // ExtDecList
            // 这些非终结符，理论上应该在函数中被处理掉
            // 但是既然走到了这一步，显然没有，所以这里肯定要报错
            printf("ERROR: Unexpected node token with character %s\n", temp->character);
            // pop(stack_ptr);
            if_unfold = 1;
            break;
        }
        //节点处理完了，下一个

    } while (!isEmpty(stack_ptr));

    return 0;
}
