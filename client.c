#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 256

int main(void) {
    int sock_fd;
    struct sockaddr_in addr;
    char buf[BUF_SIZE];
    ssize_t n;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock_fd);
        return 1;
    }

    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock_fd);
        return 1;
    }

    printf("Connected to server\n");
    printf("Type messages (exit to quit)\n");

    while (1) {
        printf("> ");
        fflush(stdout);

        if (fgets(buf, BUF_SIZE, stdin) == NULL) {
            break;
        }

        n = (ssize_t)strlen(buf);
        if (write(sock_fd, buf, (size_t)n) < 0) {
            perror("write");
            break;
        }

        if (strncmp(buf, "exit", 4) == 0) {
            break;
        }

        memset(buf, 0, BUF_SIZE);
        n = read(sock_fd, buf, BUF_SIZE - 1);
        if (n < 0) {
            perror("read");
            break;
        }
        if (n == 0) {
            printf("Server closed connection\n");
            break;
        }

        buf[n] = '\0';
        printf("Server: %s", buf);
    }

    close(sock_fd);
    printf("Done\n");
    return 0;
}
