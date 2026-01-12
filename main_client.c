#include "rpc_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    // -------------------------
    // Valori default
    // -------------------------
    char IP[32] = "127.0.0.1";
    int port = PORT;
    int timeout = 3000;
    int priority = PRIORITY_HIGH;   // HIGH implicit

    // -------------------------
    // Parsare flaguri
    // -------------------------
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--ip") == 0 && i + 1 < argc) {
            strcpy(IP, argv[++i]);
        }
        else if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            port = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--timeout") == 0 && i + 1 < argc) {
            timeout = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--priority") == 0 && i + 1 < argc) {
            if (strcmp(argv[i + 1], "low") == 0)
                priority = PRIORITY_LOW;
            else
                priority = PRIORITY_HIGH;
            i++;
        }
        else {
            printf("Flag necunoscut: %s\n", argv[i]);
            printf("Utilizare:\n");
            printf("  --ip <IP>\n");
            printf("  --port <PORT>\n");
            printf("  --timeout <ms>\n");
            printf("  --priority low|high\n");
            return 1;
        }
    }

    int mode, a, b, result;
    char func[LG_FUNCTIE_MAX];

    // -------------------------
    // Meniu principal
    // -------------------------
    while (1) {
        printf("\n=== CLIENT RPC ===\n");
        printf("Server: %s:%d | timeout=%dms | priority=%s\n",
               IP, port, timeout,
               priority == PRIORITY_HIGH ? "HIGH" : "LOW");

        printf("1. Apel sincron\n");
        printf("2. Apel asincron\n");
        printf("0. Iesire\n");
        printf("Alege: ");
        scanf("%d", &mode);

        if (mode == 0)
            break;

        printf("Functie (add/sub): ");
        scanf("%s", func);
        printf("Arg1: ");
        scanf("%d", &a);
        printf("Arg2: ");
        scanf("%d", &b);

        rpc_call_opts opts;
        opts.timeout_ms = timeout;
        opts.retries = 1;

        if (mode == 1) {
            // ---------- SINCRON ----------
            rpc_call_sync(
                IP,
                port,
                func,
                a,
                b,
                priority,   // <-- PARAMETRUL NOU
                &result,
                opts
            );
        }
        else if (mode == 2) {
            // ---------- ASINCRON ----------
            rpc_call_async(
                IP,
                port,
                func,
                a,
                b,
                priority    // <-- PARAMETRUL NOU
            );
        }
        else {
            printf("Optiune invalida!\n");
        }
    }

    return 0;
}
