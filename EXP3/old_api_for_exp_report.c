enum var_type{
    V_INT,
    V_FLOAT,
    V_ARRAY

};


typedef struct node
{
    char *character;
    int line_no; //行号
    struct node *child;
    struct node *sibling;
    enum node_type type;
    union
    {
        int intVal;
        float floatVal;
        char *IDVal;
    } subtype;
} * treeNode;

typedef treeNode Tree;

//两个系统之间传递变量信息用
//并不是每一个值在每一个函数中都有用
struct var_dec
{
    //用枚举类表明变量的种类
    enum var_type type;
    
    //变量名字
    char* name;
    
    //变量出现的位置
    int line;
    
    //如果是个表的话，这里记录表的长度
    int len;
    
    //多维数组用这个指向下一个，如果值为NULL表明到头了
    var_dec* next_dimention;
};
typedef struct var_dec var_dec;


//两个系统之间传递函数信息用
typedef struct fun_dec
{
    //函数头声明的返回值
    enum var_type return_type;

    //函数名
    char* name;
    
    //函数声明所在行
    int line;

    //函数参数链表，最后一个为空值
    var_dec* next_para;

} fun_dec;

//两个系统之间传递结构体信息用
typedef struct struct_dec
{
    //结构体名字
    char* name;

    //定义出现的行数
    int line;

    //结构体内容物链表
    var_dec* next_para;

} struct_dec;


// test
//  enum Datanode_type {Int, Float, Array, Struct, StructDomain};

// //表明继续执行深度优先遍历
// //最终形态可能并非这样一个函数，这里有待商讨
// treeNode *deep_search();

// //从def_node所有子节点中收集完整的变量信息，再交给作用域管理部分处理
// var_dec node_to_info(treeNode *def_node);

// //和上面那个函数类似，就是收集函数定义然后返回
// fun_dec node_to_fun(treeNode *def_node);

// //类似，返回一个结构体定义的信息
// struct_dec node_to_struct(treeNode *def_node);

// //...其实应该有很多很多种节点需要处理，这里列举的是最重要的判定函数，变量，结构体定义的部分，总之就是，一大堆函数咯

// //开辟新的作用域，扫描到左大括号调用
// int new_block();

// //创建变量，函数和结构体定义，正常返回0，不正常就返回错误编号
// int creat_var(var_dec info);
// int creat_fun(fun_dec info);
// int creat_struct(struct_dec info);

// //查询此处用到的变量是否合法，根据程序获取的信息查询，正常就返回0，不正常就返回错误编号
// int query_var_legal(var_dec myvar);
// int query_fun_legal(fun_dec myfun);
// int query_struct_legal(struct_dec mystruct);