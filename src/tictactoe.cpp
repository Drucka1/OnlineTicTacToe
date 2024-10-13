#include <iostream>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "game.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;
using namespace sf;

const int PORT = 8080;



// Après chaque appel critique




int start_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    
    // Création du socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Échec de la création du socket");
        return -1;
    }

    // Configuration de l'adresse
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Accepte les connexions de n'importe quelle adresse
    address.sin_port = htons(PORT);

    // Lier le socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Échec de la liaison");
        return -1;
    }

    if (listen(server_fd, 1) < 0) {
        perror("Échec de l'écoute");
        return -1;
    }

    cout << "En attente d'un adversaire" << endl;
    int addrlen = sizeof(address);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    cout << "Adversaire trouvé" << endl;

    close(server_fd);
    return new_socket;
}

int start_client() {
    int sock;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))< 0) {
        perror("Échec lors de la connexion au serveur");
        return -1;
    }
    cout << "Adversaire trouvé" << endl;

    return sock;
}

int main(int argc, char *argv[]){
    if (argc < 1) {
        std::cerr << "Usage: " << argv[0] << " <server|client>" << std::endl;
        return 1;
    }
    
    RenderWindow window(VideoMode(WINDOW+2,WINDOW+2), "TicTac",Style::Close);
    Game game;
    game.init();
    int socket;

    if (strcmp(argv[1], "server") == 0) {
        socket = start_server();
        game.run(&window,socket,Player::X);
        close(socket);
    } else if (strcmp(argv[1], "client") == 0) {
        socket = start_client();
        game.run(&window,socket,Player::O);
        close(socket);
    }

    return 0;
}
