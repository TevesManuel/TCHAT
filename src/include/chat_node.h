#pragma once
#ifndef TEVES_CHAT_NODE
    #define TEVES_CHAT_NODE
    typedef struct CHAT_NODE
    {
        char * message;
        void * next;
    }CHAT_NODE;

    CHAT_NODE * CHAT_NODE_new();
    CHAT_NODE * CHAT_NODE_add(CHAT_NODE * header, char * new_message);
    int CHAT_NODE_len(CHAT_NODE * header);
#endif