# include "tree.c"
# include "AssistantFuncs.c"
# include <math.h>
# include <stdlib.h>
# include <string.h>
# define TABLESIZE 128
# define D_INT 0
# define D_FLOAT 1
# define D_ARRAY 2
# define D_STRUCT_DEC 3
# define D_STRUCT_DEF 4
# define D_FUNC 5
# define D_AMT 6

enum Status { Active, Empty, Deleted };
//enum DataType {Int, Float, Array, Struct, StructDomain};

struct datanode1{
    char* varName;  //变量名
    /*结构体类型名不能穷举，改成用字符串表示*/
    int varType; //变量数据类型
    int arrayVarType; //数组存储的变量数据类型
    int numdim;     //若为数组，数组维度
    int* len_of_dims; //每个维度的长度
    struct datanode1* next;
};  //变量符号表结点

typedef struct datanode1 dataNodeVar;

struct datanode2{
    char* funcName;  //函数名
    int returnType; //函数返回值类型
    int defined;   //函数是否定义
    dataNodeVar* args;   //函数形参表
};  //函数符号表结点

typedef struct datanode2 dataNodeFunc;

struct datanode3{
    //char* structName;  //结构体名
    char* structTypeName;  //结构体类型名
    dataNodeVar* structDomains;  //结构体域，为一系列变量，存储为一个变量结点链表
};  //结构体符号表结点

typedef struct datanode3 dataNodeStruct;

struct table1{
    int divitor;
 	int curSize;
    int tableSize;
    dataNodeVar* data;
    enum Status* sta;
};  //变量符号表

typedef struct table1 SymbolTableVar;

struct table2{
    int divitor;
 	int curSize;
    int tableSize;
    dataNodeFunc* data;
    enum Status* sta;
};  //函数符号表

typedef struct table2 SymbolTableFunc;

struct table3{
    int divitor;
 	int curSize;
    int tableSize;
    dataNodeStruct* data;
    enum Status* sta;
};  //结构体符号表

typedef struct table3 SymbolTableStruct;

int ifExistStructDomain(SymbolTableStruct st, int type, char* domainName);
void deepcopyVarComplete(dataNodeVar* varnode1, dataNodeVar* varnode2);

int findPosVar(SymbolTableVar st, char* key){
    int i = abs(key[0]) % st.divitor;
    int j = i;
    do {
		if (st.sta[j] == Empty || (st.sta[j] == Active && strcmp(key, st.data[j].varName) == 0))
			return j;
		j = (j + 1) % st.tableSize;
	} while (j != i);
    return j;
}

int findPosFunc(SymbolTableFunc st, char* key){
    int i = abs(key[0]) % st.divitor;
    int j = i;
    do {
		if (st.sta[j] == Empty || (st.sta[j] == Active && strcmp(key, st.data[j].funcName) == 0))
			return j;
		j = (j + 1) % st.tableSize;
	} while (j != i);
    return j;
}

int findPosStruct(SymbolTableStruct st, char* key){
    int i = abs(key[0]) % st.divitor;
    int j = i;
    do {
		if (st.sta[j] == Empty || (st.sta[j] == Active && strcmp(key, st.data[j].structTypeName) == 0))
			return j;
		j = (j + 1) % st.tableSize;
	} while (j != i);
    return j;
}

int charToInt(char* type, SymbolTableStruct st){
    if(strcmp(type,"int") == 0)
        return D_INT;
    else if(strcmp(type,"float") == 0)
        return D_FLOAT;
    else if(strcmp(type,"array") == 0)
        return D_ARRAY;
    else{
        int j = findPosStruct(st, type);
        if(st.sta[j] == Active && strcmp(st.data[j].structTypeName, type))
            return j + D_AMT;
        else
            return -1;
    }
}

dataNodeVar* newNodeVar(char* name, int type){
    dataNodeVar* newNode;
    newNode = (dataNodeVar*)malloc(sizeof(dataNodeVar));
    newNode -> varName = name;
    newNode -> varType = type;
    newNode -> numdim = -1;
    newNode -> len_of_dims = NULL;
    newNode -> next = NULL;
    newNode -> numdim = 0;
    return newNode;
}

//执行数组相关的初始化
//根据具体需求再调整
// void initArray(dataNodeVar* node, int atype, int nd, int* lod){
//     node->arrayVarType = atype;
//     node->numdim = nd;
//     node->len_of_dims = lod;
//     return;
// }

dataNodeFunc* newNodeFunc(char* name, int type, int de, dataNodeVar* ar){
    dataNodeFunc* newNode = (dataNodeFunc*)malloc(sizeof(dataNodeFunc));
    newNode -> funcName = name;
    newNode -> returnType = type;
    newNode -> defined = de;
    newNode -> args = ar;
    return newNode;
}

dataNodeStruct* newNodeStruct(char* tname){
    dataNodeStruct* newNode = (dataNodeStruct*)malloc(sizeof(dataNodeStruct));
    //newNode -> structName = name;
    newNode -> structTypeName = tname;  //这里也传入“struct a”形式，和变量表保持一致
    newNode -> structDomains = NULL;
    return newNode;
}

//填结构体表时，用前插法填入结构体的域
void insertStructDomain(dataNodeStruct* structNode, dataNodeVar* input, SymbolTableStruct st, int line_no){
    dataNodeVar* newDomain = (dataNodeVar*)malloc(sizeof(dataNodeVar));
    deepcopyVarComplete(newDomain, input);
    if(structNode->structDomains == NULL){
        structNode->structDomains = newDomain;
        newDomain->next = NULL;
    }
    else{
        //判断结构体域是否重定义
        int type = charToInt(structNode->structTypeName, st);
        if(ifExistStructDomain(st, type, newDomain->varName)){
            error_msg(15, line_no, newDomain->varName);
            return;
        }
        //前插
        newDomain->next = structNode->structDomains;
        structNode->structDomains = newDomain;
    }
}

SymbolTableVar* tableVarInit(){
    //st.divitor = d;
    SymbolTableVar* st = (SymbolTableVar*)malloc(sizeof(SymbolTableVar));
    st->curSize = 0;
    st->tableSize = TABLESIZE;
    st->divitor = GetClosestPrime(TABLESIZE);
    st->data = (dataNodeVar*)calloc(TABLESIZE, sizeof(dataNodeVar));
    st->sta = (enum Status*)calloc(TABLESIZE, sizeof(enum Status));
    for(int i = 0; i < st->tableSize; i++)
        st->sta[i] = (enum Status)Empty;
    return st;
}

SymbolTableFunc* tableFuncInit(){
    SymbolTableFunc* st = (SymbolTableFunc*)malloc(sizeof(SymbolTableFunc));
    st->curSize = 0;
    st->tableSize = TABLESIZE;
    st->divitor = GetClosestPrime(TABLESIZE);
    st->data = (dataNodeFunc*)calloc(TABLESIZE, sizeof(dataNodeFunc));
    st->sta = (enum Status*)calloc(TABLESIZE, sizeof(enum Status));
    for(int i = 0; i < st->tableSize; i++)
        st->sta[i] = (enum Status)Empty;
    return st;
}

SymbolTableStruct* tableStructInit(){
    SymbolTableStruct* st = (SymbolTableStruct*)malloc(sizeof(SymbolTableStruct));
    st->curSize = 0;
    st->tableSize = TABLESIZE;
    st->divitor = GetClosestPrime(TABLESIZE);
    st->data = (dataNodeStruct*)calloc(TABLESIZE, sizeof(dataNodeStruct));
    st->sta = (enum Status*)calloc(TABLESIZE, sizeof(enum Status));
    for(int i = 0; i < st->tableSize; i++)
        st->sta[i] = (enum Status)Empty;
    return st;
}

int ifExistVar(SymbolTableVar st, char* key){
    int i = findPosVar(st, key);
    if(st.sta[i] == Active && strcmp(st.data[i].varName, key) == 0)
        return 1;
    return 0;
}

int ifExistFunc(SymbolTableFunc st, char* key){
    int i = findPosFunc(st, key);
        if(st.sta[i] == Active && strcmp(st.data[i].funcName, key) == 0)
        return 1;
    return 0;
}

int ifExistStruct(SymbolTableStruct st, char* key){
    int i = findPosStruct(st, key);
    if(st.sta[i] == Active && strcmp(st.data[i].structTypeName, key) == 0)
        return 1;
    return 0;
}

//判断某结构体类型中是否存在某个域
//type -- 结构体类型，传入之前要求判断type>=3，否则报错误13
//type通过查变量表中变量的类型获得
int ifExistStructDomain(SymbolTableStruct st, int type, char* domainName){
    //int i = findPosStruct(st, key);
    dataNodeVar* p = st.data[type-D_AMT].structDomains;
    while (p != NULL)
    {
        if(strcmp(p->varName, domainName) == 0)
            //结构体域存在
            return 1;
        p = p->next;
    }
    //结构体域不存在
    return 0;
}

void deepcopyVar(dataNodeVar* varnode1, dataNodeVar* varnode2){
    if(varnode1 == NULL || varnode2 == NULL)
        return;
    varnode1->varType = varnode2->varType;
    varnode1->numdim = varnode2->numdim;
    varnode1->next = NULL;
    varnode1->varName = (char*)malloc(sizeof(char) * strlen(varnode2->varName));
    strcpy(varnode1->varName, varnode2->varName);
    if(varnode2->len_of_dims != NULL){
        varnode1->len_of_dims = (int*)malloc(sizeof(int) * varnode2->numdim);
        for(int i = 0;i < varnode2->numdim;i++)
            varnode1->len_of_dims[i] = varnode2->len_of_dims[i];
    }
}

void deepcopyVarComplete(dataNodeVar* varnode1, dataNodeVar* varnode2){
    dataNodeVar* p1 = varnode1;
    dataNodeVar* p2 = varnode2;
    while(p1 != NULL && p2 != NULL){
        deepcopyVar(p1, p2);
        if(p2->next != NULL)
            p1->next = (dataNodeVar*)malloc(sizeof(dataNodeVar));
        else
            p1->next = NULL;
        p1 = p1->next;
        p2 = p2->next;
    }
}

void deepcopyFunc(dataNodeFunc* funcnode1, dataNodeFunc* funcnode2){
    funcnode1->returnType = funcnode2->returnType;
    funcnode1->defined = funcnode2->defined;
    funcnode1->funcName = (char*)malloc(sizeof(char) * strlen(funcnode2->funcName));
    strcpy(funcnode1->funcName, funcnode2->funcName);
    if(funcnode2 -> args != NULL){
        funcnode1->args = (dataNodeVar*)malloc(sizeof(dataNodeVar));
        deepcopyVarComplete(funcnode1->args, funcnode2->args);
    }
    return;
}

void deepcopyStruct(dataNodeStruct* structnode1, dataNodeStruct* structnode2){
    structnode1->structTypeName = (char*)malloc(sizeof(char) * strlen(structnode2->structTypeName));
    strcpy(structnode1->structTypeName, structnode2->structTypeName);
    if(structnode2->structDomains != NULL){
        structnode1->structDomains = (dataNodeVar*)malloc(sizeof(dataNodeVar));
        deepcopyVarComplete(structnode1->structDomains, structnode2->structDomains);
    }
    return;
}

//注意：传参时传入行号和函数表
void InsertVar(SymbolTableVar* st1, SymbolTableFunc* st2, dataNodeVar* elem, int line_no)
{
	if(ifExistVar(*st1, elem->varName)){  //在变量表中查找变量名，若存在则说明已经声明，报变量重定义
		error_msg(3, line_no, elem->varName);
		return;
	}
    if(ifExistFunc(*st2, elem->varName)){  //在函数表中查找变量名，若存在则说明已经声明过同名函数，报函数声明冲突
		error_msg(19, line_no, elem->varName);
		return;
	}
	int i = findPosVar(*st1, elem -> varName);
	if (st1->sta[i] != Active)
	{
		deepcopyVarComplete(&st1->data[i], elem);
		st1->sta[i] = (enum Status)Active;
		st1->curSize++;
	}
    else
        //printf("Var symbol table is full. Insert failed");
    error_msg(3, line_no, elem->varName);
    //free(elem);

}

void InsertFunc(SymbolTableVar* st1, SymbolTableFunc* st2, dataNodeFunc* elem, int line_no)
{
	if(ifExistVar(*st1, elem->funcName)){  //在变量表中查找变量名，若存在则说明已经声明，报变量重定义
		error_msg(3, line_no, elem->funcName);
		return;
	}
    if(ifExistFunc(*st2, elem->funcName)){  //在函数表中查找变量名，若存在则说明已经声明过同名函数，报函数声明冲突
		error_msg(4, line_no, elem->funcName);
		return;
	}
    int i = findPosFunc(*st2, elem->funcName);
	if (st2->sta[i] != Active)
	{
		deepcopyFunc(&st2->data[i], elem);
        
		st2->sta[i] = (enum Status)Active;
		st2->curSize++;
	}
    else
        printf("Func symbol table is full. Insert failed");
}

int InsertStruct(SymbolTableStruct* st, dataNodeStruct* elem, int line_no)
{
	if(ifExistStruct(*st, elem->structTypeName)){  //在函数表中查找变量名，若存在则说明已经声明过同名函数，报函数声明冲突
		error_msg(16, line_no, elem->structTypeName);
		return -1;
	}
    int i = findPosStruct(*st, elem->structTypeName);
	if (st->sta[i] != Active)
	{
		deepcopyStruct(&st->data[i], elem);
		st->sta[i] = (enum Status)Active;
		st->curSize++;
        return i;
	}
    else{
        printf("Struct symbol table is full. Insert failed");
        return -1;
    }
}


dataNodeVar getNodeVar(SymbolTableVar st, char* key){
    int i = findPosVar(st, key);
    return st.data[i];
}

dataNodeFunc getNodeFunc(SymbolTableFunc st, char* key){
    int i = findPosFunc(st, key);
    return st.data[i];
}

dataNodeStruct getNodeStruct(SymbolTableStruct st, char* key){
    int i = findPosStruct(st, key);
    return st.data[i];
}

int getArgNum(SymbolTableFunc st, char* key){
    int num = 0;
    int i = findPosFunc(st, key);
    dataNodeVar* arg = st.data[i].args;
    while(arg != NULL){
        num++;
        arg = arg->next;
    }
    return num;
}

//返回结构体域数
int getFieldNum(dataNodeStruct s){
    dataNodeVar* a = s.structDomains;
    int num = 0;
    while(a != NULL){
        num += 1;
        a = a->next;
    }
    return num;
}

//释放一个变量信息占据的空间
int free_var(dataNodeVar* to_del){
    dataNodeVar* origin = to_del;
    // int i = 0;
    do{
        to_del = to_del->next;
        // printf("free success for %d\n", i++);
        free(origin->varName);
        if(origin->len_of_dims != NULL){
            free(origin->len_of_dims);
        }
        free(origin);
        origin = to_del;
    }while(to_del != NULL);

    return 0;
}

int free_func(dataNodeFunc* to_del){
    if(to_del->args != NULL)
    	free_var(to_del->args);
    if(to_del != NULL){
    	free(to_del->funcName);
        free(to_del);
    }
    return 0;
}

int free_struct(dataNodeStruct* to_del){
    if(to_del->structDomains != NULL)
    	free_var(to_del->structDomains);
    if(to_del != NULL){
        free(to_del->structTypeName);
        free(to_del);
    }
    return 0;
}

void typeCount(dataNodeVar* varnode, int* num_of_types){
while(varnode != NULL){
    num_of_types[varnode->varType] += 1;
    varnode = varnode->next;
}
}

int ifStructEquivalent(SymbolTableStruct st, int struct1, int struct2){
    //先assume都存在再说...
    dataNodeVar* domain1 = st.data[struct1 - D_AMT].structDomains;
    dataNodeVar* domain2 = st.data[struct2 - D_AMT].structDomains;
    //int num_of_struct_types = st.curSize;  //结构体类型数
    int* num_of_types1 = (int*)malloc((D_AMT + TABLESIZE) * sizeof(int));  //存储每种类型的结构体成员个数
    int* num_of_types2 = (int*)malloc((D_AMT + TABLESIZE) * sizeof(int));
    //所有类型的成员个数初始化为0
    for(int j = 0; j < D_AMT + TABLESIZE; j++){
        num_of_types1[j] = 0;
        num_of_types2[j] = 0;
    }
    //数类型个数
    typeCount(domain1, num_of_types1);
    typeCount(domain2, num_of_types2);
    //判断结构体是否等价
    for(int j = 0; j < D_AMT + TABLESIZE; j++){
        if(num_of_types1[j] != num_of_types2[j])
            return 0;
    }
    return 1;
}
