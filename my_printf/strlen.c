#include <stdio.h>

int my_strlen(char* param_1)
{
    int i = 0;
if(param_1[i] == '\0'){
    return NULL;
}
for (i = 0; param_1[i] != '\0'; i++);
return i;

}

int main()
{
    // char c[4]= "";
    printf("%d\n",my_strlen(""));
}