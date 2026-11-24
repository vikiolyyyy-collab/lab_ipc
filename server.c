#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 256
#define HISTORY_FILE "history.txt"

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    char buf[BUF_SIZE];
    ssize_t n;
    FILE *hist = NULL;

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

    hist = fopen(HISTORY_FILE, "a");
    if (hist == NULL) {
        perror("fopen history");
    }

    while (1) {
        memset(buf, 0, BUF_SIZE);
        n = read(client_fd, buf, BUF_SIZE - 1);
        if (n < 0) {
            perror("read");
            break;
        }
        if (n == 0) {
            printf("Client disconnected\n");
            break;
        }

        buf[n] = '\0';
        printf("Got: %s", buf);

        if (hist != NULL) {
            fprintf(hist, "%s", buf);
            fflush(hist);
        }

        if (strncmp(buf, "exit", 4) == 0) {
            printf("Exit requested\n");
            break;
        }

        if (write(client_fd, buf, (size_t)n) < 0) {
            perror("write");
            break;
        }
        printf("Echo sent\n");
    }

    if (hist != NULL) {
        fclose(hist);
    }
    close(client_fd);
    close(server_fd);
    printf("Done\n");
    return 0;
}
