#ifndef RPC_COMMON_H
#define RPC_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define LG_FUNCTIE_MAX 30
#define MSG_MAX 200

#define PRIORITY_LOW  0
#define PRIORITY_HIGH 1

typedef struct {
    char func[LG_FUNCTIE_MAX];
    int arg1;
    int arg2;
    int priority;   
} rpc_request;

typedef struct {
    int result;
    int err_code; 
} rpc_response;

#endif
