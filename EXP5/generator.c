#include "var_to_reg.c"
#include "function.c"
#include <string.h>
#define T_REG_NUM 10

char* t_regs[10] = {"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9"};

char* getSubstr(char* str, int len){
    char* sub_str = (char*)malloc(len * sizeof(char));
    for(int i = 0; i < len; i++)
        sub_str[i] = str[i+1];
    return sub_str;
}

int arrayLength(char* arr[]){
    int len = 0;
    for(int i = 0; arr[i] != '\0'; i++)
        len += 1;
    return len;
}

//不带优化的代码生成
void assign(operation* op, IR_list lst, FILE* f, int* index){
    static int arg_flag = 0;
    switch (op->code){
    //与函数无关指令
    case I_LABLE:
        fprintf(f, "%s:\n", op->opers[0].o_value.name);
        *index += 1;
        break;
        
    case I_ASSIGN:
        switch (op->opers[1].o_type)
        {
        case VARIABLE:
            //右值为变量
            if(op->opers[0].o_value.name[0] == '*'){
                char* sub_str = getSubstr(op->opers[0].o_value.name, strlen(op->opers[0].o_value.name) - 1);
                fprintf(f, "\tsw %s, 0(%s)\n", op->opers[1].o_value.name, sub_str);
            }
            else
                fprintf(f, "\tmove %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
            break;
        case IMMEDIATE:
            //右值为立即数
            fprintf(f, "\tli %s, %d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value);
            break;
        default:
            printf("Operand type error!\n");
            exit(1);
            break;
        }
        *index += 1;
        break;

    case I_ADD:
        if(op->opers[2].o_type == VARIABLE)
            fprintf(f, "\tadd %s, %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers[2].o_type == IMMEDIATE)
            fprintf(f, "\taddi %s, %s, %d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else{
            printf("Operand type error\n");
            exit(1);
        }
        *index += 1;
        break;

    case I_SUB:
        if(op->opers[2].o_type == VARIABLE)
            fprintf(f, "\tsub %s, %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers[2].o_type == IMMEDIATE)
            fprintf(f, "\taddi %s, %s, -%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        else{
            printf("Operand type error\n");
            exit(1);
        }
        *index += 1;
        break;

    case I_MUL:
        fprintf(f, "\tmul %s, %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        *index += 1;
        break;

    case I_DIV:
        fprintf(f, "\tdiv %s, %s\n", op->opers[1].o_value.name, op->opers[2].o_value.name);
        fprintf(f, "\tmflo %s\n", op->opers[0].o_value.name);
        *index += 1;
        break;

    //case I_AS_ADDR:
        // if(op->op_num == 2)
        //     fprintf(f, "%s:=&%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
        // else
        //     if(op->opers[2].o_type == IMMEDIATE)
        //      fprintf(f, "%s:=&%s+#%d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        //     else if(op->opers[2].o_type == VARIABLE)
        //         fprintf(f, "%s:=&%s+%s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        //break;

    case I_AS_VALUE:
        fprintf(f, "\tlw %s, 0(%s)\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
        *index += 1;
        break;

    case I_VALUE_ASSIGN:
        fprintf(f, "\tsw %s, 0(%s)\n", op->opers[1].o_value.name, op->opers[0].o_value.name);
        *index += 1;
        break;

    case I_GOTO:
        fprintf(f, "\tj %s\n", op->opers[0].o_value.name);
        *index += 1;
        break;

    case I_IF:
        if(op->opers->o_value.name == "==")
            fprintf(f, "\tbeq %s, %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers->o_value.name == "!=")
            fprintf(f, "\tbne %s, %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers->o_value.name == ">")
            fprintf(f, "\tbgt %s, %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers->o_value.name == "<")
            fprintf(f, "\tblt %s, %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers->o_value.name == ">=")
            fprintf(f, "\tbge %s, %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        else if(op->opers->o_value.name == "<=")
            fprintf(f, "\tble %s, %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.name);
        *index += 1;
        break;

    // case I_DEC:
    //     fprintf(f, "DEC %s #%d\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value);
    //     break;
    
    // case I_BOOL:
    //     for(int i = 0; i < op->op_num; i++)
    //         fprintf(f, "%s ", op->opers[i].o_value.name);
    //     fprintf(f, "\n");
    //     break;

    
    //与函数相关指令
    case I_FUNC:
        fprintf(f, "%s:\n", op->opers[0].o_value.name);
        arg_flag = 0;
        if(op->next->code == I_PARAM)
            arg_flag = op->next->op_num;
        *index = fun_pdec(op->index, arg_flag) + 1;
        for(int j = op->index + 1; j < *index;){
            operation* p = find_op(&lst, j);
            assign(p, lst, f, &j);
        }
        break;

    case I_RETURN:
        fprintf(f, "\tmove $v0, %s:\njr $ra\n", op->opers[0].o_value.name);
        *index = fun_edec(op->index, arg_flag) + 1;
        for(int j = op->index + 1; j < *index;){
            operation* p = find_op(&lst, j);
            assign(p, lst, f, &j);
        }
        break;

    case I_ARG:
        *index += 1;
        break;

    case I_CALL:
        //保存活跃变量，压入栈中
        for(int i = 0; i < T_REG_NUM; i++){
            *index += 1;
            sw_live(0, t_regs[i], 4 * i, *index);
        }
        *index += 1;
        for(int j = op->index + 1; j < *index;){
            operation* p = find_op(&lst, j);
            assign(p, lst, f, &j);
        }
        //调用函数
        arg_flag = 0;
        if(op->next->code == I_ARG)
            arg_flag = op->next->op_num;
        *index = fun_call(op->index, arg_flag, op->opers[1].o_value.name) + 1;
        for(int j = op->index + 1; j < *index;){
            operation* p = find_op(&lst, j);
            assign(p, lst, f, &j);
        }
        int pre_index = *index;
        //恢复活跃变量
        for(int i = 0; i < T_REG_NUM; i++){
            lw_live(0, t_regs[i], 4 * i, *index);
            *index += 1;
        }
        for(int j = pre_index; j < *index;){
            operation* p = find_op(&lst, j);
            assign(p, lst, f, &j);
        }
        fprintf(f, "\tmove %s, $v0\n", op->opers[0].o_value.name);
        break;

    case I_PARAM:
        *index += 1;
        break;

    //使用read和write功能相当于函数调用
    case I_READ:
        *index = fun_call(op->index, 0, "read");
        for(int j = op->index + 1; j < *index;){
            operation* p = find_op(&lst, j);
            assign(p, lst, f, &j);
        }
        fprintf(f, "\tmove %s, $v0\n", op->opers[0].o_value.name);
        break;

    case I_WRITE:
        *index = fun_call(op->index, 1, "write");
        for(int j = op->index + 1; j < *index;){
            operation* p = find_op(&lst, j);
            assign(p, lst, f, &j);
        }
        //write相当于一个没有返回值的函数，后面无move指令
        break;

    //栈管理相关指令
    case I_SW:
        fprintf(f, "\tsw %s, %d($sp)\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value);
        *index += 1;
        break;

    case I_LW:
        fprintf(f, "\tlw %s, %d($sp)\n", op->opers[0].o_value.name, op->opers[1].o_value.im_value);
        *index += 1;
        break;

    case I_JAL:
        fprintf(f, "\tjal %s\n", op->opers[0].o_value.name);
        *index += 1;
        break;
    
    case I_JR:
        fprintf(f, "\tjr %s\n", op->opers[0].o_value.name);
        *index += 1;
        break;

    case I_MOVE:
        fprintf(f, "\tmove %s, %s\n", op->opers[0].o_value.name, op->opers[1].o_value.name);
        *index += 1;
        break;

    case I_SUBU:
        fprintf(f, "\tsubu %s, %s, %d\n", op->opers[0].o_value.name, op->opers[1].o_value.name, op->opers[2].o_value.im_value);
        *index += 1;
        break;

    case I_CLEAN: case I_RECOVER: case I_DEF:
        *index += 1;
        break;

    default:
        printf("Wrong operation type!\n");
        exit(0);
    }
}

//带窥孔优化的代码生成(数组和结构体偏移化简)
void assign_optim(operation* op, FILE* f, int* index){
    operation* op1 = op->next;
    //operation* op2 = op1->next;
    switch (op1->code){
        case I_AS_VALUE:
            fprintf(f, "\tlw %s, %d(%s)\n", op1->opers[0].o_value.name, op->opers[2].o_value.im_value, op->opers[1].o_value.name);
            *index += 2;
            break;
        case I_VALUE_ASSIGN:
            fprintf(f, "\tsw %s, %d(%s)\n", op1->opers[1].o_value.name, op->opers[2].o_value.im_value, op->opers[1].o_value.name);
            *index += 2;
            break;
        case I_ASSIGN:
            if(op->opers[0].o_type == VARIABLE && op->opers[0].o_value.name[0] == '*'){
                //char* sub_str = getSubstr(op->opers[0].o_value.name, strlen(op->opers[0].o_value.name) - 1);
                fprintf(f, "\tsw %s, %d(%s)\n", op1->opers[1].o_value.name, op->opers[2].o_value.im_value, op->opers[1].o_value.name);
                *index += 2;
            }
            else{
                printf("error!\n");
                exit(0);
            }
            break;
    }
}

void print_read(FILE* f){
    fprintf(f, "read:\n");
    fprintf(f, "\tli $v0, 4\n");
    fprintf(f, "\tla $a0, _prompt\n");
    fprintf(f, "\tsyscall\n");
    fprintf(f, "\tli $v0, 4\n");
    fprintf(f, "\tsyscall\n");
    fprintf(f, "\tjr $ra\n\n");
}

void print_write(FILE* f){
    fprintf(f, "write:\n");
    fprintf(f, "\tli $v0, 1\n");
    fprintf(f, "\tsyscall\n");
    fprintf(f, "\tli $v0, 4\n");
    fprintf(f, "\tla $a0, _ret\n");
    fprintf(f, "\tsyscall\n");
    fprintf(f, "\tmove $v0, $0\n");
    fprintf(f, "\tjr $ra\n\n");
}

void print_data(FILE* f){
    fprintf(f, ".data\n");
    fprintf(f, "_prompt: .asciiz \"Enter an integer\"\n");
    fprintf(f, "_ret: .asciiz \"\\n\"\n");
    fprintf(f, ".globl main\n");
}

void print_text(IR_list lst, FILE* f){
    fprintf(f, ".text\n");
    //若需要read交互
    if(lst.if_read == 1)
        print_read(f);
    //若需要write交互
    if(lst.if_write == 1)
        print_write(f);
    operation* p = lst.head;
    int index = 0;
    while(1){
        if(p->flag == 1)
            assign_optim(p, f, &index);  //需要窥孔优化
        else
            assign(p, lst, f, &index);
        if(p == lst.tail)
            return;
        p = find_op(&lst, index);
    }
}

//生成目标代码
void generate(IR_list lst, FILE* f){
    //数据段
    print_data(f);
    //代码段
    print_text(lst, f);
}
