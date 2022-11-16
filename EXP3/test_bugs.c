#include <stdio.h>
#include "DomainStack.c"

int main(){
    stackNode *var_domain_ptr;
    SymbolTableVar *testt;
    SymbolTableFunc *fun_table;
    SymbolTableStruct *struct_table;
    var_domain_ptr = createStackNode();
    tableVarInit(testt);
    // tableFuncInit(fun_table);
    // tableStructInit(struct_table);

    return 0;
}