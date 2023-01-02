#include <stdio.h>
#include "exp.c"
#include "syntax_treenode_analyz_dcs.c"
#include "ir_gen_dcs.c"
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

// 改成任意不为0的数字开启debug输出
#define IF_DEBUG_PRINT 0

// 为了生成一系列中间变量，这里需要一个记录体来简单记录一下都有哪些变量
// 每一个字母下属变量一共有几个

// 识别非终结符VarDec,收集变量的名字，收集是否是数组，返回一个初始化好的dataNodeVar

int tree_analys(treeNode *mytree)
{
    // 栈初始化部分
    treeNode *temp = mytree;
    seqStack myStack;
    seqStack *stack_ptr;
    operand_list *operand_to_use = init_operand_list();
    stack_ptr = &myStack;
    initStack(stack_ptr);
    push(stack_ptr, mytree);
    if (IF_DEBUG_PRINT)
    {
        printf("Initializing stack successfully\n");
    }
    // 表初始化部分
    var_domain_ptr = domainPush(var_domain_ptr);
    fun_table = tableFuncInit();
    struct_table = tableStructInit();
    lst_of_ir = init_IR();
    if (IF_DEBUG_PRINT)
    {
        printf("Initializing tables successfully\n");
    }
    // 用于存储变量信息以及一些flag
    int if_unfold = 1;
    int exp_flag = EXP_DO_NOTHING;
    int now_processing = IN_GLOBAL;
    int nearest_speci_type = -1;
    int nearestfunc_type = -1;
    int exp_stmt_out = 0;
    char *temp_ir_lable = NULL;
    dataNodeVar *var_head = NULL;
    dataNodeVar *var_ptr = NULL;
    dataNodeFunc *func_ptr = NULL;
    dataNodeStruct *struct_ptr = NULL;
    if (IF_DEBUG_PRINT)
    {
        printf("Initializing varibles successfully\n");
    }

    // 中间代码生成用到的，用于处理if-else及其嵌套结构
    if_stack *if_stmts_lables;
    if_stmts_lables = (if_stack *)malloc(sizeof(if_stack));
    if_stmts_lables->len = 0;
    for (int i = 0; i < IF_BLOCK_NUM; i++)
    {
        if_stmts_lables->quene[i].node_cnt = 0;
        if_stmts_lables->quene[i].name_cnt = 0;
        if_stmts_lables->quene[i].end_lable = NULL;
    }

    // 记录现在是否属于一串if中，用于判断if_stack是否压栈 flag0
    int in_set_of_if = 0;
    // 记录栈到什么位置才会完全解析else list，方便中间代码的书写 flag1
    int when_to_end_if = -1;
    // 扫描到else的特殊记号，这里是专门用来讨论else的 flag2
    int last_token_is_else = 0;
    // 这里记录还有几个if下属的stmt未处理，数着栈长度来计算 flag3
    int if_stmt_remain = 0;
    // 这里记录处理的到底是if还是while
    int while_flag = -1;
    // 这个地方需要提前的插入read()和write()两个函数，后面会有调用的。
    // 或者说exp那一层直接给我解决了？我不知道，问你刘爹

    while (!isEmpty(stack_ptr))
    {
        // 根据收到的不同符号调用不同的处理函数
        switch (temp->nodeType)
        {
        case N_PROGRAM:
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
            // 一个是结束函数的声明
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
            // 注意！这里还不知道函数有没有定义，只知道有这样一个声明
            // 这个声明暂存后，并没有提交到表中
            if (IF_DEBUG_PRINT)
            {
                printf("Function declareration detected\n");
            }
            func_ptr = fun_dec(temp, nearest_speci_type);
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
                // 函数初始化之写标签
                dataNodeVar *arg_of_func = func_ptr->args;
                insert_func(func_ptr->funcName);
                // 根据师兄建议，增加了形参段
                // 这个部分可以节约exp的工作量
                while (arg_of_func != NULL)
                {
                    InsertVar(&(var_domain_ptr->tVar), fun_table, arg_of_func, temp->line_no);
                    new_operand(operand_to_use, VARIABLE, arg_of_func->ir_name, 0, 0);
                    new_op(lst_of_ir, I_PARAM, *operand_to_use);
                    del_operand_content(operand_to_use);
                    arg_of_func = arg_of_func->next;
                }
            }
            if (now_processing == IN_FUNC_COMPST)
            {
                if (IF_DEBUG_PRINT)
                {
                    printf("Create new domain\n");
                }
                var_domain_ptr = domainPush(var_domain_ptr);
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
                // 清除一下最后一个返回值占用的内存
                IR_list* xxx = lst_of_ir;
                free(Exp_s(temp));
                xxx = lst_of_ir;
                break;

            case EXP_LOOP:
                // while 的前置标签
                temp_ir_lable = gen_lable();
                new_operand(operand_to_use, VARIABLE, temp_ir_lable, 0, 0);
                new_op(lst_of_ir, I_LABLE, *operand_to_use);
                del_operand_content(operand_to_use);
                add_lable(if_stmts_lables, temp_ir_lable);

                // while的IF相关语句
                temp_ir_lable = gen_lable();
                Exp_o(temp, temp_ir_lable);
                add_lable(if_stmts_lables, temp_ir_lable);
                pop(stack_ptr);
                pop(stack_ptr);
                add_stmt(if_stmts_lables, top(stack_ptr));

                // 设置一下相关的控制信号
                // while语句固定只有一个stmt，所以这里可以直接跳到步骤2
                while_flag = 1;
                // 这两个数值设置直接引起stmt全部push入栈
                in_set_of_if = 2;
                when_to_end_if = stack_ptr->top - 1;
                // GOTO跳出循环的位置，这个必须写
                last_token_is_else = 0;

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
                temp_ir_lable = gen_lable();
                Exp_o(temp, temp_ir_lable);
                add_lable(if_stmts_lables, temp_ir_lable);
                pop(stack_ptr);
                pop(stack_ptr);
                add_stmt(if_stmts_lables, top(stack_ptr));
                break;

            case EXP_INIT_VAR:
                new_operand(operand_to_use, VARIABLE, var_ptr->ir_name, 0, 0);
                add_operand(operand_to_use, Exp_s(temp));
                new_op(lst_of_ir, I_ASSIGN, *operand_to_use);
                del_operand_content(operand_to_use);
                break;
            case EXP_RETURN:
                if (IF_DEBUG_PRINT)
                {
                    printf("EXP_RETURN\n");
                }
                add_operand(operand_to_use, Exp_s(temp));
                new_op(lst_of_ir, I_RETURN, *operand_to_use);
                del_operand_content(operand_to_use);
                break;

            default:
                if (IF_DEBUG_PRINT)
                {
                    printf("BAD ERROR:the totally unexpected type of the exp_flag!!!!\n");
                }
                break;
            }
            // if(exp_flag == EXP_BRANCH){
            // exp_flag = EXP_DO_NOTHING;
            // if_unfold = 1;
            // break;
            // }

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

            // 遇到了else句子
            if (last_token_is_else)
            {
                // last_token_is_else = 0;
                if (temp->child->nodeType == N_IF)
                {
                    if_unfold = 1;
                    if (in_set_of_if == 1)
                    {
                        in_set_of_if++;
                        when_to_end_if = stack_ptr->top - 1;
                    }
                    break;
                }
                else
                {
                    temp_ir_lable = gen_lable();
                    add_lable(if_stmts_lables, temp_ir_lable);
                    add_stmt(if_stmts_lables, temp);
                    new_operand(operand_to_use, VARIABLE, temp_ir_lable, 0, 0);
                    new_op(lst_of_ir, I_GOTO, *operand_to_use);
                    del_operand_content(operand_to_use);
                    in_set_of_if++;
                    when_to_end_if = stack_ptr->top - 1;
                    if_unfold = 0;
                }
                // 这些是确定了这个句子和else if没关系才做的事

                break;
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
            if (in_set_of_if == 0 || in_set_of_if == 3)
            {
                new_if(if_stmts_lables, in_set_of_if, when_to_end_if,
                       last_token_is_else, if_stmt_remain, while_flag);
                in_set_of_if = 1;
                if_stmt_remain = 0;
                while_flag = 0;
            }
            last_token_is_else = 0;
            exp_flag = EXP_BRANCH;
            if_unfold = 0;
            break;

        case N_WHILE:
            if (IF_DEBUG_PRINT)
            {
                printf("WHILE detected\n");
            }

            if (in_set_of_if == 0 || in_set_of_if == 3)
            {
                new_if(if_stmts_lables, in_set_of_if, when_to_end_if,
                       last_token_is_else, if_stmt_remain, while_flag);
                in_set_of_if = 1;
                if_stmt_remain = 0;
                // while_flag = 1;
            }
            exp_flag = EXP_LOOP;
            if_unfold = 0;
            break;

        case N_ELSE:
            last_token_is_else = 1;
        case N_LP:
        case N_RP:
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

            // case N_ELSE_L:
            //     if (IF_DEBUG_PRINT)
            //     {
            //         printf("ElseList detected\n");
            //     }
            //     if (in_set_of_if == 1)
            //     {
            //         in_set_of_if++;
            //         when_to_end_if = stack_ptr->top - 1;
            //     }
            //     if_unfold = 1;
            //     break;

        default:
            // 这些非终结符，理论上应该在函数中被处理掉
            // 但是既然走到了这一步，显然没有，所以这里肯定要报错
            if (IF_DEBUG_PRINT)
            {
                printf("ERROR: Unexpected node token with character %s\n", temp->character);
            }
            if_unfold = 1;
            break;
        }

        // 节点处理完了，下一个

        pop(stack_ptr);
        if (if_unfold)
        {
            reversed_insert(stack_ptr, temp);
        }

        // if(while_flag == 1){

        // }

        if (in_set_of_if == 2 && stack_ptr->top == when_to_end_if)
        {
            if_stmt_remain = push_all_stmt(if_stmts_lables, stack_ptr);
            in_set_of_if++;
            if (!last_token_is_else)
            {
                // 写上一个goto到终末位置
                new_operand(operand_to_use, VARIABLE,
                            if_stmts_lables->quene[if_stmts_lables->len - 1].end_lable, 0, 0);
                new_op(lst_of_ir, I_GOTO, *operand_to_use);
                del_operand_content(operand_to_use);
            }
            if (while_flag)
            {
                new_operand(operand_to_use, VARIABLE,
                            if_stmts_lables->quene[if_stmts_lables->len - 1].lable_names[1], 0, 0);
            }
            else
            {
                new_operand(operand_to_use, VARIABLE,
                            if_stmts_lables->quene[if_stmts_lables->len - 1]\
                            .lable_names[0], 0, 0);
            }

            new_op(lst_of_ir, I_LABLE, *operand_to_use);
            del_operand_content(operand_to_use);
            last_token_is_else = 0;
        }
        if (in_set_of_if == 3 && !while_flag &&
            stack_ptr->top == when_to_end_if + if_stmt_remain - 1)
        {
            if_block_lst *block_now = &(if_stmts_lables->quene[if_stmts_lables->len - 1]);
            IR_list* xxx = lst_of_ir;
            if_stmt_remain--;
            if (if_stmt_remain)
            {
                new_operand(operand_to_use, VARIABLE, block_now->end_lable, 0, 0);
                new_op(lst_of_ir, I_GOTO, *operand_to_use);
                del_operand_content(operand_to_use);
            }

            new_operand(operand_to_use, VARIABLE,
                        block_now->lable_names[block_now->node_cnt - if_stmt_remain], 0, 0);
            new_op(lst_of_ir, I_LABLE, *operand_to_use);
            del_operand_content(operand_to_use);
            last_token_is_else = 0;
            if (!if_stmt_remain)
            {
                // 走到这里就是真的完结了，需要用一些办法来进行几个控制变量的恢复以及内存空间的释放
                end_if(if_stmts_lables, &in_set_of_if,
                       &when_to_end_if, &last_token_is_else, &if_stmt_remain, &while_flag);
            }
        }

        if (in_set_of_if == 3 && while_flag &&
            stack_ptr->top == when_to_end_if + if_stmt_remain - 1)
        {
            if_block_lst *block_now = &(if_stmts_lables->quene[if_stmts_lables->len - 1]);
            new_operand(operand_to_use, VARIABLE, block_now->lable_names[0], 0, 0);
            new_op(lst_of_ir, I_GOTO, *operand_to_use);
            del_operand_content(operand_to_use);

            new_operand(operand_to_use, VARIABLE,
                        block_now->end_lable, 0, 0);
            new_op(lst_of_ir, I_LABLE, *operand_to_use);
            del_operand_content(operand_to_use);
            end_if(if_stmts_lables, &in_set_of_if,
                   &when_to_end_if, &last_token_is_else, &if_stmt_remain, &while_flag);
        }
        temp = top(stack_ptr);
    }
    FILE* F = fopen("test.txt", "w");
    print_IR(lst_of_ir, F);
    return 0;
}
