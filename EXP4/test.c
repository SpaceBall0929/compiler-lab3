#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    char *ttt = "*a0";
    if (ttt[0] == '*')
    {
        ttt = ttt + 1;
    }
    printf("%s\n", ttt);

    return 0;
}