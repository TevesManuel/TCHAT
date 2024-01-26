#pragma once
#ifndef TEVES_UI_params
    #define TEVES_UI_params
    #include <chat_node.h>

    typedef struct UI_params
    {
        CHAT_NODE * header;
        char * user_message;
    }UI_params;

    UI_params * UI_params_new();
#endif