#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return -1;
    }
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    int bindresult = bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (bindresult == -1) {
        perror("bind");
        return -1;
    }
    int listenresult = listen(server_fd, 5);
    if (listenresult == -1) {
        perror("Listen");
        return -1;
    }
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    printf("waiting for connection...\n");
    while (1) {
        char buffer[1024];
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd == -1) {
            perror("accept");
            return -1;
        }

        while (1) {
            int bytes_read = read(client_fd, buffer, sizeof(buffer));
            buffer[bytes_read] = '\0';
            if (bytes_read <= 0) break;
            printf("received: %s", buffer);
        }
        
        close(client_fd);
    }
    return 0;
}