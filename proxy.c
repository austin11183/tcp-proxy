#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
int main() {
    // === 階段 1: 開啟socket ===
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return -1;
    }
    int opt = 1;
    // 設置socket選項(可重複使用的地址)
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // === 階段 2: 命名socket(綁定) ===
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    int bindresult = bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (bindresult == -1) {
        perror("bind");
        return -1;
    }
    
    // === 階段 3: 連結(監聽) ===
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
        // === 階段 3: 連結(接受連接) ===
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd == -1) {
            perror("accept");
            return -1;
        }

        // === 階段 4: 傳輸(接收資料) ===
        while (1) {
            int bytes_read = read(client_fd, buffer, sizeof(buffer));
            if (bytes_read <= 0) break;
            buffer[bytes_read] = '\0';
            printf("received: %s", buffer);
        }
        
        // === 階段 5: 停止(關閉連接) ===
        close(client_fd);
    }
    return 0;
}