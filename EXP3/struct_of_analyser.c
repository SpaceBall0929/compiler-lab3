#include <stdio.h>
#include "DomainStack.c"

//亟待解决的问题：
//改变所有的类型传入为int型
//锁定错误位置，增添定义部分的报错



stackNode *var_domain_ptr;
SymbolTableFunc *fun_table;

SymbolTableStruct *struct_table;

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

// 识别非终结符VarDec,收集变量的名字，收集是否是数组，返回一个初始化好的dataNodeVar
dataNodeVar *var_dec(treeNode *dec_node, char* var_type)
{
    dataNodeVar *new_var;
    treeNode *origrn = dec_node;
    dec_node = dec_node->child;
    int dimension = 0;
    int dimensionlen[10];
    while (dec_node->nodeType != N_ID)
    {
        dec_node = dec_node->child;
        dimensionlen[dimension++] = dec_node->sibling->sibling->subtype.intVal;
    }
    new_var = newNodeVar(dec_node->subtype.IDVal, var_type, struct_table);
    if (dimension == 0)
    {
        return new_var;
    }
    new_var->numdim = dimension;
    new_var->len_of_dims = (int *)malloc(sizeof(int) * dimension);
    for (int i = 0; i < dimension; i++)
    {
        new_var->len_of_dims[i] = dimensionlen[dimension - 1 - i];
    }
    return new_var;
}

dataNodeVar* param_dec(treeNode* para){
    int type_def = specifier(para -> child);
    if(type_def == D_STRUCT_DEF){
        printf("ERROR: Unexpected treeNode: structure definition in the parameters of the function.\n");
        return NULL;
    }

    return var_dec(para -> child -> sibling, para -> child -> character);
}

//处理参数表
dataNodeVar *var_list(treeNode *arg_list)
{
    arg_list = arg_list->child;
    dataNodeVar* temp_node = param_dec(arg_list);
    dataNodeVar* ptr = temp_node;
    arg_list = arg_list -> sibling;
    while (arg_list != NULL){
        arg_list = arg_list -> sibling -> child;
        ptr -> next = param_dec(arg_list);
        ptr = ptr -> next;
        arg_list = arg_list -> sibling; 
    }
    ptr -> next = NULL;

    return temp_node;
}

//处理FunDec
dataNodeFunc *fun_dec(treeNode *dec_node, int return_type)
{
    treeNode *temp_node = dec_node->child->sibling->sibling;
    dataNodeVar *arg_list = NULL;
    if (temp_node->nodeType == N_VAR_L)
    {
        arg_list = var_list(temp_node);
    }
    return newNodeFunc(dec_node->child->subtype.IDVal, return_type, 0, arg_list, struct_table);
}

//判定specifier的指向：整形/浮点/结构体定义/结构体使用
int specifier(treeNode *speci)
{
    switch (speci->child->nodeType)
    {
    case N_TYPE:
        if (speci->child->subtype.IDVal[0] == 'i')
        {
            return D_INT;
        }
        return D_FLOAT;

        break;

    case N_STRUCT_SPECI:
        if (speci->child->child->sibling->nodeType == N_TAG)
        {
            return D_STRUCT_DEC;
        }
        return D_STRUCT_DEF;
    default:
        printf("ERROR: Unexpected nodeType in the child of Specifier\n");
        return 0;
        break;
    }
}

dataNodeVar *dec_list(treeNode *decs, char *var_type)
{
    dataNodeVar *temp_var;
    dataNodeVar *return_vars;
    decs = decs->child;
    temp_var = var_dec(decs->child, var_type);
    return_vars = temp_var;
    if (decs->child->sibling != NULL)
    {
        if (charToInt(var_type, struct_table) != Exp_s(decs->child->sibling->sibling))
        {
            error_msg(5, decs->line_no, decs->child->child->subtype.IDVal);
        }
    }
    decs = decs->sibling;
    while (decs != NULL)
    {
        decs = decs->sibling->child;
        temp_var->next = var_dec(decs->child, var_type);
        temp_var = temp_var->next;
        if (decs->child->sibling != NULL)
        {
            if (charToInt(var_type, struct_table) != Exp_s(decs->child->sibling->sibling))
            {
                error_msg(5, decs->line_no, decs->child->child->subtype.IDVal);
            }
        }
        decs = decs -> sibling;
    }

    temp_var = NULL;
    return return_vars;
}

// int dec_list(treeNode* decs, char* var_type){
//     dataNodeVar* temp_var;
    
//     do{
//         decs = decs  -> child;
//         InsertVar(&(var_domain_ptr -> tVar), var_dec(decs -> child, var_type));
//         if(decs -> sibling != NULL){
//             if(charToInt(var_type, struct_table) !=Exp_s(decs -> sibling -> sibling)){
//                 error_msg(5, decs ->line_no, decs -> child -> child -> subtype.IDVal);
//             }
//         }
//         decs = decs -> sibling;
//         if(decs != NULL){
//             decs = decs -> sibling;
//         }
//     }while(decs != NULL);

//     return 0;
// }

dataNodeVar* def_list(treeNode* defs){
    // int temp_type;
    dataNodeVar* return_vars;
    dataNodeVar* temp_ptr;
    defs = defs -> child;
    if(defs == NULL){
        return NULL;
    }
    return_vars = dec_list(defs -> child -> sibling, defs -> child -> child -> character);
    temp_ptr = return_vars;
    defs = defs -> sibling -> child;
    while (defs != NULL)
    {
        temp_ptr = temp_ptr -> next;
        temp_ptr = dec_list(defs -> child -> sibling, defs -> child -> child -> character);
        defs = defs -> sibling -> child; 
    }
    temp_ptr = NULL;
    
    return return_vars;
}


int comp_stmt(treeNode* comp_stmt, int expected_type){
    dataNodeVar* all_vars; 
    dataNodeVar* temp_for_del;
    domainPush(var_domain_ptr);
    all_vars = def_list(comp_stmt -> child -> sibling);
    while (all_vars != NULL)
    {
        temp_for_del = all_vars;
        InsertVar(&(var_domain_ptr->tVar), all_vars);
        all_vars = all_vars -> next;
        free(temp_for_del);
    }
    
    Stmt_s(comp_stmt -> child -> sibling -> sibling, var_domain_ptr, expected_type);
    domainPop(var_domain_ptr);

    return 0;
}

int struct_specifier_def(treeNode* def_node){
    char* name = NULL;
    def_node = def_node -> child -> sibling;
    if(def_node -> child != NULL){
        name = def_node  -> sibling -> child -> subtype.IDVal;
    }
    dataNodeStruct* new_struc = newNodeStruct(name);
    dataNodeVar* temp_for_del =new_struc ->structDomains;
    dataNodeVar* temp2;
    insertStructDomain(new_struc, def_list(def_node -> sibling ->sibling));
    InsertStruct(struct_table, *new_struc);
    while(temp_for_del != NULL){
        temp2 = temp_for_del;
        temp_for_del = temp_for_del -> next;
        free(temp2);
    }
    free(new_struc);

    return 0;
}

int struct_specifier_dec(treeNode* dec_node){
    
}


int ext_def(treeNode *ExtDef, seqStack *stack, stackNode *domain)
{

    treeNode *type_node = ExtDef->child->child;
    treeNode *core_node = ExtDef->child->sibling;
    int def_type = specifier(type_node);

    switch (def_type)
    {
    case D_STRUCT_DEC:
        def_type = charToInt(type_node -> child -> sibling -> child -> subtype.IDVal, struct_table);
    case D_INT:
    case D_FLOAT:
        if (core_node->nodeType == N_EXT_DEC_L)
        {
            treeNode *temp_node;
            do
            {
                temp_node = core_node->child;
                core_node = temp_node->sibling->sibling;
                InsertVar(&(domain->tVar), var_dec(temp_node, type_node -> child -> character));
            } while (core_node != NULL);
        }
        else
        {
            dataNodeFunc* abc = fun_dec(core_node, type_node -> child -> character);
            if(core_node -> sibling ->nodeType != N_SEMI){
                abc -> defined = 1;
                //返回类型不匹配在这里面报错
                comp_stmt(core_node -> sibling, def_type);
            }
            
            InsertFunc(fun_table, *abc);
            free(abc);
        }

        break;
    case D_STRUCT_DEF:
        struct_specifier_def(type_node);
        break;

    
        // struct_specifier_dec(type_node);
        // break;

    default:
        break;
    }

    return 0;
}

int tree_analys(treeNode *mytree)
{

    //栈初始化部分
    treeNode *temp;
    seqStack myStack;
    seqStack *stack_ptr;
    stack_ptr = &myStack;
    initStack(stack_ptr);
    push(stack_ptr, mytree);

    //表初始化部分
    var_domain_ptr = createStackNode();
    tableFuncInit(fun_table);
    tableStructInit(struct_table);
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
            ext_def(temp, stack_ptr, var_domain_ptr);
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
/*
剩余要解决的问题：
    type
*/
#include "tree.c" //不想看报错
void error_msg(int type, int line_no, char *content);
int StmtList_s(treeNode *stmt, int d_type)
{
	/*
	StmtList -> 
      Stmt StmtList
	| 空
	Stmt -> 
      Exp SEMI
	| CompSt
	| RETURN Exp SEMI
	| IF LP Exp RP Stmt
	| IF LP Exp RP Stmt ELSE Stmt
	| WHILE LP Exp RP Stmt
	*/
	treeNode*Stmtnode=getchild(stmt,0);
	treeNode*tempnode=getchild(stmt,1);
	Stmt_s(Stmtnode, d_type);
	if(tempnode!=NULL)
    {
		StmtList_s(tempnode, d_type) ;
	}

}
int Stmt_s(treeNode *stmt, int d_type)
{
	/*
	Stmt -> Exp SEMI
	| CompSt
	| RETURN Exp SEMI
	| IF LP Exp RP Stmt
	| IF LP Exp RP Stmt ELSE Stmt
	| WHILE LP Exp RP Stmt
	*/
	
	treeNode* tempnode1=getchild(stmt,0);
	if(tempnode1->nodeType == N_COMPST)
    {   
        //新开一个作用域,进入CompSt，然后溜
        domainStack ds;
        domainPush(ds);
        comp_stmt(tempnode1, d_type);
        domainPop(ds);
	}else if(tempnode1->nodeType == N_EXP)
    {
		int uselesstype=Exp_s(tempnode1);//返回exp的返回type
	}else if(tempnode1->nodeType == N_RETURN)
    {	
		treeNode* expnode=getchild(stmt,1);
		if(expnode->nodeType != N_EXP)
        {
			printf("Stmt_s bug: should be Exp!\n");
		}
		int returntype = Exp_s(expnode);
		if(returntype != NULL)
        {
			//!!!!!!!!!!可能要改  int result=check_type(d_type, returntype);
			if(d_type == returntype)
            {
				error_msg(8, stmt->line_no, NULL);	//错误类型8，函数返回类型不匹配
				return -1;
			}else
            {
				;//exp里面已经因为NULL报错
			}
		}		

	}else if(tempnode1->nodeType == N_WHILE)
    {
		treeNode* expnode = getchild(stmt,2);
		treeNode* stmtnode = getchild(stmt,4);
		int type = Exp_s(expnode);
        ///可能要改
		if(type!=NULL){
			if(type == 0)
            {
				;
			}else
            {
				error_msg(7,stmt->line_no,NULL);	//错误类型7，while条件操作数类型不匹配
			}
			
		}else
        {
			;//Exp里面已经报过了
		}
		Stmt_s(stmtnode, d_type);
	}else if(tempnode1->nodeType == N_IF)
    {
		/*	| IF LP Exp RP Stmt
	| IF LP Exp RP Stmt ELSE Stmt
	*/
		treeNode*expnode=getchild(stmt,2);
		if(expnode->nodeType != N_EXP)
        {
			printf("Stmt_s bug: should be Exp!\n");
			
		}
		treeNode* tempnode6=getchild(stmt,5);//ELSE
		int iftype = Exp_s(expnode);
		if(iftype!=NULL){
			if(iftype == 0)
            {
				;
			}else
            {
				error_msg(7,stmt->line_no,NULL);	//错误类型7，if条件操作数类型不匹配
				//return -1;
			}
		}
		if(tempnode6==NULL)
        {
			treeNode*stmtnode1=getchild(stmt,4);

			Stmt_s(stmtnode1, d_type);
		}else
        {
			treeNode*stmtnode1=getchild(stmt,4);
			treeNode*stmtnode2=getchild(stmt,6);

			Stmt_s(stmtnode1, d_type);

			Stmt_s(stmtnode2, d_type);
		}
		;
	}else
    {
		printf("Stmt_s error: Impossible to be here!\n");
	}
	return 0;
}

int Exp_s(treeNode *exp)
{ //处理Exp
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
    if (exp == NULL)
    {
        return NULL;
    };
    node_type result = NULL;

    treeNode *tempnode1 = getchild(exp, 0);
    treeNode *tempnode2 = getchild(exp, 1);

    // ID, EXP DOT ID(结构体), Exp LB Exp RB (数组)
    if (tempnode1->nodeType == N_EXP)
    {
        if (tempnode2 != NULL && tempnode2->nodeType == N_ASSIGNOP)
        { // Exp ASSIGNOP Exp
            treeNode *tempnode11 = getchild(tempnode1, 0);
            treeNode *tempnode12 = getchild(tempnode1, 1);
            if (tempnode12 == NULL)
            {
                if (tempnode11->nodeType != N_ID)
                {                                     //左侧不是ID
                    error_msg(6, exp->line_no, NULL); //报错
                    return NULL;
                }
            }
            else
            {
                treeNode *tempnode13 = getchild(tempnode1, 2);
                if (tempnode13 != NULL)
                {
                    treeNode *tempnode14 = getchild(tempnode1, 3);
                    if (tempnode14 == NULL)
                    { // Exp DOT ID(结构体)
                        if (tempnode11->nodeType == N_EXP &&
                            tempnode12->nodeType == N_DOT &&
                            tempnode13->nodeType == N_ID )
                        {
                            ; //正确
                        }
                        else
                        { //报错
                            error_msg(6, exp->line_no, NULL);
                            return NULL;
                        }
                    }
                    else
                    { // EXP LB EXP RB (数组)
                        if (tempnode11->nodeType == N_EXP &&
                            tempnode12->nodeType == N_LB &&
                            tempnode13->nodeType == N_EXP &&
                            tempnode14->nodeType == N_RB)
                        {
                            ; //正确
                        }
                        else
                        { //报错
                            error_msg(6, exp->line_no, NULL);
                            return NULL;
                        }
                    }
                }
                else
                { // tempnode13==NULL 报错
                    error_msg(6, exp->line_no, NULL);
                    return NULL;
                }
            }
        }
    }

    if (tempnode2 == NULL)
    { // ID，INT，FLOAT
        if (tempnode1->nodeType == N_ID)
        { //检查该ID是否已定义  (local & global)
            if (!ifExistVarStack(var_domain_ptr, tempnode1->character))
            {
                error_msg(1, exp->line_no, tempnode1->character); //错误类型1，变量未定义
                return NULL;
            }
            else
            {
                result = tempnode1->nodeType; //找到了
                return result;
            }
            
        }
        else if (tempnode1->nodeType == N_INT)
        { //返回int
            result = D_INT;
            return result;
            ;
        }
        else if (tempnode1->nodeType == N_FLOAT)
        { //处理float
            result = D_FLOAT;
            return result;
        };
    }
    else
    {
        treeNode *tempnode3 = getchild(exp, 2);
        //第一部分;
        if (tempnode3 != NULL)
        {
            treeNode *tempnode4 = getchild(exp, 3);
            if (tempnode4 == NULL &&
                tempnode3->nodeType == N_EXP &&
                tempnode2->nodeType != N_LB)
            { // 3元且第三项是Exp，如Exp AND Exp
                treeNode *Expnode1 = tempnode1;
                treeNode *Expnode2 = tempnode3;
                if (Expnode1->nodeType != N_EXP)
                {
                    printf("It isn't Exp xx Exp.\n");
                }
                int exp1type = Exp_s(Expnode1);
                int exp2type = Exp_s(Expnode2);
                if (exp1type != NULL && exp2type != NULL)
                {
                    //检查类型是否匹配
                    if (exp1type == exp2type && tempnode2->nodeType == N_ASSIGNOP)
                    {                                   //赋值号
                        error_msg(5, exp->line_no, NULL); //错误类型5，赋值号两侧类型不匹配
                        return NULL;
                    }
                    if (exp1type != exp2type)
                    {                                   //不是赋值号，为运算符
                        error_msg(7, exp->line_no, NULL); //错误类型7，操作数类型不匹配
                        return NULL;
                    }
                    else
                    {
                        //左值错误,左值只能够是变量
                        result = exp1type;
                        return result;
                    }
                }
                else
                {
                    ;            //如果是返回NULL的话exp里面肯定报错了,就不重复报错了;
                    return NULL; //把NULL往前传,因为有错;
                }
            }
        }
        //第二部分;
        if (tempnode1->nodeType == N_LP ||
            tempnode1->nodeType == N_MINUS ||
            tempnode1->nodeType == N_NOT)
        { // LP Exp RP，MINUS Exp，NOT Exp
            treeNode *expnode = tempnode2;
            if (expnode->nodeType != N_EXP)
            { //第二个不是exp
                printf("The second part should be Exp!\n");
            }
            int exp1type = Exp_s(expnode);
            result = exp1type;
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
		if(tempnode1->nodeType == N_ID)
        {	//当前为函数，需要去检查该函数是否已定义
			char*funcname=tempnode1->character;
			int querytype;
			int queryifdef=-1;
			int queryresult=ifExistFunc(*fun_table, funcname);	//在全局里面搜索;
            dataNodeFunc func_node = getNodeFunc(*fun_table, funcname);//搜素这个函数节点
            int ret_type = func_node.returnType;//获取函数返回类型
			if(queryresult)
            {
				//找到了,判断类型;
				if(querytype != 6)
                {		//当前ID不是函数名
					error_msg(11,exp->line_no, funcname);//错误类型11，对普通变量调用函数，例如i()；
					return NULL;
				}
			}

			if(!queryresult){//没找到或者不是定义;  
				error_msg(2, exp->line_no, funcname);		//错误类型2，函数未定义
				return NULL;
			}

			if(tempnode3->nodeType == N_ARGS)
            {
				if(func_node.args == NULL)
                {	//函数本身没有形参，但此时有实参
					error_msg(9, exp->line_no, NULL);	//错误类型9，函数实参形参不匹配
					return NULL;
				}else{
					/*Args -> Exp COMMA Args
					| Exp;
					*/
					//检查args的数量;
					int cnt=0;
					treeNode*cntnode=tempnode3;
					while(1)
                    {	//计算所有实参的数目
						cnt += 1;
						treeNode*tempcntnode=getchild(cntnode, 1);
						if(tempcntnode==NULL)
                        {
							break;
						}
						//cnt+=1;
						cntnode=getchild(cntnode, 2);
					}
					//printf("cnt:%d shouldbe:%d\n",cnt,querytype->u.function.paramnums);
					if(cnt!=querytype->u.function.paramnums)
                    {
						error_msg(9,exp->line_no,NULL,NULL);	//错误类型9，函数实参形参个数不匹配
						return NULL;
					}
					int argresult=Arg_s(tempnode3,querytype->u.function.params);
					if(argresult==0)
                    {
						return result;
					}else
                    {
						return NULL;
					}
				}
			}else
            {

				if(func_node.args!=NULL)
                {	//函数有形参，但此时没有实参
					error_msg(9,exp->line_no,NULL);	//错误类型9，函数实参形参个数不匹配
					return NULL;
				}
				else
                {
					return result;
				}
			}
			
		}
        else
        {
            treeNode *tempnode4 = getchild(exp, 3);

            if (tempnode4 == NULL)
            {
                //结构体部分Exp DOT ID----结构体存在----域名存在----返回这个域名的type
                if (tempnode1->nodeType == N_EXP &&
                    tempnode2->nodeType == N_DOT &&
                    tempnode3->nodeType == N_ID)
                {
                    int exptype = Exp_s(tempnode1);

                    // if(exptype==NULL)
                    if (exptype != NULL)
                    {
                        if (exptype != /****结构体*/ 1)
                        {                                          //当前Exp不是结构体    用datatype
                            error_msg(13, exp->line_no, NULL); //错误类型13，对非结构体变量使用“.”
                            return NULL;
                        }
                        else
                        {
                            ; //*******搜索域名;

                            if (ifExistStructDomain(struct_table, , ))
                            {
                                //找到了!
                                result = /*这个域名的type*/ 1;
                                return result;
                            }
                            else
                            {
                                //域名不存在;
                                //////*****content待补充
                                error_msg(14, exp->line_no, NULL); //错误类型14，该域没有在访问结构体中未定义
                                return NULL;
                            }
                        };
                    }
                    else
                    {
                        ;
                        return NULL;
                    }
                }
            }
            else
            {
                ;
                //数组部分;| Exp LB Exp RB 数组
                if (tempnode1->nodeType == N_EXP &&
                    tempnode2->nodeType == N_LB &&
                    tempnode3->nodeType==  N_EXP)
                {
                    int type1 = Exp_s(tempnode1);
                    int type3 = Exp_s(tempnode3);
                    if (type1 == NULL || type3 == NULL)
                    {
                        return NULL;
                    }
                    if (type1 != 2) //type1不是数组
                    {
                        error_msg(10, exp->line_no, NULL); //错误类型10，对非数组变量进行数组访问
                        return NULL;
                    }
                    else
                    {
                        if (type3 == D_INT)
                        { // Exp是整数
                            ;
                        }
                        else
                        {                                    // Exp不是整数
                            error_msg(12, exp->line_no, NULL); //错误类型12，数组访问符中出现非整数
                            return NULL;
                        }
                    }
                    //*****返回结构体元素的类型result=type1->u.array_.elem;
                    result = /***这是个什么的数组?*/;
                    return result;
                }
            }
        };
    }
    return NULL; //防止漏网之鱼;
}

void error_msg(int type, int line_no, char *content)
{ //报错
    printf("Error type %d at Line %d: ", type, line_no);
    switch (type)
    {
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
