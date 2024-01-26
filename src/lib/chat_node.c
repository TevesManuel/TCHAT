#include <chat_node.h>
#include <str.h>
#include <stdlib.h>
#include <stdio.h>

CHAT_NODE * CHAT_NODE_new()
{
    CHAT_NODE * out = malloc(sizeof(CHAT_NODE));
    out->message = str_new();
    out->next = NULL;
    return out;
}
CHAT_NODE * CHAT_NODE_add(CHAT_NODE * header, char * new_message)
{
    if(header == NULL)
    {
        header = CHAT_NODE_new();
        sprintf(header->message, "%s", new_message);
    }
    else
    {
        CHAT_NODE * next = header;
        while(1)
        {
            if(next->next == NULL)
            {
                next->next = CHAT_NODE_new();
                next = next->next;
                next->message = str_new();
                sprintf(next->message, "%s", new_message);
                break;
            }
            next = next->next;
        }
    }
    return header;
}
int CHAT_NODE_len(CHAT_NODE * header)
{
    int out = 0;
    CHAT_NODE * next = header;
    while(next != NULL)
    {
        out++;
        next = next->next;
    }
    return out;
}