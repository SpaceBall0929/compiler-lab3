# include "SymbolTable.c"

struct node
{
    struct node* next;
    struct node* last;
    /* data */
    SymbolTableVar tVar;
    SymbolTableFunc tFunc;
    SymbolTableStruct tStruct;
};
typedef struct node stackNode;
typedef struct node* domainStack;

stackNode* createStackNode(){
    stackNode* newNode = (stackNode*)malloc(stackNode);
    newNode->last = (stackNode*)malloc(stackNode);
    newNode->next = (stackNode*)malloc(stackNode);
    tableVarInit(newNode->tVar);
    tableFuncInit(newNode->tFunc);
    tableStructInit(newNode->tStruct);
    return newNode;
}

//创建新作用域并开启
domainStack domainPush(domainStack ds){
    stackNode* newNode = createStackNode();
    if(ds == NULL){
        ds = newNode;
        //建立第一个作用域时node->last设置为NULL
        ds->last = NULL;
        ds->next = NULL;
    }
    else{
        newNode->last = ds;
        ds->next = newNode;
        newNode->next = NULL;
        ds = newNode;
    }
    return ds;
}

//关闭当前作用域并销毁，退回到上一级作用域
domainStack domainPop(domainStack ds){
    stackNode* p = ds;
    ds = ds->last;
    ds->next = NULL;
    free(p);
    return ds;
}

//对未定义问题，查找所有各层作用域是否存在（重定义问题直接调用ifExistVar(ds->tVar, key)）
//查找变量
//建立第一个作用域时node->last设置为NULL
int ifExistVarStack(domainStack ds, char* key){
    domainStack p = ds;
    while(p != NULL){
        if(ifExistVar(p->tVar, key))
            return 1;
        p = p->last;
    }
    return 0;
}

//查找函数
int ifExistFuncStack(domainStack ds, char* key){
    domainStack p = ds;
    while(p != NULL){
        if(ifExistFunc(p->tFunc, key))
            return 1;
        p = p->last;
    }
    return 0;
}

//查找结构体
int ifExistStructStack(domainStack ds, char* key){
    domainStack p = ds;
    while(p != NULL){
        if(ifExistStruct(p->tStruct, key))
            return 1;
        p = p->last;
    }
    return 0;
}