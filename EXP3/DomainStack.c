# include "SymbolTable.c"

struct stacknode
{
    struct stacknode* next;
    struct stacknode* last;
    /* data */
    SymbolTableVar tVar;  //只对变量做作用域管理
    //SymbolTableFunc tFunc;  //函数和结构体类型需要两张全局的表
    //SymbolTableStruct tStruct;
};
typedef struct stacknode stackNode;
typedef struct stacknode* domainStack;

stackNode* createStackNode(){
    stackNode* newNode = (stackNode*)malloc(sizeof(stackNode));
    newNode->last = NULL;
    newNode->next = NULL;
    newNode->tVar = *tableVarInit();
    //tableFuncInit(&newNode->tFunc);
    //tableStructInit(&newNode->tStruct);
    return newNode;
}

//创建新作用域并开启
domainStack domainPush(domainStack ds){
    stackNode* newNode = createStackNode();
    if(ds == NULL)
        ds = newNode;
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
    if(ds != NULL)
    	ds->next = NULL;
    if(p != NULL)
    	free(p);
    return ds;
}

//对未定义问题，查找所有各层作用域是否存在（重定义问题直接调用ifExistVar(ds->tVar, key)）
//查找变量
//建立第一个作用域时node->last设置为NULL
int ifExistVarStack(domainStack ds, char* key){  //key为变量名
    domainStack p = ds;
    while(p != NULL){
        if(ifExistVar(p->tVar, key))
            return 1;
        p = p->last;
    }
    return 0;
}
