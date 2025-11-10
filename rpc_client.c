#include "rpc_client.h"
#include "rpc_lib.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


typedef struct {
    char func[LG_FUNCTIE_MAX];
    int arg1;
    int arg2;
    char ip[32];
    int port;
} async_call_args;


void *rpc_call_async_thread(void *arg) {
    async_call_args data = (async_call_args)arg;
    rpc_request req = {0};
    strcpy(req.func, data->func);
    req.arg1 = data->arg1;
    req.arg2 = data->arg2;

    rpc_response res;
    int sock = rpc_connect(data->ip, data->port, 2000);
    if (sock < 0) {
        printf("[Async] Eroare la conectare\n");
        free(data);
        return NULL;
    }

    rpc_send(sock, &req, sizeof(req));
    int r = rpc_recv(sock, &res, sizeof(res), 3000);
    close(sock);

    if (r <= 0) {
        printf("[Async] Timeout sau eroare la recepție\n");
    } else if (res.err_code == 0) {
        printf("[Async] Rezultatul %s(%d,%d) = %d\n", req.func, req.arg1, req.arg2, res.result);
    } else {
        printf("[Async] Funcție invalidă: %s\n", req.func);
    }

    free(data);
    return NULL;
}


void rpc_call_async(const char *ip, int port, const char *func, int a, int b) {
    async_call_args *args = malloc(sizeof(async_call_args));
    strcpy(args->ip, ip);
    args->port = port;
    strcpy(args->func, func);
    args->arg1 = a;
    args->arg2 = b;

    pthread_t tid;
    pthread_create(&tid, NULL, rpc_call_async_thread, args);
    pthread_detach(tid);
}


int rpc_call_sync(const char *host, uint16_t port,
                  const char *func, int arg1, int arg2,
                  int *out_result,
                  rpc_call_opts opts)
{
    if (!out_result) return -1;
    *out_result = 0;

    rpc_request req = {0};
    rpc_response res;
    strcpy(req.func, func);
    req.arg1 = arg1;
    req.arg2 = arg2;

    int sock = rpc_connect(host, port, opts.timeout_ms > 0 ? opts.timeout_ms : 2000);
    if (sock < 0) {
        printf("[Sync] Eroare: nu ma pot conecta la server.\n");
        return -1;
    }

    rpc_send(sock, &req, sizeof(req));
    int r = rpc_recv(sock, &res, sizeof(res), opts.timeout_ms > 0 ? opts.timeout_ms : 3000);
    close(sock);

    if (r <= 0) {
        printf("[Sync] Timeout sau conexiune inchisa.\n");
        return -2;
    }

    if (res.err_code == 0) {
        *out_result = res.result;
        printf("[Sync] Rezultatul %s(%d,%d) = %d\n", func, arg1, arg2, res.result);
        return 0;
    } else {
        printf("[Sync] Funcția '%s' nu există pe server.\n", func);
        return -3;
    }
}