#include <stdlib.h>
#include <str.h>

char * str_new()
{
    char * out = malloc(sizeof(char)*MAX_STR_SIZE);
    for(int i = 0; i < MAX_STR_SIZE; i++)
    {
        out[i] = '\0';
    }
    return out;
}

void str_add_ch(char * string, char ch)
{
    for(int i = 0; i < MAX_STR_SIZE; i++)
    {
        if(string[i] == '\0')
        {
            string[i] = ch;
            break;
        }
    }
}

void str_rm_lastch(char * string)
{
    for(int i = 0; i < MAX_STR_SIZE; i++)
    {
        if(string[i] == '\0')
        {
            string[i-1] = '\0';
            break;
        }
    }
}

void str_clear(char * string)
{
    for(int i = 0; i < MAX_STR_SIZE; i++)
    {
        string[i] = '\0';
    }
}