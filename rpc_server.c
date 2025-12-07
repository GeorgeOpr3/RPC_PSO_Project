#include "rpc_common.h"
#include <pthread.h>

// ============================
// Variabile globale pentru sincronizare
// ============================
int client_count = 0;   // număr clienți activi
pthread_mutex_t client_lock = PTHREAD_MUTEX_INITIALIZER;

// ============================
// Funcțiile RPC
// ============================
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }

// ============================
// Thread handler pentru fiecare client
// ============================
void *handle_client(void *arg) {
    int client_fd = *(int*)arg;
    free(arg);

    // ---- SINCRONIZARE: incrementăm numărul de clienți activi ----
    pthread_mutex_lock(&client_lock);
    client_count++;
    printf("[Server] Client conectat. Clienti activi: %d\n", client_count);
    pthread_mutex_unlock(&client_lock);

    rpc_request req;
    rpc_response res;

    ssize_t r = read(client_fd, &req, sizeof(req));
    if (r <= 0) {
        close(client_fd);

        // ---- SINCRONIZARE: decrementăm la deconectare ----
        pthread_mutex_lock(&client_lock);
        client_count--;
        printf("[Server] Client deconectat. Clienti activi: %d\n", client_count);
        pthread_mutex_unlock(&client_lock);

        pthread_exit(NULL);
    }

    printf("[Server] Primit apel RPC: %s(%d, %d)\n", req.func, req.arg1, req.arg2);

    res.err_code = 0;

    if (strcmp(req.func, "add") == 0)
        res.result = add(req.arg1, req.arg2);
    else if (strcmp(req.func, "sub") == 0)
        res.result = sub(req.arg1, req.arg2);
    else {
        res.err_code = 1;
        res.result = 0;
    }

    write(client_fd, &res, sizeof(res));
    close(client_fd);

    printf("[Server] Rezultat trimis pentru %s(%d,%d): %d\n",
           req.func, req.arg1, req.arg2, res.result);

    // ---- SINCRONIZARE: decrementăm numărul de clienți ----
    pthread_mutex_lock(&client_lock);
    client_count--;
    printf("[Server] Client deconectat. Clienti activi: %d\n", client_count);
    pthread_mutex_unlock(&client_lock);

    pthread_exit(NULL);
}

// ============================
// Funcția principală server
// ============================
int main() {
    int server_fd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Eroare socket");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Eroare bind");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Eroare listen");
        close(server_fd);
        exit(1);
    }

    printf("[Server] Pornit și ascultă pe portul %d\n", PORT);

    while (1) {
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr*)&cli_addr, &cli_len);

        if (*client_fd < 0) {
            perror("Eroare accept");
            free(client_fd);
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid);  // threadul se curăță singur la final
    }

    close(server_fd);
    return 0;
}
