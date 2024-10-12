#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "game.h"

using namespace std;
using namespace sf;

void draw_menu(RenderWindow* window) {
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

int main(){
    RenderWindow window(VideoMode(WINDOW+2,WINDOW+2), "TicTac",Style::Close);
    Event event;
    Game game;
    game.init();

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (game.is_ongoing()){
            if (Mouse::isButtonPressed(Mouse::Left)){
                Vector2i pos = Mouse::getPosition(window);
                Vector2i case_pos = Vector2i(pos.x / (WINDOW/3),pos.y / (WINDOW/3));
                game.move(case_pos.x,case_pos.y,Player::O);
            }
            if (Mouse::isButtonPressed(Mouse::Right)){
                Vector2i pos = Mouse::getPosition(window);
                Vector2i case_pos = Vector2i(pos.x / (WINDOW/3),pos.y / (WINDOW/3));
                game.move(case_pos.x,case_pos.y,Player::X);
            }
            window.clear();
            game.draw(&window);
        }
        else {
            window.clear();
            game.draw(&window);
            draw_menu(&window);

            if (Mouse::isButtonPressed(Mouse::Left)){
                Vector2i pos = Mouse::getPosition(window);

                RectangleShape menu(Vector2f(206,46));
                menu.setPosition(199,204);
                

                RectangleShape new_game(Vector2f(416,47));
                new_game.setPosition(94,353);

                if (menu.getGlobalBounds().contains(pos.x,pos.y)) window.close();

                if (new_game.getGlobalBounds().contains(pos.x,pos.y)) {
                    game.init();                 
                }

            }
        }
    
        
        window.display();
    }
    return 0;
}
