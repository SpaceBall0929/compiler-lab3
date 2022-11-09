# include "tree.c"
# include "AssistantFuncs.c"
# include <math.h>
# include <string.h>
# define TABLESIZE 128

enum Status { Active, Empty, Deleted };
//enum DataType {Int, Float, Array, Struct, StructDomain};

struct datanode1{
    char* varName;  //变量名
    /*结构体类型名不能穷举，改成用字符串表示*/
    //对结构体变量：varType = "struct a"
    char* varType; //变量数据类型
    int numdim;     //若为数组，数组维度
    int* len_of_dims; //每个维度的长度
    struct datanode1* next;
};  //变量符号表结点

typedef struct datanode1 dataNodeVar;

dataNodeVar* newNodeVar(char* name, char* type){
    dataNodeVar* newNode;
    newNode = (dataNodeVar*)malloc(sizeof(dataNodeVar));
    newNode -> varName = name;
    newNode -> varType = type;
    newNode -> len_of_dims = NULL;
    newNode -> next = NULL;
     if(type == Array){
        newNode ->numdim = -1;
        return newNode;
    }
    newNode -> numdim = 0;
    return newNode;
}

struct datanode2{
    char* funcName;  //函数名
    char* returnType; //函数返回值类型
    int defined;   //函数是否定义
    dataNodeVar* args;   //函数形参表
};  //函数符号表结点

typedef struct datanode2 dataNodeFunc;

dataNodeFunc* newNodeFunc(char* name, char* type, int de, dataNodeVar* ar){
    dataNodeFunc* newNode = (dataNodeFunc*)malloc(sizeof(dataNodeFunc));
    newNode -> funcName = name;
    newNode -> returnType = type;
    newNode -> defined = de;
    newNode -> args = ar;
    return newNode;
}

struct datanode3{
    //char* structName;  //结构体名
    char* structTypeName;  //结构体类型名
    dataNodeVar* structDomains;  //结构体域，为一系列变量，存储为一个变量结点链表
};  //结构体符号表结点

typedef struct datanode3 dataNodeStruct;

dataNodeStruct* newNodeStruct(char* tname){
    dataNodeStruct* newNode = (dataNodeStruct*)malloc(sizeof(dataNodeStruct));
    //newNode -> structName = name;
    newNode -> structTypeName = tname;  //这里也传入“struct a”形式，和变量表保持一致
    newnode -> structDomains = NULL;
    return newNode;
}

//填结构体表时，用前插法填入结构体的域
void insertStructDomain(dataNodeStruct* structNode, dataNodeVar* newDomain){
    if(structNode->structDomains == NULL)
        structNode->structDomains = newDomain;
    else{
        //前插
        newDomain->next = structNode->structDomains;
        structNode->structDomains = newDomain;
    }
    return;
}

struct table1{
    int divitor;
 	int curSize;
    int tableSize;
    dataNodeVar* data;
    enum Status* sta;
};  //变量符号表

typedef struct table1 SymbolTableVar;

void tableVarInit(SymbolTableVar* st){
    //st.divitor = d;
    st->curSize = 0;
    st->curSize = TABLESIZE;
    st->divitor = GetClosestPrime(TABLESIZE);
    st->data = (dataNodeVar*)calloc(TABLESIZE, sizeof(dataNodeVar));
    st->sta = (enum Status*)calloc(TABLESIZE, sizeof(enum Status));
    for(int i = 0; i < st->tableSize; i++)
        st->sta[i] = (enum Status)Empty;
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

void tableFuncInit(SymbolTableFunc* st){
    st->curSize = 0;
    st->tableSize = TABLESIZE;
    st->divitor = GetClosestPrime(TABLESIZE);
    st->data = (dataNodeFunc*)calloc(TABLESIZE, sizeof(dataNodeFunc));
    st->sta = (enum Status*)calloc(TABLESIZE, sizeof(enum Status));
    for(int i = 0; i < st->tableSize; i++)
        st->sta[i] = (enum Status)Empty;
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

void tableStructInit(SymbolTableStruct* st){
    st->curSize = 0;
    st->tableSize = TABLESIZE;
    st->divitor = GetClosestPrime(TABLESIZE);
    st->data = (dataNodeStruct*)calloc(TABLESIZE, sizeof(dataNodeStruct));
    st->sta = (enum Status*)calloc(TABLESIZE, sizeof(enum Status));
    for(int i = 0; i < st->tableSize; i++)
        st->sta[i] = (enum Status)Empty;
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
    if(st.sta[i] == Active && st.data[i].structTypeName == key)
        return 1;
    return 0;
}

//判断某结构体类型中是否存在某个域
int ifExistStructDomain(SymbolTableStruct st, char* key, char* domainName){
    int i = findPosStruct(st, key);
    //结构体类型存在
    dataNodeVar* p = st.data[i].structDomains;
    while (p != NULL)
    {
        if(p->varName == domainName)
            //结构体域存在
            return 1;
        p = p->next;
    }
    //结构体域不存在
    return 0;
}

void InsertVar(SymbolTableVar* st, dataNodeVar* elem)
{
	int i = findPosVar(*st, elem -> varName);
	if (st->sta[i] != Active)
	{
		st->data[i] = *elem;
		st->sta[i] = (enum Status)Active;
		st->curSize++;
	}
    else
        printf("Symbol table is full. Insert failed");
    free(elem);

}

void InsertFunc(SymbolTableFunc* st, dataNodeFunc elem)
{
	int i = findPosFunc(*st, elem.funcName);
	if (st->sta[i] != Active)
	{
		st->data[i] = elem;
		st->sta[i] = (enum Status)Active;
		st->curSize++;
	}
    else
        printf("Symbol table is full. Insert failed");
}

void InsertStruct(SymbolTableStruct* st, dataNodeStruct elem)
{
	int i = findPosStruct(*st, elem.structName);
	if (st->sta[i] != Active)
	{
		st->data[i] = elem;
		st->sta[i] = (enum Status)Active;
		st->curSize++;
	}
    else
        printf("Symbol table is full. Insert failed");
}
