#include "rpc_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char *IP = "10.55.90.247";
    int port = PORT;
    int mode, a, b, result;
    char func[LG_FUNCTIE_MAX];

    while (1) {
        printf("\n=== CLIENT RPC ===\n");
        printf("1. Apel sincron\n");
        printf("2. Apel asincron\n");
        printf("0. Iesire\n");
        printf("Alege: ");
        scanf("%d", &mode);
        if (mode == 0) break;

        printf("Functie (add/sub): ");
        scanf("%s", func);
        printf("Arg1: "); scanf("%d", &a);
        printf("Arg2: "); scanf("%d", &b);

        FILE *log = fopen("client.log", "a");
        if (log) {
            fprintf(log, "[CLIENT] Cerere: %s(%d,%d), mod=%s\n",
                func, a, b,
                (mode == 1 ? "sync" : "async"));
            fclose(log);
        }

        if (mode == 1) {
            rpc_call_opts opts = { .timeout_ms = 3000, .retries = 1 };
            rpc_call_sync(IP, port, func, a, b, &result, opts);
        } 
        else if (mode == 2) {
            rpc_call_async(IP, port, func, a, b);
        }
    }
    return 0;
}
