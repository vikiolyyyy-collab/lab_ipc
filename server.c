#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 64

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    char buf[BUF_SIZE];
    ssize_t n;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    printf("Client connected\n");

    memset(buf, 0, BUF_SIZE);
    n = read(client_fd, buf, BUF_SIZE - 1);
    if (n < 0) {
        perror("read");
        close(client_fd);
        close(server_fd);
        return 1;
    }
    if (n == 0) {
        printf("Client closed connection\n");
        close(client_fd);
        close(server_fd);
        return 0;
    }

    buf[n] = '\0';
    printf("Got: %s", buf);

    if (strncmp(buf, "ping", 4) == 0) {
        if (write(client_fd, "pong\n", 5) < 0) {
            perror("write");
            close(client_fd);
            close(server_fd);
            return 1;
        }
        printf("Sent: pong\n");
    } else {
        printf("Not a ping, ignore\n");
    }

    close(client_fd);
    close(server_fd);
    printf("Done\n");
    return 0;
}
