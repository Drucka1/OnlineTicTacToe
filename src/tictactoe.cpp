#include <iostream>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "game.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


using namespace std;
using namespace sf;

const int PORT = 11111;

int start_server() {
    int server_socket, socket_to_client(0);
    struct sockaddr_in address;
    
    // Création du socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        perror("Échec de la création du socket");
        return -1;
    }

    // Configuration de l'adresse
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Accepte les connexions de n'importe quelle adresse
    address.sin_port = htons(PORT);

    // Lier le socket
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Échec de la liaison");
        return -1;
    }

    if (listen(server_socket, 1) < 0) {
        perror("Échec de l'écoute");
        return -1;
    }

    cout << "En attente d'un adversaire" << endl;
    int addrlen = sizeof(address);
    
    socket_to_client = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    int flags = fcntl(server_socket, F_GETFL, 0);
    if (fcntl(socket_to_client, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("Échec de la configuration du socket en mode non-bloquant");
        return -1;
    }
    cout << "Adversaire trouvé" << endl;

    close(server_socket);
    return socket_to_client;
}

int start_client() {
    int client_socket(0);
    struct sockaddr_in serv_addr;

    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr))< 0) {
        perror("Échec lors de la connexion au serveur");
        return -1;
    }

    int flags = fcntl(client_socket, F_GETFL, 0);
    if (fcntl(client_socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("Échec de la configuration du socket en mode non-bloquant");
        return -1;
    }
    cout << "Adversaire trouvé" << endl;

    return client_socket;
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
