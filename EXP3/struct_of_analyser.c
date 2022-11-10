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
//test
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
dataNodeVar* var_dec(treeNode* dec_node, int var_type){
    dataNodeVar* new_var;
    treeNode* origrn = dec_node;
    dec_node = dec_node -> child;
    int dimension = 0;
    int dimensionlen[10];
    while(dec_node -> nodeType != N_ID){
        dec_node = dec_node -> child;
        dimensionlen[dimension++] = dec_node -> sibling -> sibling -> subtype.intVal;
    }
    new_var = newNodeVar(dec_node -> subtype.IDVal, var_type);
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


//函数未完工
dataNodeVar* var_list(treeNode* arg_list){
    treeNode* type_node = arg_list -> child;

}

//处理FunDec
dataNodeFunc* fun_dec(treeNode* dec_node, int return_type){
    treeNode* temp_node = dec_node -> child -> sibling -> sibling;
    dataNodeVar* arg_list = NULL;
    if(temp_node -> nodeType == N_VAR_L){
        arg_list = var_list(temp_node);
    }
    return newNodeFunc(dec_node -> child -> character, return_type, arg_list);
}

//判定specifier的指向：整形/浮点/结构体定义/结构体使用
int specifier(treeNode* speci){
    switch (speci -> child -> nodeType)
    {
    case N_TYPE:
        if(speci -> child -> subtype.IDVal[0] == 'i'){
            return D_INT;
        }
        return D_FLOAT;
        
        break;
    
    case N_STRUCT_SPECI:
        if(speci -> child -> child -> sibling -> nodeType == N_TAG){
            return D_STRUCT_DEC;
        }
        return D_STRUCT_DEF;
    default:
        printf("ERROR: Unexpected nodeType in the child of Specifier\n");
        return 0;
        break;
    }
}

int ext_def(treeNode* ExtDef, seqStack* stack, stackNode* domain){
    
    treeNode* type_node = ExtDef -> child -> child;
    treeNode* core_node = ExtDef -> child -> sibling;
    int def_type = specifier(type_node);


    switch (def_type)
    {
    case D_INT:
    case D_FLOAT:
        if(core_node -> nodeType == N_EXT_DEC_L){
            treeNode* temp_node;
            do{
                temp_node = core_node -> child;
                core_node = temp_node -> sibling -> sibling;
                InsertVar(&(domain->tVar), var_dec(temp_node, def_type));
            }while(core_node != NULL);
        }else{
            fun_dec();
        }


        break;
    case D_STRUCT_DEF:
        struct_specifier_def(type_node);
        break;

    case D_STRUCT_DEC:
        struct_specifier_dec(type_node);
        break;


    default:
        break;
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
    int type_now;

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


/**************************
*       以下施工中……         *
***************************/
#include"tree.c"//不想看报错
void error_msg(int type, int line_no, char* content);
int StmtList_s(treeNode*stmt, stackNode* domain, enum DataType d_type);
int Stmt_s(treeNode*stmt, stackNode* domain, enum DataType d_type);


int Exp_s(treeNode*exp)
{//处理Exp
	/*Exp -> 
      Exp ASSIGNOP Exp
	| Exp AND Exp
	| Exp OR Exp
	| Exp RELOP Exp
 	| Exp PLUS Exp
	| Exp MINUS Exp
	| Exp STAR Exp
	| Exp DIV Exp

	| LP Exp RP
	| MINUS Exp
	| NOT Exp 

	| ID LP Args RP 函数
	| ID LP RP 

	| Exp LB Exp RB 数组
	| Exp DOT ID 结构体

	| ID
	| INT
	| FLOAT
	*/
	if(exp==NULL){return NULL;};
	node_type result=NULL;

	treeNode*tempnode1=getchild(exp, 0);
	treeNode*tempnode2=getchild(exp, 1);

	//ID, EXP DOT ID(结构体), Exp LB Exp RB (数组)
	if(strcmp(tempnode1->character,"Exp")==0)
    {
		if(tempnode2!=NULL&&strcmp(tempnode2->character, "ASSIGNOP")==0)
        {	//Exp ASSIGNOP Exp
			treeNode*tempnode11=getchild(tempnode1, 0);
			treeNode*tempnode12=getchild(tempnode1, 1);
			if(tempnode12==NULL)
            {
				if(strcmp(tempnode11->character,"ID")!=0)
                {	//左侧不是ID
					error_msg(6, exp->line_no, NULL);	//报错
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
							error_msg(6, exp->line_no, NULL);	
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
							error_msg(6, exp->line_no, NULL);	
							return NULL;
						    }

					}
				}else
                {//tempnode13==NULL 报错
					error_msg(6, exp->line_no, NULL);
					return NULL;
				}
			}
		}
	}

    /***************************************/
	if(tempnode2 == NULL)
    { //ID，INT，FLOAT
		if(strcmp(tempnode1->character,"ID") == 0)
        {	//检查该ID是否已定义  (local & global)
			if(/*****被定义*/1)
            {
				;//****当前层找到了用当前的;是普通变量,
				result = tempnode1->nodeType;
				return result;
			}else
            {
				;//****查看全局层,如果没有或为struct则报错
				if(/*****没找到*/1)
                {
					;//****没有找到全局定义,报错
					error_msg(1,exp->line_no,tempnode1->subtype.IDVal);	//错误类型1，变量未定义
					return NULL;
				}else
                {
					result=result=tempnode1->nodeType;		//在全局层找到了
					return result;
				}
			}
		}else if(strcmp(tempnode1->character,"INT")==0)
        {//返回适合int的type
            result = N_INT;
			return result;
			;
		}else if(strcmp(tempnode1->character,"FLOAT")==0)
        {//处理float
			result = N_FLOAT;
			return result;
		}
		;  

        }else
        {
            treeNode* tempnode3=getchild(exp,2);
            //第一部分;
            if(tempnode3!=NULL)
            {
                treeNode* tempnode4 = getchild(exp,3);
                if(tempnode4==NULL&&
                   strcmp(tempnode3->character,"Exp")==0&&
                   strcmp(tempnode2->character,"LB")!=0)
                   {//3元且第三项是Exp，如Exp AND Exp
                    treeNode*Expnode1=tempnode1;
                    treeNode*Expnode2=tempnode3;
                    if(strcmp(Expnode1->character,"Exp")!=0)
                    {
                        printf("It isn't Exp xx Exp.\n");
                    }
                    int exp1type=Exp_s(Expnode1);
                    int exp2type=Exp_s(Expnode2);
                //	printf("hererer\n");
                    if(exp1type!=NULL&&exp2type!=NULL)
                    {
                        //int tempresult=check_type(exp1type,exp2type);	//检查类型是否匹配
                        int tempresult = 1;//不想报错
                        if(tempresult==0&&0==strcmp(tempnode2->character,"ASSIGNOP"))
                        {//赋值号
                            error_s(5,exp->line_no,NULL);	//错误类型5，赋值号两侧类型不匹配
                            return NULL;
                        }
                        if(tempresult==0)
                        {				//不是赋值号，为运算符
                            error_s(7,exp->line_no,NULL);	//错误类型7，操作数类型不匹配
                            return NULL;
                        }else
                        {
                            //左值错误,左值只能够是变量
                            result=exp1type;
                            return result;
                        }
                    }else
                    {
                        ;//如果是返回NULL的话exp里面肯定报错了,就不重复报错了;
                        return NULL;//把NULL往前传,因为有错;
                    }
                }
            }
		//第二部分;
		if(strcmp(tempnode1->character,"LP")==0||
           strcmp(tempnode1->character,"MINUS")==0||
           strcmp(tempnode1->character,"NOT")==0)
           {	//LP Exp RP，MINUS Exp，NOT Exp
			treeNode* expnode=tempnode2;
			if(strcmp(expnode->character,"Exp")!=0)
            {//第二个不是exp
				printf("The second part should be Exp!\n");
			}
			int exp1type=Exp_s(expnode);
			result=exp1type;
			return result;
		}
		//第三部分;
		/*
		| ID LP Args RP 有参函数
		| ID LP RP 无参函数

		| Exp LB Exp RB 数组
		| Exp DOT ID 结构体;
		*/
		//函数部分：判断第一个是不是ID;需要检查这个函数的存在性,得到函数的params交给下一层检查,并且查看这个ID是不是函数类型

		/*********************************************
         *                 此处未完成                  *
         * ********************************************/
		else{
			treeNode*tempnode4=getchild(exp,3);
			
			if(tempnode4==NULL)
            {
				//结构体部分Exp DOT ID----结构体存在----域名存在----返回这个域名的type
				if(strcmp(tempnode1->character,"Exp")==0&&
                   strcmp(tempnode2->character,"DOT")==0&&
                   strcmp(tempnode3->character,"ID")==0)
                   {
					int exptype=Exp_s(tempnode1);
					
					// if(exptype==NULL)
					if(exptype!=NULL)
                    {
						if(exptype!=/****结构体*/1)
                        {	//当前Exp不是结构体    用datatype
							error_s(13,exp->line_no,NULL,NULL);	//错误类型13，对非结构体变量使用“.”
							return NULL;
						}else
                        {
							;//*******搜索域名;
							
							if(/*搜索到了*/1){
								//找到了!
								result=/*这个域名的type*/1;
								return result;
							}else{
								//域名不存在;
                                //////*****content待补充
								error_s(14,exp->line_no,NULL);	//错误类型14，该域没有在访问结构体中未定义
								return NULL;
							}
						}
					;}else{
						;return NULL;
					}
				}
			}else
            {;
				//数组部分;| Exp LB Exp RB4 数组
				if(strcmp(tempnode1->character,"Exp")==0&&
                   strcmp(tempnode2->character,"LB")==0&&
                   strcmp(tempnode3->character,"Exp")==0)
                {
					int type1=Exp_s(tempnode1);
					int type3=Exp_s(tempnode3);
					if(type1==NULL||type3==NULL)
                    {
						return NULL;
					}
					int checkresult=/****type1和type3类型是否匹配*/1;
					if(/****Exp不是数组*/1)//****应该是用datatype那边
                    {
						error_s(10,exp->line_no,NULL);	//错误类型10，对非数组变量进行数组访问
						return NULL;
					}else
                    {
						if(type3==N_INT){	//Exp是整数
							;
						}else{					//Exp不是整数
							error_s(12,exp->line_no,NULL);	//错误类型12，数组访问符中出现非整数
							return NULL;
						}
					}
					//返回结构体元素的类型result=type1->u.array_.elem;
					return result;
				}
			}
		}
		;
	}
	return NULL;//防止漏网之鱼;
	
    //未完……

}

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
            printf("Duplicated character \"%s\".\n", content);
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
