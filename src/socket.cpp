#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8080;

void start_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Création du socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Configuration de l'adresse
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accepte les connexions de n'importe quelle adresse
    address.sin_port = htons(PORT);

    // Lier le socket
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Serveur en attente de connexion..." << std::endl;
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    std::cout << "Connexion acceptée." << std::endl;

    read(new_socket, buffer, 1024);
    std::cout << "Message reçu: " << buffer << std::endl;

    const char *response = "Message reçu!";
    send(new_socket, response, strlen(response), 0);
    
    // Fermer le socket
    close(new_socket);
    close(server_fd);
}

void start_client(const char *server_ip) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char message[] = "Hello from client";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convertir l'adresse IPv4 et l'adresse IPv6 en une forme utilisable
    inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);

    // Se connecter au serveur
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock, message, strlen(message), 0);
    std::cout << "Message envoyé au serveur." << std::endl;

    // Fermer le socket
    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <server|client> [server_ip]" << std::endl;
        return 1;
    }

    if (strcmp(argv[1], "server") == 0) {
        start_server();
    } else if (strcmp(argv[1], "client") == 0 && argc == 3) {
        start_client(argv[2]);
    } else {
        std::cerr << "Arguments invalides." << std::endl;
        return 1;
    }

    return 0;
}
