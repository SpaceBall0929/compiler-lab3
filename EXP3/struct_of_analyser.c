#include <stdio.h>
#include "DomainStack.c"

//这里给了一个node_type定义，其实树的头里面已经给过了
//这里只是为了方便编译不报错，最后应该当删除
typedef enum node_type
{
    // NONTERMINAL
    N_PROGRAM,
    N_EXT_DEF_L,
    N_EXT_DEF,
    N_EXT_DEC_L,
    N_FUN_DEC,
    N_SPECI,
    N_STRUCT_SPECI,
    N_OPT_TAG,
    N_TAG,
    N_VAR_DEC,
    N_VAR_L,
    N_EXP,
    N_STMT,
    N_PARAM_DEC,
    N_COMPST,
    N_STMT_L,
    N_DEF_L,
    N_DEF,
    N_DEC,
    N_DEC_L,
    N_ARGS,

    //TERMINAL
    N_INT ,
    N_FLOAT = 100,
    N_ID,
    N_TYPE,
    N_LF,
    N_SEMI,
    N_COMMA,
    N_DOT,
    N_ASSIGNOP,
    N_RELOP,
    N_PLUS,
    N_MINUS,
    N_STAR,
    N_DIV,
    N_AND,
    N_OR,
    N_NOT,
    N_LP,
    N_RP,
    N_LB,
    N_RB,
    N_LC,
    N_RC,
    N_STRUCT,
    N_RETURN,
    N_IF,
    N_ELSE,
    N_WHILE
} node_type;

typedef struct node
{
    char *character;
    int line_no; //行号
    treeNode *child;
    treeNode *sibling;
    node_type nodeType;
    // int flag;//遍历标记
    union
    {
        int intVal;
        float floatVal;
        char *IDVal;
    } subtype;
} treeNode;

typedef treeNode Tree;

// enum Datanode_type {Int, Float, Array, Struct, StructDomain};

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

// 识别非终结符VarDec,收集变量的名字，收集是否是数组，返回一个初始化好的dataNodeVar
dataNodeVar* var_dec(treeNode* dec_node, enum DataType var_type){
    dataNodeVar* new_var;
    treeNode* origrn = dec_node;
    dec_node = dec_node -> child;
    int dimension = 0;
    int dimensionlen[10];
    while(dec_node -> nodeType != N_ID){
        dec_node = dec_node -> child;
        dimensionlen[dimension++] = dec_node -> sibling -> sibling -> subtype.intVal;
    }
    new_var = newNodeVar(dec_node->subtype.IDVal, var_type);
    if(dimension == 0){
        return new_var;
    }
    new_var -> numdim = dimension;
    new_var -> len_of_dims = (int*)malloc(sizeof(int) * dimension);
    for(int i = 0; i < dimension; i++){
        new_var -> len_of_dims[i] = dimensionlen[dimension - 1 - i];
    }
    return new_var;
}

dataNodeVar* param_dec(treeNode* para){
    treeNode* type_node = para -> child -> ;

}

dataNodeFunc* fun_dec(treeNode* dec_node, enum DataType return_type){

}

int ext_def(treeNode* ExtDef, seqStack* stack, stackNode* domain){
    enum DataType def_type;
    treeNode* type_node = ExtDef -> child -> child;
    treeNode* core_node = ExtDef -> child -> sibling;

    //判断一下这到底是个什么类型的声明
    if(type_node -> nodeType == N_TYPE){
        if(type_node -> subtype.IDVal[0] == 'i'){
            def_type = Int;
        }else{
            def_type = Float;
        }
        if(core_node -> nodeType == N_EXT_DEC_L){
            treeNode* temp_node;
            do{
                temp_node = core_node -> child;
                core_node = temp_node -> sibling -> sibling;
                InsertVar(&(domain->tVar), var_dec(temp_node, def_type));
            }while(core_node != NULL);
        }else{

        }

    }else{
        struct_specifier(type_node);
    }

       
    return 0;
}

int tree_analys(treeNode *mytree)
{
    
    //栈初始化部分
    treeNode* temp;
    seqStack myStack;
    seqStack* stack_ptr;
    stack_ptr = &myStack;
    initStack(stack_ptr);
    push(stack_ptr, mytree);

    //表初始化部分
    stackNode* domain_ptr = createStackNode();


    //用于存储变量信息
    int define_flag = 0;
    enum DataType type_now;

    do
    {
        reversed_insert(stack_ptr, pop(stack_ptr));
        temp = top(stack_ptr);
        //根据收到的不同符号调用不同的处理函数
        switch (temp->nodeType)
        {
        //这里涉及的一系列节点都是不需要做特殊处理的，接着pop就好
        case N_EXT_DEF_L:
            printf("ExtDefList detected\n");
            // if(temp->child->sibling->sibling != NULL){
            //     printf("ERROR: Unexpected 3rd child in childs of the NONTERMINAL ExtDefList.\n");
            // }
            break;
        case N_EXT_DEF:
            printf("ExtDef detected\n");
            ext_def(temp, stack_ptr, domain_ptr);
            break;
        case N_SPECI:


            break;


        default:
            // 这里给出一个列表：
            // ExtDecList
            // 这些非终结符，理论上应该在函数中被处理掉
            // 但是既然走到了这一步，显然没有，所以这里肯定要报错
            printf("ERROR: Unexpected node token with character %s\n", temp->character);

            break;
        }
        //节点处理完了，下一个
        
    } while (1);

    //后续的程序等等...先不写了，我也不清楚
}




// int ext_def(var_dec *out0, fun_dec *out1, struct_dec *out2)
// {
//     treeNode sub_tree_root;
//     treeNode child = sub_tree_root->child->sibling;
//     switch (child->node_type)
//     {
//     case N_EXT_DEF_L:
//         out0 = global_var(sub_tree_root);
//         return 0;
//         break;

//     case N_SEMI:
//         out1 = global_fun(sub_tree_root);
//         return 1;
//         break;

//     case N_FUN_DEC:
//         out2 = global_struct(sub_tree_root);
//         return 2;
//     default:
//         printf("ERROR: wrong syntax tree\n");
//         break;
//     }
// }


int specifiers(){

}


/**************************
*       以下施工中……         *
***************************/
#include"tree.c"//不想看报错
void error_msg(int type, int line_no, char* content){//报错
    printf("Error type %d at Line %d: ", type, line_no);
    switch(type){
        case 1:
            printf("Undefined var \"%s\".\n", content);
            break;
        case 2:
            printf("Undefined function \"%s\".\n", content);
            break;
        case 3:
            printf("Redefined var \"%s\".\n", content);
            break;
        case 4:
            printf("Redefined function \"%s\".\n", content);
            break;
        case 5:
            printf("Type mismatched for assigment.\n");
            break;
        case 6:
            printf("The left-hand side of an assignment must be a var.\n");
            break;
        case 7:
            printf("Type mismatched for operands.\n");
            break;
        case 8:
            printf("Type mismatched for return.\n");
            break;
        case 9:
            printf("Function is not applicable for arguments.\n");
            break;
        case 10:
            printf("This is not an array.\n");
            break;
        case 11:
            printf("\"%s\" is not a function.\n", content);
            break;
        case 12:
            printf("This is not an integer.\n");
            break;
        case 13:
            printf("Illegal use of \".\".\n");
            break;
        case 14:
            printf("Non-existent field \"%s\".\n", content);
            break;
        case 15:
            printf("Redefined field \"%s\".\n", content);
            break;
        case 16:
            printf("Duplicated name \"%s\".\n", content);
            break;
        case 17:
            printf("Undefined structure \"%s\".\n", content);
            break;
        case 18:
            printf("Undefined function \"%s\".\n", content);
            break;
        case 19:
            printf("Inconsistent declaration of function \"%s\".\n", content);
            break;
        default:
            printf("Wrong semantic type:%s\n", content);
            //
            break;
        }
}


node_type Exp_s(treeNode*exp)
{
	/*Exp -> Exp ASSIGNOP Exp3
	| Exp AND Exp3
	| Exp OR Exp3
	| Exp RELOP Exp3
 	| Exp PLUS Exp3
	| Exp MINUS Exp3
	| Exp STAR Exp3
	| Exp DIV Exp3

	| LP Exp RP3  
	| MINUS Exp 2 
	| NOT Exp 2 

	| ID LP Args RP 4函数
	| ID LP RP 3

	| Exp LB Exp RB4 数组
	| Exp DOT ID3 结构体;

	| ID1 
	| INT1 
	| FLOAT1 
	*/
	if(exp==NULL){return NULL;};
	node_type result=NULL;

	treeNode*tempnode1=getchild(exp, 0);
	treeNode*tempnode2=getchild(exp, 1);

	//ID, EXP DOT ID(结构体), Exp LB Exp RB (数组)
	if(strcmp(tempnode1->character,"Exp")==0)
    {
		if(tempnode2!=NULL&&strcmp(tempnode2->character,"ASSIGNOP")==0)
        {	//Exp ASSIGNOP Exp
			treeNode*tempnode11=getchild(tempnode1,0);
			treeNode*tempnode12=getchild(tempnode1,1);
			if(tempnode12==NULL)
            {
				if(strcmp(tempnode11->character,"ID")!=0)
                {	//左侧不是ID
					error_msg(6,exp->line_no,NULL);	//报错
					return NULL;
				}
		}
            else
            {
				treeNode* tempnode13=getchild(tempnode1,2);
				if(tempnode13!=NULL)
                {
					treeNode* tempnode14=getchild(tempnode1,3);
					if(tempnode14==NULL)
                    {//Exp DOT ID(结构体)
						if(strcmp(tempnode11->character,"Exp")==0&&
                           strcmp(tempnode12->character,"DOT")==0&&
                           strcmp(tempnode13->character,"ID")==0)
                        {
							;//正确
						}else
                            {//报错
							error_msg(6,exp->line_no,NULL);	
							return NULL;
						    }
					}else
                    {//EXP LB EXP RB (数组)
						if(strcmp(tempnode11->character,"Exp")==0&&
                           strcmp(tempnode12->character,"LB")==0&&
                           strcmp(tempnode13->character,"Exp")==0&&
                           strcmp(tempnode14->character,"RB")==0)
                           {
							;//正确
						    }else
                            {//报错
							error_msg(6,exp->line_no,NULL);	
							return NULL;
						    }

					}
				}else
                {//tempnode13==NULL 报错
					error_msg(6,exp->line_no,NULL);
					return NULL;
				}
			}
		}
	}

    //未完……
}