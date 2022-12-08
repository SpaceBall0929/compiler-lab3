#include <stdio.h>
#include "DomainStack.c"
#include "Ir.c"
#include "exp.c"
#define EXP_DO_NOTHING 114514
#define EXP_RETURN 114515
#define EXP_BRANCH 114516
#define EXP_LOOP 114517
#define EXP_INIT_VAR 114518
#define IN_GLOBAL 1919810
#define IN_FUNC_DEC 1919811
#define IN_FUNC_COMPST 1919812
#define IN_STRUCT_DEC_L 1919813
#define IN_VAR_DEC 1919814

//改成任意不为0的数字开启debug输出
#define IF_DEBUG_PRINT 0

// 为了生成一系列中间变量，这里需要一个记录体来简单记录一下都有哪些变量
//每一个字母下属变量一共有几个
int num_of_vars = 0;
//变量从a开始
int letter_now = 97;
char *var_name_gen()
{
    char *out = (char *)malloc(sizeof(char) * 3);
    out[0] = letter_now;
    out[1] = num_of_vars + 48;
    out[2] = '\0';
    num_of_vars++;
    if (num_of_vars == 10)
    {
        letter_now += 1;
        num_of_vars = 0;
    }

    return out;
}

//为了给标签计数
int lable_num0 = 0;
int lable_num1 = 0;

//插入标签的简便方式,返回值为标签编号，标签名格式为lb+编号
char* insert_lable(){
    char* lable_name = (char*)malloc(sizeof(char)* 5);
    lable_name[0] = 'l';
    lable_name[1] = 'b';
    lable_name[2] = lable_num0 + 48;
    lable_name[3] = lable_num1 + 48;
    lable_name[4] = '\0';
    lable_num1++;
    if(lable_num1 == 10){
        lable_num0++;
        lable_num1 = 0;
    }
    operand* myop = init_operand(VARIABLE, lable_name, 0, 0);
    new_op(lst_of_ir, I_LABLE, myop, 0);

    return lable_name;
}

//为了处理if语句，这里需要一个记录stmt的地方哦
treeNode *stmt_quene[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
//这个用来记录if对应语句块在哪里
int goto_idx[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
char* all_lable_names[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
//记录stmt的总个数
int stmt_cnt = 0;

// 识别非终结符VarDec,收集变量的名字，收集是否是数组，返回一个初始化好的dataNodeVar

// 判定specifier的指向：整形/浮点/结构体定义/结构体使用
int specifier(treeNode * speci)
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
        if (IF_DEBUG_PRINT)
        {
            printf("ERROR: Unexpected nodeType in the child of Specifier\n");
        }
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
        dimensionlen[dimension++] = dec_node->sibling->sibling->subtype.intVal;
        dec_node = dec_node->child;
    }

    //这里直接给变量换名字了，方便中间代码使用
    new_var = newNodeVar(dec_node->subtype.IDVal, var_name_gen(), var_type);
    if (dimension == 0)
    {
        return new_var;
    }
    new_var->arrayVarType = var_type;
    new_var->varType = D_ARRAY;
    new_var->numdim = dimension;
    new_var->len_of_dims = (int *)malloc(sizeof(int) * dimension);
    for (int i = 0; i < dimension; i++)
    {
        new_var->len_of_dims[i] = dimensionlen[i];
    }
    return new_var;
}

dataNodeVar *param_dec(treeNode *para)
{
    int type_def = specifier(para->child);
    if (type_def == D_STRUCT_DEF)
    {
        if (IF_DEBUG_PRINT)
        {
            printf("ERROR: Unexpected treeNode: structure definition in the parameters of the function.\n");
        }
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

//返回对应类型的宏
int find_type(treeNode *n)
{
    char *name = n->subtype.IDVal;
    if (ifExistStruct(*struct_table, name))
    {
        if (IF_DEBUG_PRINT)
            printf("Struct\n");
        return charToInt(name, *struct_table);
    }
    else if (ifExistFunc(*fun_table, name))
    {
        return 5;
    }
    else
    {
        if (IF_DEBUG_PRINT)
            printf("Input name is %s\n", n->subtype.IDVal);
        if (IF_DEBUG_PRINT)
            printf("After searching, the var name is %s\n var type is %d \n", getNodeVar(var_domain_ptr->tVar, name).varName,
                   getNodeVar(var_domain_ptr->tVar, name).varType);
        return getNodeVar(var_domain_ptr->tVar, name).varType;
    }
}

//返回这个type的字符串
char *find_type2(int t, char *name)
{
    char *c = "Sruct";
    switch (t)
    {
    case 0:
        return "int";
        break;
    case 1:
        return "float";
        break;
    case 2:
        return "array";
        break;
    case 5:
        return "function";
        break;
    default:
        // strcat(c, (char)(getNodeVar(var_domain_ptr->tVar, name).varType - 5));
        c[5] = (char)(getNodeVar(var_domain_ptr->tVar, name).varType - 5);
        break;
    }
    return c;
}

//检查类型是否可以赋值
int right_type(int t1, int t2)
{
    if (t1 >= 6 && t2 >= 6)
    { //看看是否是体相同结构体
        int i = ifStructEquivalent(*struct_table, t1, t2);
        if (IF_DEBUG_PRINT)
        {
            printf("比较结构体结果是%d", i);
        }
        return i;
    }
    else
    {
        return t1 == t2;
    }
}



//处理Arg_s，判断实参和形参类型是否匹配（数量是否匹配已在exp中判断过）
int Arg_s(treeNode *args, dataNodeVar *params, char *name)
{
    /*Args ->
      Exp COMMA Args
    | Exp;
    */
    treeNode *expnode = getchild(args, 0);
    treeNode *tempnode = getchild(args, 1); //判断实参是否还有参数

    int temptype = Exp_s(expnode); //检查函数形参和实参类型是否匹配

    if (temptype != params->varType)
    {
        return -1;
    }

    if (tempnode != NULL)
    { //实参还有参数，继续检查下一个实参和形参是否匹配
        treeNode *argsnode = getchild(args, 2);
        return Arg_s(argsnode, params->next, name);
    }
    return 0;
}

//返回是否报错，若只有一个参数，第二个参数写1
int check_error(int a, int b)
{
    if (a == -1 || b == -1)
    {
        return 1;
    }
    return 0;
}





int tree_analys(treeNode *mytree)
{
    //栈初始化部分
    treeNode *temp = mytree;
    seqStack myStack;
    seqStack *stack_ptr;
    operand *operand_to_use;
    stack_ptr = &myStack;
    initStack(stack_ptr);
    push(stack_ptr, mytree);
    if (IF_DEBUG_PRINT)
    {
        printf("Initializing stack successfully\n");
    }
    //表初始化部分
    var_domain_ptr = domainPush(var_domain_ptr);
    fun_table = tableFuncInit();
    struct_table = tableStructInit();
    lst_of_ir = init_IR();
    if (IF_DEBUG_PRINT)
    {
        printf("Initializing tables successfully\n");
    }
    //用于存储变量信息以及一些flag
    int if_unfold = 1;
    // int in_func_domain = 0;
    int exp_flag = EXP_DO_NOTHING;
    int now_processing = IN_GLOBAL;
    int nearest_speci_type = -1;
    // int nearest_struct_type = -1;
    int nearestfunc_type = -1;
    // int in_local = 0;
    // int in_struct_def = 0;
    int exp_stmt_out = 0;
    char *temp_ID = NULL;
    dataNodeVar *var_head = NULL;
    dataNodeVar *var_ptr = NULL;
    dataNodeFunc *func_ptr = NULL;
    dataNodeStruct *struct_ptr = NULL;
    if (IF_DEBUG_PRINT)
    {
        printf("Initializing varibles successfully\n");
    }

    //这个地方需要提前的插入read()和write()两个函数，后面会有调用的。
    //或者说exp那一层直接给我解决了？我不知道，问你刘爹

    while (!isEmpty(stack_ptr))
    {
        //根据收到的不同符号调用不同的处理函数
        switch (temp->nodeType)
        {
        case N_PROGRAM:
            if (IF_DEBUG_PRINT)
            {
                printf("Program detected\n");
            }
            if_unfold = 1;
            break;

        case N_EXT_DEF_L:
            if (IF_DEBUG_PRINT)
            {
                printf("ExtDefList detected\n");
            }
            if (temp->child == NULL)
            {
                now_processing = IN_GLOBAL;
                if (func_ptr != NULL)
                {
                    free_func(func_ptr);
                    func_ptr = NULL;
                    if (IF_DEBUG_PRINT)
                    {
                        printf("----------------------a function ended-----------------------------\n");
                    }
                }
            }
            if_unfold = 1;
            break;
        case N_EXT_DEF:
            if (IF_DEBUG_PRINT)
            {
                printf("ExtDef detected\n");
            }
            now_processing = IN_GLOBAL;
            if (func_ptr != NULL)
            {
                free_func(func_ptr);
                func_ptr = NULL;
                if (IF_DEBUG_PRINT)
                {
                    printf("----------------------a function ended-----------------------------\n");
                }
            }
            if_unfold = 1;
            break;

        // specifier处理部分
        // specifier总是被期待返回一个类型值，所以在栈上统一解开（省点内存吧球球了）
        case N_SPECI:
            if (IF_DEBUG_PRINT)
            {
                printf("Specifier detected, now finding the type...\n");
            }
            if_unfold = 1;
            break;
        case N_TYPE:
            if (IF_DEBUG_PRINT)
            {
                printf("Normal type in the Specifier\n");
            }
            if_unfold = 0;
            if (temp->subtype.IDVal[0] == 'i')
            {
                nearest_speci_type = D_INT;
                break;
            }
            nearest_speci_type = D_FLOAT;
            break;

        case N_STRUCT_SPECI:
            if (IF_DEBUG_PRINT)
            {
                printf("Structure type in the specifier\n");
            }
            if (temp->child->sibling->nodeType == N_TAG)
            {
                nearest_speci_type = struct_specifier_dec(temp);
                now_processing = IN_VAR_DEC;
                if_unfold = 0;
                break;
            }
            // nearest_speci_type = struct_specifier_def(temp);
            now_processing = IN_STRUCT_DEC_L;
            if_unfold = 1;
            break;

        case N_STRUCT:
            if_unfold = 0;
            break;

        case N_OPT_TAG:
            if (IF_DEBUG_PRINT)
            {
                printf("Struct definition detected\n");
            }
            if_unfold = 0;
            struct_ptr = newNodeStruct(temp->child->subtype.IDVal);
            nearest_speci_type = InsertStruct(struct_table, struct_ptr, temp->line_no);
            break;

        case N_EXT_DEC_L:
            if (IF_DEBUG_PRINT)
            {
                printf("ExtDecList detected.\n");
            }
            if_unfold = 1;
            break;

        case N_VAR_DEC:

            // IN_DEC
            if (IF_DEBUG_PRINT)
            {
                printf("VarDec detected, processing with the outer functions...\n");
            }
            if_unfold = 0;
            if (var_head == NULL)
            {
                var_head = var_dec(temp, nearest_speci_type);
                var_ptr = var_head;
                break;
            }
            var_ptr->next = var_dec(temp, nearest_speci_type);
            var_ptr = var_ptr->next;
            // if(IF_DEBUG_PRINT){printf("VarDec processed successfully\n");}
            break;

        case N_SEMI:
            if (IF_DEBUG_PRINT)
            {
                printf("SEMI detected\n");
            }
            // SEMI在这个层次被扫描有两重功能，一个是结束变量。结束结构体
            //一个是结束函数的声明
            switch (now_processing)
            {
            case IN_FUNC_DEC:
                InsertFunc(&(var_domain_ptr->tVar), fun_table, func_ptr, temp->line_no);
                free_func(func_ptr);
                func_ptr = NULL;
                if (IF_DEBUG_PRINT)
                {
                    printf("----------------------a function ended-----------------------------\n");
                }
                now_processing = IN_GLOBAL;
                break;

            case IN_VAR_DEC:
            case IN_FUNC_COMPST:
                if (var_head != NULL)
                {
                    var_ptr = var_head;
                    do
                    {
                        InsertVar(&(var_domain_ptr->tVar), fun_table, var_ptr, temp->line_no);
                        var_ptr = var_ptr->next;
                    } while (var_ptr != NULL);
                    free_var(var_head);
                    var_head = NULL;
                }
                break;

            case IN_STRUCT_DEC_L:
                var_ptr = var_head;
                do
                {
                    insertStructDomain(getNodeStruct(*struct_table, struct_ptr->structTypeName), var_ptr, *struct_table, temp->line_no);
                    var_ptr = var_ptr->next;
                } while (var_ptr != NULL);
                free_var(var_head);
                var_head = NULL;
                var_ptr = NULL;
                // InsertStruct(struct_table, struct_ptr);
                // now_processing = IN_GLOBAL;
                break;

            default:
                break;
            }
            if_unfold = 0;
            break;

        case N_COMMA:
            if_unfold = 0;
            break;

        case N_FUN_DEC:
            //注意！这里还不知道函数有没有定义，只知道有这样一个声明
            //这个声明暂存后，并没有提交到表中
            if (IF_DEBUG_PRINT)
            {
                printf("Function declareration detected\n");
            }
            func_ptr = fun_dec(temp, nearest_speci_type);

            //函数初始化之写标签
            operand_to_use = init_operand(VARIABLE, func_ptr->funcName, 0, 0);
            new_op(lst_of_ir, I_LABLE, operand_to_use, 1);

            nearestfunc_type = nearest_speci_type;
            now_processing = IN_FUNC_DEC;
            if_unfold = 0;
            break;

        case N_COMPST:
            if (IF_DEBUG_PRINT)
            {
                printf("Compst detected\n");
            }
            if_unfold = 1;
            break;

        case N_LC:
            if (now_processing == IN_FUNC_DEC)
            {
                now_processing = IN_FUNC_COMPST;
                func_ptr->defined = 1;
                InsertFunc(&(var_domain_ptr->tVar), fun_table, func_ptr, temp->line_no);
            }
            if (now_processing == IN_FUNC_COMPST)
            {
                if (IF_DEBUG_PRINT)
                {
                    printf("Create new domain\n");
                }
                var_domain_ptr = domainPush(var_domain_ptr);
                dataNodeVar *arg_of_func = func_ptr->args;

                //根据师兄建议，增加了形参段
                //这个部分可以节约exp的工作量
                while (arg_of_func != NULL)
                {
                    InsertVar(&(var_domain_ptr->tVar), fun_table, arg_of_func, temp->line_no);
                    arg_of_func = arg_of_func->next;
                    operand_to_use = init_operand(VARIABLE, arg_of_func->ir_name, 0, 0);
                    new_op(lst_of_ir, I_PARAM, operand_to_use, 1);
                }
            }
            if_unfold = 0;
            break;

        case N_DEF_L:
            if (IF_DEBUG_PRINT)
            {
                printf("DefList detected\n");
            }
            if_unfold = 1;
            break;
        case N_DEF:
            if (IF_DEBUG_PRINT)
            {
                printf("Def detected\n");
            }
            if_unfold = 1;
            break;
        case N_DEC_L:
            if (IF_DEBUG_PRINT)
            {
                printf("DecList detected\n");
            }
            if_unfold = 1;
            break;
        case N_DEC:
            if (IF_DEBUG_PRINT)
            {
                printf("Dec detected\n");
            }
            if_unfold = 1;
            break;
        case N_ASSIGNOP:
            exp_flag = EXP_INIT_VAR;
            if_unfold = 0;
            break;

        case N_EXP:
            if (IF_DEBUG_PRINT)
            {
                printf("EXP detected, finding the usage of expression...\n");
            }
            switch (exp_flag)
            {
            case EXP_DO_NOTHING:
                if (IF_DEBUG_PRINT)
                {
                    printf("EXP_DO_NOTHING\n");
                }
                Exp_s(temp);
                if (IF_DEBUG_PRINT)
                {
                    printf("EXP_DONE\n");
                }
                break;

            case EXP_LOOP:
                all_lable_names[2] = insert_lable();
                goto_idx[stmt_cnt++] = Exp_o(temp);
                operand_to_use = init_operand(VARIABLE, NULL, 0, 0);
                new_op(lst_of_ir, I_GOTO, operand_to_use, 1);
                goto_idx[stmt_cnt++] = lst_of_ir->length - 1;
                all_lable_names[0] = insert_lable();
                if (IF_DEBUG_PRINT)
                {
                    printf("EXP_LOOP\n");
                }
                break;

            case EXP_BRANCH:
                if (IF_DEBUG_PRINT)
                {
                    printf("EXP_LOOP\n");
                }
                break;

            case EXP_INIT_VAR:
                if (IF_DEBUG_PRINT)
                {
                    printf("EXP_INIT_VAR\n");
                }
                if (Exp_s(temp) != nearest_speci_type)
                {
                    if (IF_DEBUG_PRINT)
                    {
                        printf("EXP_DONE\n");
                    }
                    error_msg(5, temp->line_no, NULL);
                }
                break;
            case EXP_RETURN:
                if (IF_DEBUG_PRINT)
                {
                    printf("EXP_RETURN\n");
                }
                if (Exp_s(temp) != nearestfunc_type)
                {
                    if (IF_DEBUG_PRINT)
                    {
                        printf("EXP_DONE\n");
                    }
                    error_msg(8, temp->line_no, NULL);
                }
                break;

            default:
                if (IF_DEBUG_PRINT)
                {
                    printf("BAD ERROR:the totally unexpected type of the exp_flag!!!!\n");
                }
                break;
            }

            exp_flag = EXP_DO_NOTHING;
            if_unfold = 0;
            break;

        case N_STMT_L:
            if (IF_DEBUG_PRINT)
            {
                printf("StmtList detected\n ");
            }
            if_unfold = 1;
            break;

        case N_STMT:
            if(){
                
            }


            
            if (IF_DEBUG_PRINT)
            {
                printf("stmt detected\n");
            }
            

            if_unfold = 1;
            break;

        case N_RETURN:
            if (IF_DEBUG_PRINT)
            {
                printf("RETURN detected\n");
            }
            exp_flag = EXP_RETURN;
            if_unfold = 0;
            break;
        case N_IF:
            if (IF_DEBUG_PRINT)
            {
                printf("IF detected\n");
            }
            exp_flag = EXP_BRANCH;
            if_unfold = 0;
            break;

        case N_WHILE:
            if (IF_DEBUG_PRINT)
            {
                printf("WHILE detected\n");
            }
            exp_flag = EXP_LOOP;
            if_unfold = 0;
            break;

        case N_LP:
        case N_RP:
        case N_ELSE:
            if_unfold = 0;
            break;

        case N_RC:
            if (now_processing == IN_FUNC_COMPST)
            {
                if (IF_DEBUG_PRINT)
                {
                    printf("Close the domain\n");
                }
                var_domain_ptr = domainPop(var_domain_ptr);
            }
            if (now_processing == IN_STRUCT_DEC_L)
            {
                now_processing = IN_VAR_DEC;
                nearest_speci_type = charToInt(struct_ptr->structTypeName, *struct_table);
                free_struct(struct_ptr);
                struct_ptr = NULL;
            }
            if_unfold = 0;
            break;

        case N_ELSE_L:
            if (IF_DEBUG_PRINT)
            {
                printf("ElseList detected\n");
            }
            if_unfold = 1;
            break;

        default:
            // 这些非终结符，理论上应该在函数中被处理掉
            // 但是既然走到了这一步，显然没有，所以这里肯定要报错
            if (IF_DEBUG_PRINT)
            {
                printf("ERROR: Unexpected node token with character %s\n", temp->character);
            }
            // pop(stack_ptr);
            if_unfold = 1;
            break;
        }

        //节点处理完了，下一个

        pop(stack_ptr);
        if (if_unfold)
        {
            reversed_insert(stack_ptr, temp);
        }

        temp = top(stack_ptr);
    }

    return 0;
}
