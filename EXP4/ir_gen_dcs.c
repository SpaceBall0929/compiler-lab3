// # include "ir_gen_dcs.h"
#include "exp.h"
#define IF_BLOCK_NUM 15

//生成变量名
int num_of_vars = 0;
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
        letter_now += 2;
        num_of_vars = 0;
    }

    return out;
}

//生成标签名
int lable_num0 = 0;
int lable_num1 = 0;

//生成标签名
char* gen_lable(){
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

    return lable_name;
}

//便捷的插入标签
int insert_lable(char* lable_name){
    // IR_list* my = lst_of_ir;
    operand_list* myop = init_operand_list();
    char* deep_copy_str = malloc(sizeof(char) * strlen(lable_name));
    strcpy(deep_copy_str, lable_name);  
    new_operand(myop, VARIABLE, deep_copy_str, 0, 0);
    new_op(lst_of_ir, I_LABLE, *myop);
    return 0;
}

int insert_func(char* lable_name){
    IR_list* my = lst_of_ir;
    operand_list* myop = init_operand_list();
    char* deep_copy_str = malloc(sizeof(char) * strlen(lable_name));
    strcpy(deep_copy_str, lable_name);  
    new_operand(myop, VARIABLE, deep_copy_str, 0, 0);
    new_op(lst_of_ir, I_FUNC, *myop);
    return 0;
}

//便捷的插入跳转
int insert_goto(char* lable_name){
    operand_list* myop = init_operand_list();  
    new_operand(myop, VARIABLE, lable_name, 0, 0);
    new_op(lst_of_ir, I_GOTO, *myop);
    return 0;
}

//保存处理IF后面stmt的结构体，用栈管理if嵌套问题

//管理某一层次的if-else，注意全部清空
typedef struct if_block_lst{
    char* lable_names[IF_BLOCK_NUM + 1];
    int name_cnt;
    treeNode* block_nodes[IF_BLOCK_NUM];
    int node_cnt;
    char* end_lable;
    int flags[5]; 
}if_block_lst;

//管理嵌套的if-else结构
typedef struct if_stack{
    if_block_lst quene[IF_BLOCK_NUM];
    int len;
}if_stack;

// int push(if_stack* mystack){

// }

// int pop(){

// }

//
int new_if(if_stack* lst, int flag0, int flag1, int flag2, int flag3, int flag4){
    lst->quene[lst->len].flags[0] = flag0;
    lst->quene[lst->len].flags[1] = flag1;
    lst->quene[lst->len].flags[2] = flag2;
    lst->quene[lst->len].flags[3] = flag3;
    lst->quene[lst->len].flags[4] = flag4;
    lst->len += 1;
}


//注意，这个lable_name是会被使用的，请不要释放内存
void add_lable(if_stack* lst, char* lable_name){
    lst->quene[lst->len - 1].lable_names[lst->quene[lst->len - 1].name_cnt++] = lable_name;
}

//添加到栈当前层次的stmt表中
void add_stmt(if_stack* lst, treeNode* stmt){
    lst->quene[lst->len - 1].block_nodes[lst->quene[lst->len - 1].node_cnt++] = stmt;
}

//结束本层次的if-else, 把所有的stmt压栈, 确定最终的结束lable
//返回一共有几个stmt块需要处理
int push_all_stmt(if_stack* lst, seqStack* tree_stack){
    if_block_lst* loc_now = &(lst->quene[lst->len - 1]);
    for(int i = loc_now->node_cnt - 1; i > -1; i--){
        push(tree_stack, loc_now->block_nodes[i]);
    }
    loc_now->end_lable = gen_lable();
    loc_now->lable_names[loc_now->name_cnt++] = loc_now->end_lable;
    return loc_now->node_cnt;
}

//恢复几个flag的状态，退栈，消除标签字符串占用的内存空间
int end_if(if_stack* lst, int* flag0, int* flag1, int* flag2, int* flag3, int* flag4){
    if_block_lst* loc_now = &(lst->quene[lst->len - 1]);
    *flag0 = loc_now->flags[0];
    *flag1 = loc_now->flags[1];
    *flag2 = loc_now->flags[2];
    *flag3 = loc_now->flags[3];
    *flag4 = loc_now->flags[4];
    lst->len--;
    loc_now->name_cnt = 0;
    loc_now->node_cnt = 0;
    for(int i = 0; i < loc_now->name_cnt; i++){
        free(loc_now->lable_names[i]);
    }
    
    return 0;
}




