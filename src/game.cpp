#include "game.h"
#include <SFML/Graphics.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

using namespace sf;
using namespace std;

void Game::init(Player player) {
    return_match();

    if (player == Player::X) {//Server
        TcpListener server;
        IpAddress addr = IpAddress::Any;

        if (server.listen(PORT, addr) != sf::Socket::Done) {
            std::cerr << "Erreur lors de l'écoute sur le port " << PORT << std::endl;
            return;
        }

        std::cout << "En attente de connexion..." << std::endl;

        if (server.accept(this->opponent) != sf::Socket::Done) {
            std::cerr << "Erreur lors de l'acceptation de la connexion." << std::endl;
            return; 
        }

        this->opponent.setBlocking(false);
        std::cout << "Client connecté : " << this->opponent.getRemoteAddress() << std::endl;

        server.close(); 
    } else {//Client        
        if (this->opponent.connect("192.168.91.252", PORT) != sf::Socket::Done) {
            std::cerr << "Erreur de connexion au serveur sur le port " << PORT << std::endl;
            return;
        }

        this->opponent.setBlocking(false);
        std::cout << "Connecté au serveur." << std::endl;
    }
}

void Game::play(int i, int j, Player player){
    if (this->map[i][j] == Player::None && player == this->turn && 0 <= i && i <= 3 && 0 <= j && j <= 3){
        this->map[i][j] = player;
        this->tour++;
        this->turn = player == Player::O ? Player::X : Player::O;
    }
}

bool Game::is_ongoing(){
    return winner() == Player::None && this->tour < 9;
}

void Game::return_match(){
    this->turn = Player::X;
    this->tour = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->map[i][j] = Player::None;
        }
    }
}

Player Game::winner(){
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

void Game::draw(RenderWindow* window){
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

void Game::accept_return_match(RenderWindow* window){
    RectangleShape background(Vector2f(WINDOW, WINDOW));
    background.setFillColor(Color(0, 0, 0, 192));
    window->draw(background);

    static Font font; 
    static bool fontLoaded = false;
    if (!fontLoaded) {
        if (!font.loadFromFile("./font/MegamaxJonathanToo-YqOq2.ttf")) {
            std::cerr << "Erreur de chargement de la police." << std::endl;
        }
        fontLoaded = true;
    }
    Text menu;
    menu.setFont(font); 
    menu.setString("Menu");
    menu.setCharacterSize(72);
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
    yes.setCharacterSize(72);
    yes.setFillColor(Color::White);
    yes.setPosition(WINDOW / 3, 3*WINDOW / 4);
    window->draw(yes);

    Text no;
    no.setFont(font); 
    no.setString("NO");
    no.setCharacterSize(72);
    no.setFillColor(Color::White);
    no.setPosition(2*WINDOW / 3, 3*WINDOW / 4);
    window->draw(no);
}

void Game::menu(RenderWindow* window) {
    RectangleShape background(Vector2f(WINDOW, WINDOW));
    background.setFillColor(Color(0, 0, 0, 192));
    window->draw(background);

    static Font font; 
    static bool fontLoaded = false;
    if (!fontLoaded) {
        if (!font.loadFromFile("./font/MegamaxJonathanToo-YqOq2.ttf")) {
            std::cerr << "Erreur de chargement de la police." << std::endl;
        }
        fontLoaded = true;
    }

    Text menu;
    menu.setFont(font); 
    menu.setString("Menu");
    menu.setCharacterSize(72);
    menu.setFillColor(Color::White);
    menu.setPosition((WINDOW +2 - menu.getGlobalBounds().width) / 2, WINDOW /4+26);
    window->draw(menu);

    Text text;
    text.setFont(font); 
    text.setString("New Game");
    text.setCharacterSize(72);
    text.setFillColor(Color::White);
    text.setPosition((WINDOW +2- text.getGlobalBounds().width) / 2, 2*WINDOW / 4+26);
    window->draw(text);
}

void Game::run(RenderWindow* window, Player player){
    init(player);
    Event event;
    bool rematch = false;

    while (window->isOpen()) {
        while (window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                window->close();
                //send quit
            }
        }
        
        window->clear();

        if (!is_ongoing()){

            if (!rematch){
                draw(window);
                menu(window);

                if (Mouse::isButtonPressed(Mouse::Left)){
                    Vector2i pos = Mouse::getPosition(*window);

                    RectangleShape menu(Vector2f(206,46));
                    menu.setPosition(199,204);
                    
                    RectangleShape new_game(Vector2f(416,47));
                    new_game.setPosition(94,353);

                    if (menu.getGlobalBounds().contains(pos.x,pos.y)) window->close();

                    if (new_game.getGlobalBounds().contains(pos.x,pos.y)) {
                        Move move = {-1,-1};
                        opponent.send(&move,sizeof(Move));
                        return_match();                 
                    }
                }
                Move move;
                size_t received = sizeof(Move);
                opponent.receive(&move,sizeof(Move),received);
                if (move.i == -1 && received == sizeof(Move)) {
                    rematch = true;
                    cout << "rematch ?" << endl;
                }

            }
            else {
                draw(window);
                accept_return_match(window);

                if (Mouse::isButtonPressed(Mouse::Left)){
                    Vector2i pos = Mouse::getPosition(*window);

                    RectangleShape menu(Vector2f(206,46));
                    menu.setPosition(199,204);
                    
                    RectangleShape yes(Vector2f(50,47));
                    yes.setPosition(WINDOW / 3, 3*WINDOW / 4);

                    RectangleShape no(Vector2f(50,47));
                    no.setPosition(2*WINDOW / 3, 3*WINDOW / 4);

                    if (menu.getGlobalBounds().contains(pos.x,pos.y)) window->close();

                    if (yes.getGlobalBounds().contains(pos.x,pos.y)) {
                        return_match();
                        rematch = false;                 
                    }

                    if (no.getGlobalBounds().contains(pos.x,pos.y)) {
                        //      rematch = false;             
                    }
                }


            }
        }  

        else if (player == this->turn){
            if (Mouse::isButtonPressed(Mouse::Right)){
                Vector2i pos = Mouse::getPosition(*window);
                Vector2i case_pos = Vector2i(pos.x / (WINDOW/3),pos.y / (WINDOW/3));
                Move move = {case_pos.x,case_pos.y};
                opponent.send(&move,sizeof(Move));
                play(case_pos.x,case_pos.y,player);
            }
            draw(window);
        } 
        
        else  {
            draw(window);
            window->display();

            Move move;
            size_t received = sizeof(Move);
            opponent.receive(&move,sizeof(Move),received);
            if (received == sizeof(Move)) play(move.i, move.j, player == Player::O ? Player::X : Player::O);
            
        }

        window->display();
    }
}
