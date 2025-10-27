#include "rpc_common.h"



int add(int a, int b) { return a+b; }
int sub(int a, int b) { return a-b; }


int main()
{
    int server_fd;
    int client_fd;
    struct sockaddr_in serv_addr,cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    rpc_request req;
    rpc_response res;

    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd < 0)
    {
        perror("eroare socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("eroare bind");
        close(server_fd);
        exit(1);
    }


    listen(server_fd,3);
    printf("Serverul asculta pe port %d\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&cli_addr, &cli_len);
        if (client_fd < 0) { 
            perror("accept"); continue; }

        read(client_fd, &req, sizeof(req));
        printf("Primit apel RPC: %s(%d, %d)\n", req.func, req.arg1, req.arg2);

        res.err_code = 0;
        if (strcmp(req.func, "add") == 0) {
            res.result = add(req.arg1, req.arg2);
        } else if (strcmp(req.func, "sub") == 0) {
            res.result = sub(req.arg1, req.arg2);
        } 
        else {
            res.err_code = 1;
            res.result = 0;
        }

        write(client_fd, &res, sizeof(res));
        close(client_fd);
    }

    close(server_fd);
    return 0;
}

