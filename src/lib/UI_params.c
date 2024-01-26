#include <UI_params.h>
#include <chat_node.h>
#include <str.h>
#include <stdlib.h>

UI_params * UI_params_new()
{
    UI_params * out = malloc(sizeof(UI_params));
    out->header = NULL;
    out->user_message = str_new();
    return out;
}