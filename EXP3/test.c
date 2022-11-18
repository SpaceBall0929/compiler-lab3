#include<stdio.h>
#include"DomainStack.c"

int main(){
    SymbolTableStruct *struct_table;
    struct_table = tableStructInit();
    dataNodeStruct* new_struct = newNodeStruct("test");
    InsertStruct(struct_table, new_struct, 10);
    dataNodeVar* t1 = newNodeVar("t1", D_INT);
    dataNodeVar* t2 = newNodeVar("t2", D_FLOAT);
    insertStructDomain(new_struct, t1 , *struct_table, 11);
    insertStructDomain(new_struct, t2 , *struct_table, 11);
    free_struct(new_struct);
    free_var(t1);
    free_var(t2);
    int out = ifExistStruct(*struct_table, "test");
    printf("out is %d", out);
    return 0;
}