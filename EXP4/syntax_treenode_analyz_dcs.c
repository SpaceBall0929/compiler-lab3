#include "exp.h"

// 判定specifier的指向：整形/浮点/结构体定义/结构体使用
int specifier(treeNode * speci)
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

dataNodeVar *var_dec(treeNode *dec_node, int var_type)
{
    dataNodeVar *new_var;
    treeNode *origrn = dec_node;
    dec_node = dec_node->child;
    int dimension = 0;
    int dimensionlen[10];
    while (dec_node->nodeType != N_ID)
    {
        dimensionlen[dimension++] = dec_node->sibling->sibling->subtype.intVal;
        dec_node = dec_node->child;
    }

    //这里直接给变量换名字了，方便中间代码使用
    new_var = newNodeVar(dec_node->subtype.IDVal, var_name_gen(), var_type);
    if (dimension == 0)
    {
        return new_var;
    }
    new_var->arrayVarType = var_type;
    new_var->varType = D_ARRAY;
    new_var->numdim = dimension;
    new_var->len_of_dims = (int *)malloc(sizeof(int) * dimension);
    for (int i = 0; i < dimension; i++)
    {
        new_var->len_of_dims[i] = dimensionlen[i];
    }
    return new_var;
}

dataNodeVar *param_dec(treeNode *para)
{
    int type_def = specifier(para->child);
    if (type_def == D_STRUCT_DEF)
    {
         printf("ERROR: Unexpected treeNode: structure definition in the parameters of the function.\n");
        return NULL;
    }

    return var_dec(para->child->sibling, type_def);
}

//处理参数表
dataNodeVar *var_list(treeNode *arg_list)
{
    arg_list = arg_list->child;
    dataNodeVar *temp_node = param_dec(arg_list);
    dataNodeVar *ptr = temp_node;
    arg_list = arg_list->sibling;
    while (arg_list != NULL)
    {
        arg_list = arg_list->sibling->child;
        ptr->next = param_dec(arg_list);
        ptr = ptr->next;
        arg_list = arg_list->sibling;
    }
    ptr->next = NULL;

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
    return newNodeFunc(dec_node->child->subtype.IDVal, return_type, 0, arg_list);
}

int struct_specifier_dec(treeNode *dec_node)
{
    dec_node = dec_node->child->sibling->child;
    int temp = charToInt(dec_node->subtype.IDVal, *struct_table);
    if (temp == -1)
    {
        error_msg(17, dec_node->line_no, dec_node->subtype.IDVal);
    }
    return temp;
}
