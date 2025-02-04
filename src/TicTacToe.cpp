#include "TicTacToe.h"
#include <SFML/Graphics.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

using namespace sf;
using namespace std;

void TicTacToe::init(){
    if (!this->font.loadFromFile("./font/MegamaxJonathanToo-YqOq2.ttf")) {
        cerr << "Erreur de chargement de la police." << endl;
    }
    this->window = GameWindow::Menu;
}

void TicTacToe::init_match(){
    this->turn = Player::X;
    this->tour = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->map[i][j] = Player::None;
        }
    }
}

string TicTacToe::get_local_ip(){
    struct ifaddrs *interfaces = nullptr;
    struct ifaddrs *ifa = nullptr;
    char ip[INET_ADDRSTRLEN];
    string local_ip = "";

    if (getifaddrs(&interfaces) == -1) {
        return local_ip;
    }

    for (ifa = interfaces; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *sock_addr = (struct sockaddr_in *)ifa->ifa_addr;
            if (inet_ntop(AF_INET, &sock_addr->sin_addr, ip, sizeof(ip)) != nullptr) {
                if (strcmp(ifa->ifa_name, "lo") != 0) {
                    local_ip = string(ip);
                    break;
                }
            }
        }
    }

    if (interfaces != nullptr) {
        freeifaddrs(interfaces);
    }

    return local_ip;
}

void TicTacToe::init_socket(Player player) {
    init_match();

    this->player = player;

    if (player == Player::X) {//Server
        TcpListener server;
        IpAddress addr = IpAddress::Any;

        if (server.listen(PORT, addr) != sf::Socket::Done) {
            cerr << "Erreur lors de l'écoute sur le port " << PORT << endl;
            return;
        }

        cout << "En attente de connexion..." << endl;

        if (server.accept(this->opponent) != sf::Socket::Done) {
            cerr << "Erreur lors de l'acceptation de la connexion." << endl;
            return; 
        }

        this->opponent.setBlocking(false);
        cout << "Client connecté : " << this->opponent.getRemoteAddress() << endl;

        server.close(); 
    } else {//Client        
        if (this->opponent.connect(get_local_ip(), PORT) != sf::Socket::Done) {
            cerr << "Erreur de connexion au serveur sur le port " << PORT << endl;
            return;
        }

        this->opponent.setBlocking(false);
        cout << "Connecté au serveur." << endl;
    }
}

void TicTacToe::play(int i, int j, Player player){
    if (this->map[i][j] == Player::None && player == this->turn && 0 <= i && i <= 3 && 0 <= j && j <= 3){
        this->map[i][j] = player;
        this->tour++;
        this->turn = player == Player::O ? Player::X : Player::O;
        if (!is_ongoing()) this->window = GameWindow::End;
    }
}

bool TicTacToe::is_ongoing(){
    return winner() == Player::None && this->tour < 9;
}

Player TicTacToe::winner(){
    if (tour < 5) return Player::None;

    for(int i =0;i<3;i++){
        if (this->map[0][i] == Player::O && this->map[1][i] == Player::O && this->map[2][i] == Player::O) return Player::O;
        if (this->map[i][0] == Player::O && this->map[i][1] == Player::O && this->map[i][2] == Player::O) return Player::O;
        if (this->map[i][0] == Player::X && this->map[i][1] == Player::X && this->map[i][2] == Player::X) return Player::X;
        if (this->map[0][i] == Player::X && this->map[1][i] == Player::X && this->map[2][i] == Player::X) return Player::X;
    }
    if (this->map[0][0] == Player::O && this->map[1][1] == Player::O && this->map[2][2] == Player::O) return Player::O;
    if (this->map[2][0] == Player::O && this->map[1][1] == Player::O && this->map[0][2] == Player::O) return Player::O;
    if (this->map[0][0] == Player::X && this->map[1][1] == Player::X && this->map[2][2] == Player::X) return Player::X;
    if (this->map[2][0] == Player::X && this->map[1][1] == Player::X && this->map[0][2] == Player::X) return Player::X;
    return Player::None;
}

void TicTacToe::draw(RenderWindow* window){
    Vertex line1[] = {
        Vertex(Vector2f(WINDOW/3+1, 0)),
        Vertex(Vector2f(WINDOW/3+1, WINDOW+2))
    };
    (*window).draw(line1, 2, Lines);
    Vertex line2[] = {
        Vertex(Vector2f(2*WINDOW/3+1, 0)),
        Vertex(Vector2f(2*WINDOW/3+1, WINDOW+2))
    };
    (*window).draw(line2, 2, Lines);
    Vertex line3[] = {
        Vertex(Vector2f(0,WINDOW/3+1)),
        Vertex(Vector2f(WINDOW+2,WINDOW/3+1))
    };
    (*window).draw(line3, 2, Lines);
    Vertex line4[] = {
        Vertex(Vector2f(0,2*WINDOW/3+1)),
        Vertex(Vector2f( WINDOW+2,2*WINDOW/3+1))
    };
    (*window).draw(line4, 2, Lines);

    for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
            if (map[i][j] == Player::O){
                CircleShape circle;
                circle.setPosition(50 + i*WINDOW/3,50 +  j*WINDOW/3);
                circle.setFillColor(Color::Black);
                circle.setOutlineColor(Color::Blue);
                circle.setOutlineThickness(25);
                circle.setRadius(WINDOW/6 - 50);
                (*window).draw(circle);
            }
            if (map[i][j] == Player::X){              
                RectangleShape horizontal(Vector2f(100 * 2, 25));
                horizontal.setFillColor(Color::Red);
                horizontal.setOrigin(100, 25 / 2);
                horizontal.setPosition(WINDOW / 6 + i*WINDOW/3 , WINDOW / 6 + j*WINDOW/3);

                RectangleShape vertical(Vector2f(25, 100 * 2));
                vertical.setFillColor(Color::Red);
                vertical.setOrigin(25 / 2, 100);
                vertical.setPosition(WINDOW / 6 + i*WINDOW/3 , WINDOW / 6 + j*WINDOW/3);

                horizontal.setRotation(45.0f);
                vertical.setRotation(45.0f);

                (*window).draw(horizontal);
                (*window).draw(vertical);
            }
        }
    }
}

void TicTacToe::accept_rematch(RenderWindow* window){
    RectangleShape background(Vector2f(WINDOW, WINDOW));
    background.setFillColor(Color(0, 0, 0, 192));
    window->draw(background);

    Text menu;
    menu.setFont(font); 
    menu.setString("Menu");
    menu.setCharacterSize(48);
    menu.setFillColor(Color::White);
    menu.setPosition((WINDOW +2 - menu.getGlobalBounds().width) / 2, WINDOW /4+26);
    window->draw(menu);

    Text text;
    text.setFont(font); 
    text.setString("accept rematch ?");
    text.setCharacterSize(48);
    text.setFillColor(Color::White);
    text.setPosition((WINDOW +2- text.getGlobalBounds().width) / 2, 2*WINDOW / 4+26);
    window->draw(text);

    Text yes;
    yes.setFont(font); 
    yes.setString("YES");
    yes.setCharacterSize(48);
    yes.setFillColor(Color::White);
    yes.setPosition(WINDOW / 3, 3*WINDOW / 4);
    window->draw(yes);

    Text no;
    no.setFont(font); 
    no.setString("NO");
    no.setCharacterSize(48);
    no.setFillColor(Color::White);
    no.setPosition(2*WINDOW / 3, 3*WINDOW / 4);
    window->draw(no);
    
    if (Mouse::isButtonPressed(Mouse::Left)){
        Vector2i pos = Mouse::getPosition(*window);

        RectangleShape menu(Vector2f(206,46));
        menu.setPosition(199,204);
        
        RectangleShape yes(Vector2f(100,47));
        yes.setPosition(WINDOW / 3, 3*WINDOW / 4);

        RectangleShape no(Vector2f(75,47));
        no.setPosition(2*WINDOW / 3, 3*WINDOW / 4);

        if (menu.getGlobalBounds().contains(pos.x,pos.y)) window->close();

        if (yes.getGlobalBounds().contains(pos.x,pos.y)) {
            send({MessageType::AnswerRematch,true});
        }

        if (no.getGlobalBounds().contains(pos.x,pos.y)) {
            send({MessageType::AnswerRematch,false});            
        }
    }
}

void TicTacToe::end(RenderWindow* window) {
    RectangleShape background(Vector2f(WINDOW, WINDOW));
    background.setFillColor(Color(0, 0, 0, 192));
    window->draw(background);

    Text menu;
    menu.setFont(this->font); 
    menu.setString("Menu");
    menu.setCharacterSize(48);
    menu.setFillColor(Color::White);
    menu.setPosition((WINDOW +2 - menu.getGlobalBounds().width) / 2, WINDOW /4+26);
    window->draw(menu);

    Text rematch;
    rematch.setFont(this->font); 
    rematch.setString("Rematch");
    rematch.setCharacterSize(48);
    rematch.setFillColor(Color::White);
    rematch.setPosition((WINDOW +2- rematch.getGlobalBounds().width) / 2, 2*WINDOW / 4+26);
    window->draw(rematch);
    
    if (Mouse::isButtonPressed(Mouse::Left)){
        Vector2i pos = Mouse::getPosition(*window);

        if (menu.getGlobalBounds().contains(pos.x,pos.y)) {
            send({MessageType::Quit});
        }
        if (rematch.getGlobalBounds().contains(pos.x,pos.y)) {
            send({MessageType::AskRematch});        
        }
    }
    
}

void TicTacToe::menu(RenderWindow* window){

    Text text;
    text.setFont(this->font); 
    text.setString("TicTacToe");
    text.setCharacterSize(72);
    text.setFillColor(Color::White);
    text.setPosition((WINDOW +2 - text.getGlobalBounds().width) / 2, WINDOW /4-36);
    window->draw(text);

    Text create;
    create.setFont(this->font); 
    create.setString("Create Game");
    create.setCharacterSize(48);
    create.setFillColor(Color::White);
    create.setPosition((WINDOW +2- create.getGlobalBounds().width) / 2, 2*WINDOW / 4);
    window->draw(create);

    Text join;
    join.setFont(this->font); 
    join.setString("Join Game");
    join.setCharacterSize(48);
    join.setFillColor(Color::White);
    join.setPosition((WINDOW +2- join.getGlobalBounds().width) / 2, 3*WINDOW / 4);
    window->draw(join);

    if (Mouse::isButtonPressed(Mouse::Left)){
        Vector2i pos = Mouse::getPosition(*window);
        if (join.getGlobalBounds().contains(pos.x,pos.y)) {
            init_socket(Player::O); 
            this->window = GameWindow::Play;   
        }
        if (create.getGlobalBounds().contains(pos.x,pos.y)){
            init_socket(Player::X);
            this->window = GameWindow::Play;
        }
    }
}

void TicTacToe::run(RenderWindow* window){
    Event event;
    init();

    while (window->isOpen()) {
        
        read();

        while (window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                window->close();
                send({MessageType::Quit});
            }
        }

        window->clear();
        switch (this->window)
        {
        case GameWindow::Menu:
            menu(window);
            break;

        case GameWindow::Credit:
            //
            break;

        case GameWindow::End:
            draw(window);
            end(window);
      
            break;
        
        case GameWindow::Play:
            if (this->player == this->turn){
                if (Mouse::isButtonPressed(Mouse::Right)){
                    Vector2i pos = Mouse::getPosition(*window);
                    Vector2i case_pos = Vector2i(pos.x / (WINDOW/3),pos.y / (WINDOW/3));
                    send({MessageType::Move,{case_pos.x,case_pos.y}});
                }
            } 
            draw(window);
            break;

        case GameWindow::Rematch:
            draw(window);
            accept_rematch(window);
            break;

        }
        window->display();
    }
}


void TicTacToe::send(Message msg){
    
    opponent.send(&msg,sizeof(Message));  
    switch (msg.type)
    {
    case MessageType::Move:
        play(msg.data.move.i, msg.data.move.j, this->player); 
        break;

    case MessageType::Quit:
        this->opponent.disconnect();
        this->window = GameWindow::Menu;        
        break;

    case MessageType::AskRematch:
        
        break;

    case MessageType::AnswerRematch:
         
        if (msg.data.answerRematch) {
            init_match();
            this->window = GameWindow::Play;
        } 
        else {
            this->window = GameWindow::Menu;
            this->opponent.disconnect();
        }      
        break;
    
    default:
        break;
    }

}

void TicTacToe::read(){
    Message msg;
    size_t received = sizeof(Message);
    opponent.receive(&msg,sizeof(Message),received);
    if (received != sizeof(Message)) return;

    switch (msg.type)
    {
    case MessageType::Move:
        play(msg.data.move.i, msg.data.move.j, this->player == Player::O ? Player::X : Player::O);        
        break;

    case MessageType::Quit:
        this->window = GameWindow::Menu;
        this->opponent.disconnect();
        cout << "L'adversaire a quitte la parti" << endl;
        break;

    case MessageType::AskRematch:
        this->window = GameWindow::Rematch;
        break;

    case MessageType::AnswerRematch:
        if (msg.data.answerRematch) {
            init_match();
            this->window = GameWindow::Play;
        } else {
            this->window = GameWindow::Menu;
            this->opponent.disconnect();
            cout << "L'adversaire a ne souhaite pas rejouer" << endl;
        }        
        break;
    
    default:
        break;
    }
    
}

int main() {
    RenderWindow window(VideoMode(WINDOW + 2, WINDOW + 2), "TicTacToe", Style::Close);
    TicTacToe game;
    game.run(&window);

    return 0;
}