#pragma once
#include <stddef.h>
#include <UI_params.h>
#include <stdbool.h>

#ifndef TEVES_SOCKET
    #define TEVES_SOCKET

    typedef struct ParamsWbSk
    {
        char * name;
        char * partyname;
        UI_params * ui_params;
        char * message_to_send;
    }ParamsWbSk;

    ParamsWbSk * ParamsWbSk_new();
    void * SocketFunction(void *arg);
#endif