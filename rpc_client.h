#ifndef RPC_CLIENT_H
#define RPC_CLIENT_H

#include "rpc_common.h"

typedef struct {
    int timeout_ms;
    int retries;
} rpc_call_opts;

int rpc_call_sync(const char *host, uint16_t port,
                  const char *func, int arg1, int arg2,
                  int priority,
                  int *out_result,
                  rpc_call_opts opts);

void rpc_call_async(const char *ip, int port,
                    const char *func, int a, int b,
                    int priority);

#endif
