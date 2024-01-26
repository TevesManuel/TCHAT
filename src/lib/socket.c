#include <str.h>
#include <chat_node.h>
#include <UI_params.h>
#include <socket.h>
#include <uv.h>
#include <pthread.h>
#include <ctype.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#define STR_SEPARATOR "_TEVESSEPARATOR_"
#define MAX_EVENTS 64
#define SERVER_URL "127.0.0.1"
#define SERVER_PORT 3000
#define BUFFER_SIZE 1024

ParamsWbSk * ParamsWbSk_new()
{
    ParamsWbSk * out = malloc(sizeof(ParamsWbSk));
    out->name = str_new();
    out->partyname = str_new();
    out->ui_params = NULL;
    out->message_to_send = NULL;
    return out;
}

uv_tcp_t socket_client;
ParamsWbSk * params_wbsk;

void * thread_for_write(void * arg)
{
    while(true)
    {
        if(params_wbsk->message_to_send != NULL)
        {
            uv_write_t write_req;
            uv_buf_t write_buf;
            write_buf = uv_buf_init(params_wbsk->message_to_send , strlen(params_wbsk->message_to_send));
            uv_write(&write_req, (uv_stream_t*)&socket_client, &write_buf, 1, NULL);
            params_wbsk->message_to_send = NULL;
        }
    }
}

void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    if (nread > 0)
    {
        char * chain = str_new();
        sprintf(chain, "%.*s", (int)nread, buf->base);
        CHAT_NODE_add(params_wbsk->ui_params->header, chain);
        
        printf("Mensaje del servidor: %.*s\n", (int)nread, buf->base);
    }
    else if (nread < 0)
    {
        fprintf(stderr, "Error al leer datos: %s\n", uv_strerror(nread));
        uv_close((uv_handle_t*)stream, NULL);
        CHAT_NODE_add(params_wbsk->ui_params->header, "Server is down.");
    }

    // Liberar el buffer después de su uso
    free(buf->base);
}

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    *buf = uv_buf_init((char*)malloc(suggested_size), suggested_size);
}

void on_connect(uv_connect_t* req, int status)
{
    if (status < 0)
    {
        fprintf(stderr, "Error en la conexión: %s\n", uv_strerror(status));
        CHAT_NODE_add(params_wbsk->ui_params->header, "Server is down.");
        uv_close((uv_handle_t*)req->handle, NULL);
        return;
    }

    printf("Conectado al servidor!\n");

    uv_read_start((uv_stream_t*)&socket_client, alloc_buffer, on_read);
    
    pthread_t hilo;
    
    if (pthread_create(&hilo, NULL, thread_for_write, NULL) != 0)
    {
        fprintf(stderr, "Error al crear el hilo.\n");
        exit(EXIT_FAILURE);
    }

    char * message_to_register = str_new();
    sprintf(message_to_register, "_TEVESREGISTER_%s%s%s", params_wbsk->name, STR_SEPARATOR, params_wbsk->partyname);
    params_wbsk->message_to_send = message_to_register;
}

void * SocketFunction(void *arg)
{
    params_wbsk = arg;
    uv_loop_t *loop;
    uv_connect_t connect_req;
    loop = uv_default_loop();

    struct sockaddr_in dest;
    uv_ip4_addr(SERVER_URL, SERVER_PORT, &dest);

    uv_tcp_init(loop, &socket_client);

    uv_tcp_connect(&connect_req, &socket_client, (const struct sockaddr*)&dest, on_connect);

    uv_run(loop, UV_RUN_DEFAULT);
    pthread_exit(NULL);
}