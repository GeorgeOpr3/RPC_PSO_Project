#ifndef RPC_LIB_H
#define RPC_LIB_H

#include "rpc_common.h"
#include <stdbool.h>

int rpc_send(int sock, const void *data, size_t len);
int rpc_recv(int sock, void *buf, size_t len, int timeout_ms);
int rpc_connect(const char *ip, int port, int timeout_ms);
bool rpc_retry_connect(const char *ip, int port, int max_retries, int delay_ms);

#endif
