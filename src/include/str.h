#pragma once
#ifndef TEVES_STR
    #define TEVES_STR
    #define MAX_STR_SIZE 255

    char * str_new();
    void str_add_ch(char * string, char ch);
    void str_rm_lastch(char * string);
    void str_clear(char * string);
#endif