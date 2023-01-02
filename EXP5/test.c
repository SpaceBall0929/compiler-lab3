#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int num_of_vars[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//变量从a开始
int letter_now = 99;
char *var_name_gen()
{
    char *out = (char *)malloc(sizeof(char) * 3);
    out[0] = letter_now;
    out[1] = num_of_vars[letter_now - 97] + 48;
    out[2] = '\0';

    if (num_of_vars[letter_now - 97]++ == 9)
    {
        letter_now += 1;
    }

    return out;
}

int main(){
    printf("%s", var_name_gen());
    return 0;
}