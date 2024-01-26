#include <pdcurses.h>
#include <pthread.h>
#include <libwebsockets.h>
#include <str.h>
#include <chat_node.h>
#include <UI_params.h>
#include <socket.h>


#define DELETE_KEY_CODE 8

#define STATE_MYNAME 0
#define STATE_PARTYNAME 1
#define STATE_CHAT 3

void Window_render_UI(UI_params * ui_params)
{
    clear();

    int y = 0;
    int i = 0;
    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);

    if(ui_params->header != NULL)
    {
        int len_of_chat = CHAT_NODE_len(ui_params->header);
        CHAT_NODE * next = ui_params->header;
        while(next != NULL)
        {
            if(len_of_chat < (maxY-2) || i > (len_of_chat-maxY-2))
            {
                mvaddstr(y, 0, next->message);
                y++;
            }
            i++;
            next = next->next;
            if(y == maxY - 2)
                break;
        }
        int x;
        for (x = 0; x < maxX; x++)
        {
            mvaddch(y, x, '-');
        }

        y++;
    }
    mvaddstr(y, 0, "You:");
    mvaddstr(y, 4, ui_params->user_message);
    refresh();
}

int main()
{
    UI_params * params_for_ui = UI_params_new();
    ParamsWbSk * params_for_thread = ParamsWbSk_new();
    params_for_thread->ui_params = params_for_ui;
    int STATE = 0;//0-Get S/C, 1-Get IP, 2-Get PORT, 3-Chat

    initscr();
    timeout(0);

    bool live = true;

    pthread_t thread;

    while(live)
    {
        clear();
        
        if(STATE == STATE_MYNAME)
        {
            char * text = str_new();
            sprintf(text, "Your name: %s", params_for_ui->user_message);
            mvaddstr(0, 0, text);
        }
        else if(STATE == STATE_PARTYNAME)
        {
            char * text = str_new();
            sprintf(text, "Party name: %s", params_for_ui->user_message);
            mvaddstr(0, 0, text);
        }
        else if(STATE == STATE_CHAT)
            Window_render_UI(params_for_ui);

        refresh();
        char c = getch();
        if (c != ERR)
        {
            if(STATE == STATE_MYNAME)
            {
                if(c == '\n')
                {
                    sprintf(params_for_thread->name, "%s", params_for_ui->user_message);
                    str_clear(params_for_ui->user_message);
                    STATE = STATE_PARTYNAME;
                }
                else if(c == DELETE_KEY_CODE)
                {
                    str_rm_lastch(params_for_ui->user_message);
                }
                else
                {
                    str_add_ch(params_for_ui->user_message, c);
                }
            }
            else if(STATE == STATE_PARTYNAME)
            {
                if(c == '\n')
                {
                    sprintf(params_for_thread->partyname, "%s", params_for_ui->user_message);
                    str_clear(params_for_ui->user_message);
                    STATE = STATE_CHAT;
                    char * out_message = str_new();
                    sprintf(out_message, "Chat opened in party %s.", params_for_thread->partyname);
                    params_for_ui->header = CHAT_NODE_add(params_for_ui->header, out_message);
                    if (pthread_create(&thread, NULL, SocketFunction, params_for_thread) != 0)
                    {
                        fprintf(stderr, "Error at create socket thread.\n");
                        return EXIT_FAILURE;
                    }  
                }
                else if(c == DELETE_KEY_CODE)
                {
                    str_rm_lastch(params_for_ui->user_message);
                }
                else
                {
                    str_add_ch(params_for_ui->user_message, c);
                }
            }
            else if(STATE == STATE_CHAT)
            {
                if(c == '\n')
                {
                    char * out_message = str_new();
                    sprintf(out_message, "You: %s", params_for_ui->user_message);
                    params_for_ui->header = CHAT_NODE_add(params_for_ui->header, out_message);
                    free(out_message);
                    out_message = str_new();
                    sprintf(out_message, "%s", params_for_ui->user_message);
                    params_for_thread->message_to_send = out_message;
                    while(params_for_thread->message_to_send != NULL) {}
                    // printf("Send message nro%d.\n", CHAT_NODE_len(params_for_ui->header));
                    free(out_message);
                    str_clear(params_for_ui->user_message);
                }
                else if(c == DELETE_KEY_CODE)
                {
                    str_rm_lastch(params_for_ui->user_message);
                }
                else
                {
                    str_add_ch(params_for_ui->user_message, c);
                }
            }
            // printf("Char inputed: %c;%d\n", c, c);
        }
    }

    if (pthread_join(thread, NULL) != 0)
    {
        fprintf(stderr, "Error at waiting the thread.\n");
        return 1;
    }

    endwin();
    return 0;
}