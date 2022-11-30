#include <stdio.h>
#include <stdlib.h>

//操作数类型
enum operand_type {VARIABLE, IMMEDIATE, ADDRESS};

//操作数
struct operand
{
    enum operand_type o_type;  //操作数类型
    union {
        char* name; //变量名
        int im_value;   //立即数取值
        long addr;      //地址          
    }o_value;
    // struct operand* next;
    
};
typedef struct operand operand;

//指令类型
enum opcode {
    I_LABLE,        //定义标号x
    I_FUNC,         //定义函数f
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
    I_PARAM,         //函数参数声明
    I_READ,          //从控制台读取
    I_WRITE          //向控制台打印
    };       

struct operation
{
    enum opcode code;        //指令类型
    operand* opers;          //操作数链表
    int op_num;
    struct operation* next;  //双向链表
    struct operation* front;
};
typedef struct operation operation;

//指令初始化
operation* init_op(enum opcode co, operand* op, int op_num){
    operation* new_op = (operation*)malloc(sizeof(operation));
    new_op->code = co;
    new_op->opers = (operand*)malloc(op_num * sizeof(operand));
    for(int i = 0;i < op_num; i++)
        new_op->opers[i] = op[i];
    new_op->next = new_op->front = NULL;
    return new_op;
}

struct IR_list  //IR链表
{
    operation* head;
    operation* tail;
    int length;
};
typedef struct IR_list IR_list;

//IR链表初始化
IR_list* init_IR(){
    IR_list* new_lst = (IR_list*)malloc(sizeof(IR_list));
    new_lst->head = new_lst->tail = NULL;
    new_lst->length = 0;
    return new_lst;
}

//在末尾插入指令
void new_op(IR_list* lst, enum opcode co, operand* op, int op_num){
    operation* new = init_op(co, op, op_num);
    if(lst->head == NULL && lst->tail == NULL){
        lst->head = lst->tail = new;
        lst->length += 1;
        return;
    }
    lst->tail->next = new;
    new->front = lst->tail;
    new->next = NULL;
    lst->tail = new;
    lst->length += 1;
    return;
}

//插入后为第index个（从0开始）
void insert_op(IR_list* lst, enum opcode co, operand* op, int op_num, int index){
    if(index >= lst->length){
        printf("Index out of range!\n");
        exit(1);
    }
    operation* new = init_op(co, op, op_num);
    operation* pt = lst->head;
    for(int i = 1;i < index; i++)
        pt = pt->next;
    new->next = pt->next;
    pt->next->front = new;
    pt->next = new;
    new->front = pt;
}

//根据index查找指令
operation* find_op(IR_list* lst, int index){
    if(index >= lst->length){
        printf("Index out of range!\n");
        exit(1);
    }
    operation* pt = lst->head;
    for(int i = 1;i <= index; i++)
        pt = pt->next;
    return pt;
}

void print_op(operation* op, FILE* F){
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
            //右值为变量
            fprintf(F, "%s:=%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
            break;
        case IMMEDIATE:
            //右值为立即数
            fprintf(F, "%s:=%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value);
            break;

        default:
            printf("Operand type error!\n");
            exit(1);
            break;
        }
        break;

    case I_ADD:
        if(op->opers[1].o_type == VARIABLE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%s+%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers[1].o_type == VARIABLE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%s+%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else if(op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%d+%s\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.name);
        else if(op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%d+%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.im_value);
        else{
            printf("Operand type error\n");
            exit(1);
        }
        break;

    case I_SUB:
        if(op->opers[1].o_type == VARIABLE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%s-%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers[1].o_type == VARIABLE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%s-%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else if(op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%d-%s\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.name);
        else if(op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%d-%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.im_value);
        else{
            printf("Operand type error\n");
            exit(1);
        }
        break;

    case I_MUL:
        if(op->opers[1].o_type == VARIABLE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%s*%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers[1].o_type == VARIABLE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%s*%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else if(op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%d*%s\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.name);
        else if(op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%d*%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.im_value);
        else{
            printf("Operand type error\n");
            exit(1);
        }
        break;

    case I_DIV:
        if(op->opers[1].o_type == VARIABLE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%s/%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers[1].o_type == VARIABLE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%s/%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else if(op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == VARIABLE)
            fprintf(F, "%s:=%d/%s\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.name);
        else if(op->opers[1].o_type == IMMEDIATE && op->opers[2].o_type == IMMEDIATE)
            fprintf(F, "%s:=%d/%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value, op->opers[2].o_value.im_value);
        else{
            printf("Operand type error\n");
            exit(1);
        }
        break;

    case I_AS_ADDR:
        fprintf(F, "%s:=&%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
        break;

    case I_AS_VALUE:
        fprintf(F, "%s:=*%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
        break;

    case I_VALUE_ASSIGN:
        switch (op->opers[1].o_type)
        {
        case VARIABLE:
            //右值为变量
            fprintf(F, "*%s:=%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
            break;
        case IMMEDIATE:
            //右值为立即数
            fprintf(F, "*%s:=%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value);
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
            fprintf(F, "IF %s %s %d GOTO %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value, op->opers[3].o_value.name);
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
            //右值为变量
            fprintf(F, "RETURN %s\n", op->opers[0].o_value.name);
            break;
        case IMMEDIATE:
            //右值为立即数
            fprintf(F, "RETURN %d\n", op->opers[0].o_value.im_value);
            break;
        default:
            printf("Operand type error!\n");
            exit(1);
            break;
        }
        break;

    case I_DEC:
        fprintf(F, "DEC %s %d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value);
        break;

    case I_ARG:
        switch (op->opers[0].o_type)
        {
        case VARIABLE:
            //右值为变量
            fprintf(F, "ARG %s\n", op->opers[0].o_value.name);
            break;
        case IMMEDIATE:
            //右值为立即数
            fprintf(F, "ARG %d\n", op->opers[0].o_value.im_value);
            break;
        default:
            printf("Operand type error!\n");
            exit(1);
            break;
        }
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
        fprintf(F, "WRITE %s\n", op->opers[0].o_value.name);
        break;
    
    default:
        printf("Operation type error!\n");
        exit(1);
        break;
    }
    return;
}

void print_IR(IR_list* lst, FILE* F){
    operation* pt = lst->head;
    for(int i = 0; i < lst->length; i++){
        print_op(pt, F);
        pt = pt->next;
    }
    return;
}

// int main(){
//     IR_list* lst = init_IR();
//     new_op(lst, I_ADD, )
//     return 0;
// }