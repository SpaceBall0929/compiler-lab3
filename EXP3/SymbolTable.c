# include "tree.c"
# include "AssistantFuncs.c"
# include <math.h>
# include <string.h>
# define TABLESIZE 128

enum Status { Active, Empty, Deleted };
enum DataType {Int, Float, Array, Struct, StructDomain};


struct datanode1{
    char* varName;  //变量名
    enum DataType varType; //变量数据类型
    int numdim;     //若为数组，数组维度
    int* len_of_dims; //每个维度的长度
    struct datanode1* next;
};  //变量符号表结点

typedef struct datanode1 dataNodeVar;

dataNodeVar newNodeVar(char* name, enum DataType type, int nd){
    dataNodeVar newNode;
    newNode.varName = name;
    newNode.varType = type;
    newNode.numdim = nd;
    if(nd > 0)  //若为数组，使用int* len_of_dims存储每个维度长度
        newNode.len_of_dims = (int*)calloc(newNode.numdim, sizeof(int));
    else        //若非数组，numdim传入0，len_of_dims为NULL
        newNode.len_of_dims = NULL;
    return newNode;
}

struct datanode2{
    char* funcName;  //函数名
    enum DataType returnType; //函数返回值类型
    int defined;   //函数是否定义
    dataNodeVar* args;   //函数形参表
};  //函数符号表结点

typedef struct datanode2 dataNodeFunc;

dataNodeFunc newNodeFunc(char* name, enum DataType type, int de, dataNodeVar* ar){
    dataNodeFunc newNode;
    newNode.funcName = name;
    newNode.returnType = type;
    newNode.defined = de;
    newNode.args = ar;
    return newNode;
}

struct datanode3{
    char* structName;  //结构体名
    char* structTypeName;  //结构体类型名
    dataNodeVar* structDomains;  //结构体域，为一系列变量，存储为一个变量结点链表
};  //结构体符号表结点

typedef struct datanode3 dataNodeStruct;

dataNodeStruct newNodeStruct(char* name, char* tname){
    dataNodeStruct newNode;
    newNode.structName = name;
    newNode.structTypeName = tname;
    return newNode;
}

// typedef struct argnode{
//     enum DataType argType;
//     struct argnode* next;
// }argNode;  //函数形参表结点

struct table1{
    int divitor;
 	int curSize;
    int tableSize;
    dataNodeVar* data;
    enum Status* sta;
};  //变量符号表

typedef struct table1 SymbolTableVar;

void tableVarInit(SymbolTableVar st){
    //st.divitor = d;
    st.curSize = 0;
    st.tableSize = TABLESIZE;
    st.divitor = GetClosestPrime(TABLESIZE);
    st.data = (dataNodeVar*)calloc(sz, sizeof(dataNodeVar));
    st.sta = (enum Status*)calloc(sz, sizeof(enum Status));
    for(int i = 0; i < st.tableSize; i++)
        st.sta[i] = (enum Status)Empty;
    return;
}

struct table2{
    int divitor;
 	int curSize;
    int tableSize;
    dataNodeFunc* data;
    enum Status* sta;
};  //函数符号表

typedef struct table2 SymbolTableFunc;

void tableFuncInit(SymbolTableFunc st){
    st.curSize = 0;
    st.tableSize = TABLESIZE;
    st.divitor = GetClosestPrime(TABLESIZE);
    st.data = (dataNodeFunc*)calloc(sz, sizeof(dataNodeFunc));
    st.sta = (enum Status*)calloc(sz, sizeof(enum Status));
    for(int i = 0; i < st.tableSize; i++)
        st.sta[i] = (enum Status)Empty;
    return;
}

struct table3{
    int divitor;
 	int curSize;
    int tableSize;
    dataNodeStruct* data;
    enum Status* sta;
};  //结构体符号表

typedef struct table3 SymbolTableStruct;

void tableStructInit(SymbolTableStruct st){
    st.curSize = 0;
    st.tableSize = TABLESIZE;
    st.divitor = GetClosestPrime(TABLESIZE);
    st.data = (dataNodeStruct*)calloc(sz, sizeof(dataNodeStruct));
    st.sta = (enum Status*)calloc(sz, sizeof(enum Status));
    for(int i = 0; i < st.tableSize; i++)
        st.sta[i] = (enum Status)Empty;
    return;
}

int findPosVar(SymbolTableVar st, char* key){
    int i = abs(key[1] + key[2]) % st.divitor;
    int j = i;
    do {
		if (st.sta[j] == Empty || (st.sta[j] == Active && key == st.data[j].varName))
			return j;
		j = (j + 1) % st.tableSize;
	} while (j != i);
    return j;
}

int findPosFunc(SymbolTableFunc st, char* key){
    int i = abs(key[1] + key[2]) % st.divitor;
    int j = i;
    do {
		if (st.sta[j] == Empty || (st.sta[j] == Active && key == st.data[j].funcName))
			return j;
		j = (j + 1) % st.tableSize;
	} while (j != i);
    return j;
}

int findPosStruct(SymbolTableStruct st, char* key){
    int i = abs(key[1] + key[2]) % st.divitor;
    int j = i;
    do {
		if (st.sta[j] == Empty || (st.sta[j] == Active && key == st.data[j].structName))
			return j;
		j = (j + 1) % st.tableSize;
	} while (j != i);
    return j;
}

int ifExistVar(SymbolTableVar st, char* key){
    int i = findPosVar(st, key);
    if(st.sta[i] == Active && st.data[i].varName == key)
        return 1;
    return 0;
}

int ifExistFunc(SymbolTableFunc st, char* key){
    int i = findPosFunc(st, key);
    if(st.sta[i] == Active && st.data[i].funcName == key)
        return 1;
    return 0;
}

int ifExistStruct(SymbolTableStruct st, char* key){
    int i = findPosStruct(st, key);
    if(st.sta[i] == Active && st.data[i].structName == key)
        return 1;
    return 0;
}

void InsertVar(SymbolTableVar st, dataNodeVar elem)
{
	int i = findPosVar(st, elem.varName);
	if (st.sta[i] != Active)
	{
		st.data[i] = elem;
		st.sta[i] = (enum Status)Active;
		st.curSize++;
	}
    else
        printf("Symbol table is full. Insert failed");
}

void InsertFunc(SymbolTableFunc st, dataNodeFunc elem)
{
	int i = findPosFunc(st, elem.funcName);
	if (st.sta[i] != Active)
	{
		st.data[i] = elem;
		st.sta[i] = (enum Status)Active;
		st.curSize++;
	}
    else
        printf("Symbol table is full. Insert failed");
}

void InsertStruct(SymbolTableStruct st, dataNodeStruct elem)
{
	int i = findPosStruct(st, elem.structName);
	if (st.sta[i] != Active)
	{
		st.data[i] = elem;
		st.sta[i] = (enum Status)Active;
		st.curSize++;
	}
    else
        printf("Symbol table is full. Insert failed");
}

//以下是作用域管理期望的接口

//创建变量表子域
//经过修改后，st_ptr指向新的子表
//如果需要的话，也可以这样给出新表：返回值可以是SymbolTableVar*，指向新的子表
//如果需要的话，输入变量可以再增加一个栈指针，来方便子域管理
void createVarSubtable(SymbolTableVar* st_ptr);

//关闭变量表子域
//经过修改后，st_ptr指向原来表的父一级表
//如果需要的话，也可以这样给出新表：返回值可以是SymbolTableVar*，指向原来的父表
//如果需要的话，输入变量可以再增加一个栈指针，来方便子域管理
//注意这个问题：如果已经返回到顶级父表了，就把输入的指针原样输出，printf一个warning
void closeVarSubtable(SymbolTableVar* st_ptr);

//创建函数表子域
//经过修改后，st_ptr指向新的子表
//如果需要的话，也可以这样给出新表：返回值可以是SymbolTableFunc*，指向新的子表
//如果需要的话，输入变量可以再增加一个栈指针，来方便子域管理
void createFuncSubtable(SymbolTableFunc* st_ptr);

//关闭函数表子域
//经过修改后，st_ptr指向原来表的父一级表
//如果需要的话，也可以这样给出新表：返回值可以是SymbolTableFunc*，指向原来的父表
//如果需要的话，输入变量可以再增加一个栈指针，来方便子域管理
//注意这个问题：如果已经返回到顶级父表了，就把输入的指针原样输出，printf一个warning
void closeFuncSubtable(SymbolTableFunc* st_ptr);

//创建结构体表子域
//经过修改后，st_ptr指向新的子表
//如果需要的话，也可以这样给出新表：返回值可以是SymbolTableStruct*，指向新的子表
//如果需要的话，输入变量可以再增加一个栈指针，来方便子域管理
void createStructSubtable(SymbolTableStruct* st_ptr);

//关闭函数表子域
//经过修改后，st_ptr指向原来表的父一级表
//如果需要的话，也可以这样给出新表：返回值可以是SymbolTableFunc*，指向原来的父表
//如果需要的话，输入变量可以再增加一个栈指针，来方便子域管理
//注意这个问题：如果已经返回到顶级父表了，就把输入的指针原样输出，printf一个warning
void closeStructSubtable(SymbolTableStruct* st_ptr);