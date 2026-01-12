#include "rpc_common.h"
#include <pthread.h>

int client_count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }

void *handle_client(void *arg) {
    int client_fd = *(int*)arg;
    free(arg);

    pthread_mutex_lock(&lock);
    client_count++;
    printf("[Server] Client conectat. Activi: %d\n", client_count);
    pthread_mutex_unlock(&lock);

    rpc_request req;
    rpc_response res;

    read(client_fd, &req, sizeof(req));

    printf("[Server] Cerere %s(%d,%d) | priority=%s\n",
           req.func, req.arg1, req.arg2,
           req.priority == PRIORITY_HIGH ? "HIGH" : "LOW");

    if (req.priority == PRIORITY_LOW)
        sleep(3);   // simulare prioritate joasa

    res.err_code = 0;
    if (!strcmp(req.func, "add"))
        res.result = add(req.arg1, req.arg2);
    else if (!strcmp(req.func, "sub"))
        res.result = sub(req.arg1, req.arg2);
    else {
        res.err_code = 1;
        res.result = 0;
    }

    write(client_fd, &res, sizeof(res));
    close(client_fd);

    pthread_mutex_lock(&lock);
    client_count--;
    printf("[Server] Client deconectat. Activi: %d\n", client_count);
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main() {
    int server_fd;
    struct sockaddr_in addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);

    printf("[Server] Pornit pe portul %d\n", PORT);

    while (1) {
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, NULL, NULL);

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid);
    }
}
