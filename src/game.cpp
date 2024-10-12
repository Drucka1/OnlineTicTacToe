#include "game.h"
#include <SFML/Graphics.hpp>
using namespace sf;

void Game::init(){
    this->turn = Player::X;
    this->tour = 0;
    for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
            this->map[i][j] = Player::None;
        }
    }
}

void Game::move(int i, int j, Player turn){
    if (this->map[i][j] == Player::None && turn == this->turn){
        this->map[i][j] = turn;
        this->tour++;
        switch (turn) {
            case Player::O:
                this->turn = Player::X;
                break;
            case Player::X:
                this->turn = Player::O;
                break;
            default:
                break;
        }
    }
}

bool Game::is_ongoing(){
    return winner() == Player::None;
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