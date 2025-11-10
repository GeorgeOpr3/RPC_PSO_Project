#include "rpc_lib.h"
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>

int rpc_send(int sock, const void *data, size_t len) {
    size_t total = 0;
    const char *ptr = data;
    while (total < len) {
        ssize_t n = send(sock, ptr + total, len - total, 0);
        if (n <= 0) return -1;
        total += n;
    }
    return 0;
}

int rpc_recv(int sock, void *buf, size_t len, int timeout_ms) {
    fd_set fds;
    struct timeval tv;
    FD_ZERO(&fds);
    FD_SET(sock, &fds);
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    int rv = select(sock + 1, &fds, NULL, NULL, &tv);
    if (rv == 0) return -2; // timeout
    if (rv < 0) return -1;

    ssize_t n = recv(sock, buf, len, 0);
    if (n <= 0) return -3;
    return (int)n;
}

int rpc_connect(const char *ip, int port, int timeout_ms) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);

    // timeout la connect
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sock);
        return -1;
    }
    return sock;
}

bool rpc_retry_connect(const char *ip, int port, int max_retries, int delay_ms) {
    for (int i = 0; i < max_retries; i++) {
        int s = rpc_connect(ip, port, 2000);
        if (s >= 0) {
            close(s);
            return true;
        }
        usleep(delay_ms * 1000);
    }
    return false;
}
