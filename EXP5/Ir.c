#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SymbolTable.c"
#define MAX_TEMP_OP_NUM 5
#define INT_LEN 4
#define FLOAT_LEN 4

// 基本数据类型长度（单位：字节）
int data_len[2] = {INT_LEN, FLOAT_LEN};

// 操作数类型
enum operand_type
{
    VARIABLE,
    IMMEDIATE,
    ADDRESS
};

// 操作数
struct operand
{
    enum operand_type o_type; // 操作数类型
    union
    {
        char *name;   // 变量名
        int im_value; // 立即数取值
        long addr;    // 地址
    } o_value;
    struct operand *next;
};
typedef struct operand operand;

operand *init_operand(enum operand_type t, char *n, int i, long a)
{
    operand *oper = (operand *)malloc(sizeof(operand));
    oper->o_type = t;
    if (t == VARIABLE)
    {
        oper->o_value.name = (char *)malloc(sizeof(char) * strlen(n));
        strcpy(oper->o_value.name, n);
    }
    else if (t == IMMEDIATE)
        oper->o_value.im_value = i;
    else if (t == ADDRESS)
        oper->o_value.addr = a;
    else
    {
        exit(1);
    }
    return oper;
}

// 操作数链表
typedef struct operand_list
{
    operand *head;
    operand *tail;
    int length;
} operand_list;

operand_list *init_operand_list()
{
    operand_list *new = (operand_list *)malloc(sizeof(operand_list));
    new->head = new->tail = NULL;
    new->length = 0;
    return new;
}

// 在末尾插入操作数
void new_operand(operand_list *lst, enum operand_type t, char *n, int i, long a)
{
    operand *new = init_operand(t, n, i, a);
    if (lst->head == NULL && lst->tail == NULL)
    {
        lst->head = lst->tail = new;
        lst->length += 1;
        // printf("test\n");
        return;
    }
    lst->tail->next = new;
    new->next = NULL;
    lst->tail = new;
    lst->length += 1;
    return;
}

void add_operand(operand_list *lst, operand *new)
{
    if (lst->head == NULL && lst->tail == NULL)
    {
        lst->head = lst->tail = new;
        lst->length += 1;
        // printf("test\n");
        return;
    }
    lst->tail->next = new;
    new->next = NULL;
    lst->tail = new;
    lst->length += 1;
    return;
}

// 拼接参数表
operand_list *add_args(operand_list *lst1, operand_list *lst2)
{
    operand_list *new_lst = init_operand_list();
    if (lst1->head == NULL && lst1->tail == NULL)
    {
        new_lst = lst2;
        printf("test3\n");
        return new_lst;
    }
    new_lst->head = lst1->head;
    new_lst->tail = lst1->tail;
    new_lst->tail->next = lst2->head;
    new_lst->tail = lst2->tail;
    new_lst->length = lst1->length + lst2->length;
    return new_lst;
}

// 删除当前参数链表
int del_operand_content(operand_list *lst_to_del)
{
    operand *temp0 = lst_to_del->head;
    operand *temp1;
    while (temp0 != NULL)
    {
        temp1 = temp0;
        temp0 = temp0->next;
        free(temp1);
    }
    lst_to_del->length = 0;
    lst_to_del->head = NULL;
    lst_to_del->tail = NULL;
    return 0;
}

// 指令类型
enum opcode
{
    I_LABLE,        // 定义标号x
    I_FUNC,         // 定义函数f
    I_ASSIGN,       // 赋值操作
    I_ADD,          // 加法操作
    I_SUB,          // 减法操作
    I_MUL,          // 乘法操作
    I_DIV,          // 除法操作
    I_AS_ADDR,      // x := &y
    I_AS_VALUE,     // x := *y
    I_VALUE_ASSIGN, //*x := y
    I_GOTO,         // 无条件跳转到标号x
    I_IF,           // 若满足关系则跳转
    I_RETURN,       // 函数返回
    I_DEC,          // 内存空间申请
    I_ARG,          // 函数传实参
    I_CALL,         // 函数调用
    I_PARAM,        // 函数参数声明
    I_READ,         // 从控制台读取
    I_WRITE,        // 向控制台打印
    I_BOOL,         // 关系式
    I_SW,           // 函数处理中sw
    I_LW,           // 函数处理中lw
    I_JR,           // 函数处理中jr
    I_JAL,          // 函数处理中jal
    I_MOVE,         // 函数处理中move
    I_SUBU,         // 函数处理中subu

    // dcs增设的，没有对应的输出
    I_CLEAN,   // 把变量存储到静态区
    I_RECOVER, // 从静态区恢复变量
    I_DEF      // 定义一下变量
};

struct operation
{
    enum opcode code; // 指令类型
    operand *opers;   // 操作数链表
    int op_num;
    struct operation *next; // 双向链表
    struct operation *front;
    int flag;  // 是否需要优化（数组/结构体+偏移）
    int index; // 指令的index
};
typedef struct operation operation;

// 指令初始化
/*
传参：
设lst为操作数链表，指令类型为co，传入
init_op(co, lst);
*/
operation *init_op(enum opcode co, operand_list oplst)
{
    operation *new_op = (operation *)malloc(sizeof(operation));
    new_op->code = co;
    new_op->op_num = oplst.length;
    new_op->opers = (operand *)malloc(oplst.length * sizeof(operand));
    operand *pt = oplst.head;
    for (int i = 0; i < oplst.length; i++)
    {
        new_op->opers[i] = *pt;
        pt = pt->next;
    }
    new_op->next = new_op->front = NULL;
    new_op->flag = 0;
    new_op->index = -1;
    return new_op;
}

struct IR_list // IR链表
{
    operation *head;
    operation *tail;
    int length;
    int if_read;
    int if_write;
};
typedef struct IR_list IR_list;

// IR链表初始化
IR_list *init_IR()
{
    IR_list *new_lst = (IR_list *)malloc(sizeof(IR_list));
    new_lst->head = new_lst->tail = NULL;
    new_lst->length = 0;
    new_lst->if_read = new_lst->if_write = 0;
    return new_lst;
}

// 在末尾插入指令
void new_op(IR_list *lst, enum opcode co, operand_list oplst)
{
    operation *new = init_op(co, oplst);
    if (lst->head == NULL && lst->tail == NULL)
    {
        lst->head = lst->tail = new;
        new->index = 0;
        lst->length += 1;
        return;
    }
    lst->tail->next = new;
    new->front = lst->tail;
    new->next = NULL;
    new->index = lst->length;
    lst->tail = new;
    lst->length += 1;
    if(co == I_ADD)
    {
        111;
    }
    return;
}

// 末尾插入运算
void add_op(IR_list *lst, operation *op)
{
    if (lst->head == NULL && lst->tail == NULL)
    {
        lst->head = lst->tail = op;
        op->index = 0;
        lst->length += 1;
        return;
    }
    lst->tail->next = op;
    op->front = lst->tail;
    op->next = NULL;
    op->index = lst->length;
    lst->tail = op;
    lst->length += 1;
    if(op->code == I_ADD)
    {
        111;
    }
    return;
}

// 插入后为第index个（从0开始）
void insert_op(IR_list *lst, enum opcode co, operand_list oplst, int index)
{
    if (index > lst->length || index < 0)
    {
        printf("Index out of range!\n");
        exit(0);
    }
    operation *new = init_op(co, oplst);
    if (lst->head == NULL && lst->tail == NULL)
    {
        lst->head = lst->tail = new;
        new->index = 0;
        lst->length += 1;
        return;
    }
    operation *pt = lst->head;
    for (int i = 1; i < index; i++)
        pt = pt->next;
    new->next = pt->next;
    pt->next->front = new;
    pt->next = new;
    new->front = pt;
    new->index = index;
    lst->length += 1;
}

// 根据index查找指令
operation *find_op(IR_list *lst, int index)
{
    if (index >= lst->length || index < 0)
    {
        printf("Index out of range!\n");
        exit(0);
    }
    operation *pt = lst->head;
    for (int i = 1; i <= index; i++)
        pt = pt->next;
    return pt;
}

void print_op(operation *op, FILE *F)
{
    switch (op->code)
    {
    case I_LABLE:
        fprintf(F, "LABLE %s:\n", op->opers[0].o_value.name);
        break;

    case I_FUNC:
        fprintf(F, "FUNCTION %s:\n", op->opers[0].o_value.name);
        break;

    case I_ASSIGN:
        switch (op->opers[1].o_type)
        {
        case VARIABLE:
            // 右值为变量
            fprintf(F, "%s:=%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
            break;
        case IMMEDIATE:
            // 右值为立即数
            fprintf(F, "%s:=#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value);
            break;

        default:
            printf("Operand type error!\n");
            exit(1);
            break;
        }
        break;

    case I_ADD:
        if (op->opers[1].o_type == VARIABLE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%s+%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if (op->opers[1].o_type == VARIABLE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%s+#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else if (op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=#%d+%s\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.name);
        else if (op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=#%d+#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.im_value);
        else
        {
            printf("Operand type error\n");
            exit(1);
        }
        break;

    case I_SUB:
        if (op->opers[1].o_type == VARIABLE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%s-%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if (op->opers[1].o_type == VARIABLE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%s-#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else if (op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=#%d-%s\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.name);
        else if (op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=#%d-#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.im_value);
        else
        {
            printf("Operand type error\n");
            exit(1);
        }
        break;

    case I_MUL:
        if (op->opers[1].o_type == VARIABLE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%s*%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if (op->opers[1].o_type == VARIABLE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%s*#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else if (op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=#%d*%s\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.name);
        else if (op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=#%d*#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.im_value);
        else
        {
            printf("Operand type error\n");
            exit(1);
        }
        break;

    case I_DIV:
        if (op->opers[1].o_type == VARIABLE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%s/%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if (op->opers[1].o_type == VARIABLE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%s/#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else if (op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=#%d/%s\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.name);
        else if (op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=#%d/#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.im_value);
        else
        {
            printf("Operand type error\n");
            exit(1);
        }
        break;

    case I_AS_ADDR:
        if (op->op_num == 2)
            fprintf(F, "%s:=&%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
        else if (op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=&%s+#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else if (op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=&%s+%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        break;

    case I_AS_VALUE:
        fprintf(F, "%s:=*%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
        break;

    case I_VALUE_ASSIGN:
        switch (op->opers[1].o_type)
        {
        case VARIABLE:
            // 右值为变量
            fprintf(F, "*%s:=%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
            break;
        case IMMEDIATE:
            // 右值为立即数
            fprintf(F, "*%s:=#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value);
            break;

        default:
            printf("Operand type error!\n");
            exit(1);
            break;
        }
        break;

    case I_GOTO:
        fprintf(F, "GOTO %s\n", op->opers[0].o_value.name);
        break;

    case I_IF:
        switch (op->opers[2].o_type)
        {
        case VARIABLE:
            fprintf(F, "IF %s %s %s GOTO %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name, op->opers[3].o_value.name);
            break;
        case IMMEDIATE:
            fprintf(F, "IF %s %s #%d GOTO %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value, op->opers[3].o_value.name);
            break;
        default:
            printf("Operand type error!\n");
            exit(1);
            break;
        }
        break;

    case I_RETURN:
        switch (op->opers[0].o_type)
        {
        case VARIABLE:
            // 右值为变量
            fprintf(F, "RETURN %s\n", op->opers[0].o_value.name);
            break;
        case IMMEDIATE:
            // 右值为立即数
            fprintf(F, "RETURN #%d\n", op->opers[0].o_value.im_value);
            break;
        default:
            printf("Operand type error!\n");
            exit(1);
            break;
        }
        break;

    case I_DEC:
        fprintf(F, "DEC %s #%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value);
        break;

    case I_ARG:
        fprintf(F, "ARG ");
        for (int i = 0; i < op->op_num; i++)
        {
            switch (op->opers[i].o_type)
            {
            case VARIABLE:
                // 右值为变量
                fprintf(F, "%s", op->opers[i].o_value.name);
                if (i < op->op_num - 1)
                    fprintf(F, ",");
                break;
            case IMMEDIATE:
                // 右值为立即数
                fprintf(F, "#%d", op->opers[i].o_value.im_value);
                if (i < op->op_num - 1)
                    fprintf(F, ",");
                break;
            default:
                printf("Operand type error!\n");
                exit(1);
                break;
            }
        }
        fprintf(F, "\n");
        break;

    case I_CALL:
        fprintf(F, "%s:=CALL %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
        break;

    case I_PARAM:
        fprintf(F, "PARAM %s\n", op->opers[0].o_value.name);
        break;

    case I_READ:
        fprintf(F, "READ %s\n", op->opers[0].o_value.name);
        break;

    case I_WRITE:
        switch (op->opers[0].o_type)
        {
        case VARIABLE:
            // 变量
            fprintf(F, "WRITE %s\n", op->opers[0].o_value.name);
            break;
        case IMMEDIATE:
            // 立即数
            fprintf(F, "WRITE %d\n", op->opers[0].o_value.im_value);
            break;
        default:
            printf("Operand type error!\n");
            exit(1);
            break;
        }
        break;

    case I_BOOL:
        for (int i = 0; i < op->op_num; i++)
            fprintf(F, "%s ", op->opers[i].o_value.name);
        fprintf(F, "\n");
        break;

    case I_RECOVER:
        fprintf(F, "RECOVER %d %s\n", op->opers[0].o_value.im_value, op->opers[1].o_value.name);
        break;

    case I_CLEAN:
        fprintf(F, "CLEAN %d %s\n", op->opers[0].o_value.im_value, op->opers[1].o_value.name);
        break;

    case I_DEF:
        fprintf(F, "DEF %s\n", op->opers[0].o_value.name);
        break;

    default:
        printf("Operation type error!\n");
        exit(1);
        break;
    }
    return;
}

void print_IR(IR_list *lst, FILE *F)
{
    operation *pt = lst->head;
    for (int i = 0; i < lst->length; i++)
    {
        print_op(pt, F);
        pt = pt->next;
    }
    return;
}

char *get_ir(treeNode *node, SymbolTableVar *st)
{
    int i = findPosVar(*st, node->subtype.IDVal);
    char *ir_name = st->data[i].ir_name;
    return ir_name;
}

enum operand_type get_ir_type(treeNode *node)
{
    if (node->nodeType == N_ID)
        return VARIABLE;
    else
    {
        printf("Node type error!\n");
        exit(1);
    }
}

// 返回临时变量
operand *temp_op(int flag)
{
    static int count = 0;
    count += flag;
    char *num = (char *)malloc(MAX_TEMP_OP_NUM * sizeof(char));
    char *t = (char *)malloc((MAX_TEMP_OP_NUM + 1) * sizeof(char));
    t[0] = 't';
    sprintf(num, "%d", count);
    strcat(t, num);
    operand *op = init_operand(VARIABLE, t, 0, 0);
    return op;
}

// 求数组大小（单位：字节）
int array_size(dataNodeVar *array)
{
    int unit_num = 1;
    for (int i = 0; i < array->numdim; i++)
        unit_num *= array->len_of_dims[i];
    return unit_num * data_len[array->arrayVarType];
}

// 求结构体大小（单位：字节）
int struct_size(SymbolTableStruct *st, int struct_type)
{
    dataNodeStruct node = st->data[struct_type - D_AMT];
    dataNodeVar *p = node.structDomains;
    int i = 0;
    while (p != NULL)
    {
        if (p->varType == D_ARRAY) // 成员为数组
            i += array_size(p);
        else if (p->varType >= D_AMT) // 成员为结构体
            i += struct_size(st, p->varType);
        else // 成员为INT或FLOAT
            i += data_len[p->varType];
        p = p->next;
    }
    return i;
}

// 结构体域的偏移量
int struct_offset(SymbolTableStruct *st, char *struct_type, char *domain_name)
{
    dataNodeStruct *node = getNodeStruct(*st, struct_type);
    dataNodeVar *p = node->structDomains;
    int i = 0;
    while (p != NULL)
    {
        if (!strcmp(p->varName, domain_name))
            return i;
        if (p->varType == D_ARRAY) // 成员为数组
            i += array_size(p);
        else if (p->varType >= D_AMT) // 成员为结构体
            i += struct_size(st, p->varType);
        else // 成员为INT或FLOAT
            i += data_len[p->varType];
        p = p->next;
    }
    return i;
}
