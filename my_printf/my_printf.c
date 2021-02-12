#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int my_strlen(char* s);
char* converter(int d, int base);
char* converter_u(unsigned int d, int base);
char* converter_lu(unsigned long d, int base);
int my_printf(char *fmt, ...);

// int main()
// {
    
    // printf("%d\n",my_printf("NULL STRING %s!\n", "Yes!"));
    // printf("%d\n",my_printf("NULL STRING %s!\n",(char*)NULL));
    // printf("%d\n",my_printf("%d!\n", -1337));
    // printf("%d\n",printf("%d!\n",-1337));
    // printf("%d\n",my_printf("%d - %d - -%d!\n", 2048, 0, 1337));
    // printf("%d\n",my_printf("%o!\n", 100));//done
    // printf("%d\n",my_printf("%u!\n", 1337)); //done
    // printf("%d\n",printf("%x!\n", 14)); //done
    // int a;
    // printf("%d\n",printf("%p!\n", &a)); //done

//     return 0;
// }


int my_printf(char *fmt, ...)
{
    int sum = 0;
    va_list ap;
    int d;
    unsigned int o, x, u;
    void* p;
    char c;
    char *s;

    va_start(ap, fmt);
    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt++)
        {
            case 's':
            s = va_arg(ap, char *);
            if(s == NULL){
                char* res = "(null)";
               sum +=  write(1, res, 6);  
            }else
            {
            sum += write(1, s, my_strlen(s));
            }
                
            // free(s);
            break;
        case 'd':
            d = va_arg(ap, int);
            
            if (d < 0)
            {
                
               sum= write(1, "-", 1);
               d = -d;
            }
            
            s = converter(d, 10);
            sum += write(1, s, my_strlen(s));
            free(s);
            break;
        case 'o':
            o = va_arg(ap, unsigned int);
            s = converter_u(o, 8);
            sum += write(1, s, my_strlen(s));
            free(s);
            break;
        case 'u':
            u = va_arg(ap, unsigned int);
            s = converter_u(u, 10);
            sum += write(1, s, my_strlen(s));
            free(s);
            break;
        case 'x':
            x = va_arg(ap, unsigned int);
            s = converter_u(x, 16);
            sum += write(1, s, my_strlen(s));
            free(s);
            break;
        case 'p':
            p = va_arg(ap, void*);
            unsigned long longVal = (unsigned long)p;
            s = converter_lu(longVal, 16);
            sum += write(1, "0x", 2);
            sum += write(1, s, my_strlen(s));
            free(s);
            break;
        case 'c':           
            c = (char) va_arg(ap, int);
            write(1, &c, 1);
            sum++;
            break;
        }

        } else {
            write(1, fmt, 1);
            fmt++;
            sum++;
        }
    }
    va_end(ap);
    // va_end(fmt);
    return sum;
}

int my_strlen(char* param_1)
{
    int i = 0;
for (i = 0; param_1[i] != '\0'; i++);
return i;

}

char* converter(int d, int base)
{
    int tmp = 0;
    char* s = malloc(sizeof(char)*13);
    int i = 0;
    if (d == 0)
    {
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    for (i=0 ; d>0 ; i++)
    {
        tmp = d % base;
         d = d / base;
        if (tmp < 10)
        {
            s[i] = '0' + tmp;
        }
        else
        {
            s[i] = 'a' + tmp - 10;
        }
    }
    s[i] = '\0';

            
    int len = my_strlen(s);
    // if(d<0){
    //     len = my_strlen(s)+2;
    // }
    i--;
    char c ;
    for ( ; i >= len / 2; i--)
    {
        c = s[i];
        s[i] = s[len-i-1];
        s[len-i-1] = c;
    }
	return s;
    free(s);
}

char* converter_u(unsigned int d, int base)
{
    int tmp;
    char* s = malloc(sizeof(char) * 13);
    int i = 0;
    if (d == 0)
    {
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    for ( ; d > 0; i++)
    {
        tmp = d % base;
        d = d / base;
        if (tmp < 10)
        {
            s[i] = '0' + tmp;
        }
        else
        {
            s[i] = 'a' + tmp - 10;
        }
    }
    s[i] = '\0';
    int len = my_strlen(s);
    i--;
    for ( ; i >= len / 2; i--)
    {
        char c = s[i];
        s[i] = s[len-i-1];
        s[len-i-1] = c;
    }
	return s;
    free(s);
}

char* converter_lu(unsigned long d, int base)
{
    int tmp;
    char* s = malloc(sizeof(char) * 15);
    int i = 0;
    if (d == 0)
    {
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    for ( ; d > 0; i++)
    {
        tmp = d % base;
        d = d / base;
        if (tmp < 10)
        {
            s[i] = 48 + tmp;
        }
        else
        {
            s[i] = 97 + tmp - 10;
        }
    }
    s[i] = '\0';
    int len = my_strlen(s);
    i--;
    for ( ; i >= len / 2; i--)
    {
        char c = s[i];
        s[i] = s[len-i-1];
        s[len-i-1] = c;
    }
	return s;
    free(s);
}

