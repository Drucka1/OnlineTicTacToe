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

void Game::init(){
    this->turn = Player::X;
    this->tour = 0;
    for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
            this->map[i][j] = Player::None;
        }
    }
}

void Game::play(int i, int j, Player player){
    if (this->map[i][j] == Player::None && player == this->turn){
        this->map[i][j] = player;
        this->tour++;
        this->turn = player == Player::O ? Player::X : Player::O;
    }
}

bool Game::is_ongoing(){
    return winner() == Player::None || this->tour != 9;
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

void Game::menu(RenderWindow* window) {
    RectangleShape background(Vector2f(WINDOW, WINDOW));
    background.setFillColor(Color(0, 0, 0, 192));
    window->draw(background);

    Font font; 
    font.loadFromFile("./font/MegamaxJonathanToo-YqOq2.ttf");

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

void Game::run(RenderWindow* window, int socket, Player player){
    Event event;

    while (window->isOpen()) {
        if (player != this->turn){
            Move move;
            draw(window);
            window->display();
            if (read(socket,&move,sizeof(Move)) < 0){
                perror("Échec lors de la lecture du socket");
            }
            cout << move.i << " " << move.j << endl;
            play(move.i,move.j,player == Player::O ? Player::X : Player::O);
        }
        else {
            while (window->pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window->close();
                }
            }
            if (is_ongoing()){
                if (Mouse::isButtonPressed(Mouse::Right)){
                    Vector2i pos = Mouse::getPosition(*window);
                    
                    Vector2i case_pos = Vector2i(pos.x / (WINDOW/3),pos.y / (WINDOW/3));
                    if (case_pos.x <=3 && case_pos.y <= 3 && 0 <= case_pos.x && 0 <= case_pos.y){
                        play(case_pos.x,case_pos.y,player);
                        Move move = {case_pos.x,case_pos.y};
                        send(socket,&move,sizeof(Move),0);
                    }
                    
                }
                window->clear();
                draw(window);
            }
            else {
                window->clear();
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
                        init();                 
                    }
                }
            }        
            window->display();
        }
    }
}