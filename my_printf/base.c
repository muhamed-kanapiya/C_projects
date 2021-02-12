#include <stdio.h>
#include <stdlib.h>


int my_strlen(char* param_1);

char* converter(int d, int base)
{
    int tmp;
    char* s = malloc(sizeof(char)*13);
    int i = 0;
    if (d == 0)
    {
        s[0] = '0';
        s[1] = '\0';
        // 
        return s;
    }
    for (i=0 ; d>0; i++)
    {
        tmp = d % base;
        
        if (tmp < 10)
        {
            s[i] = '0' + tmp;
        }
        else
        {
            s[i] = 'a' + tmp - 10;
        }
        d = d / base;
    }
    free(s);
    // s[i] = '\0';
    // int len = my_strlen(s);
    // i--;
    // char c ;
    // for ( ; i >= len / 2; i--)
    // {
    //     c = s[i];
    //     s[i] = s[len-i-1];
    //     s[len-i-1] = c;
    // }
    // free(s);
	return s;
}

int my_strlen(char* param_1)
{
    int i = 0;
for (i = 0; param_1[i] != '\0'; i++){
return i;
}
}

int main(){
    printf(converter(15,10));
}