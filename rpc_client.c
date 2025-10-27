#include "rpc_common.h"

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    rpc_request req;
    rpc_response res;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET,"10.215.33.247",&serv_addr.sin_addr);
    while(1)
    {
        sock = socket(AF_INET,SOCK_STREAM,0);
        if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
        {
            perror("eroare la conn");
            exit(1);
        }
        char buffer[30];
        printf("Ce vrei sa executi(add/sub)?\n");
        scanf("%s",&buffer);
        strcpy(req.func,buffer);
        printf("Ce argumente vrei sa ai?\n");
        printf("Arg1: ");
        int arg1,arg2;
        scanf("%d",&arg1);
        printf("\n");
        printf("Arg2: ");
        scanf("%d", &arg2);
        printf("\n");
        req.arg1 = arg1;
        req.arg2 = arg2;
        //scriem pe socket
        write(sock,&req,sizeof(req));
        //citim pe socket
        read(sock,&res,sizeof(res));

        if(res.err_code == 0)
        {
            printf("Rezultatul functiei %s este : %d",req.func,res.result);
            printf("\n");
        }
        else
        {
            printf("eroare functie rpc, nu exista\n");
        }
        close(sock);
    }
    return 0;
}