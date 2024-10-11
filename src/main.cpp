#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int const WINDOW = 600;

int winner( int game[3][3]){
    for(int i =0;i<3;i++){
        if (game[0][i] == 1 && game[1][i] == 1 && game[2][i] == 1) return 1;
        if (game[i][0] == 1 && game[i][1] == 1 && game[i][2] == 1) return 1;
        if (game[i][0] == 2 && game[i][1] == 2 && game[i][2] == 2) return 2;
        if (game[0][i] == 2 && game[1][i] == 2 && game[2][i] == 2) return 2;
    }
    if (game[0][0] == 1 && game[1][1] == 1 && game[2][2] == 1) return 1;
    if (game[2][0] == 1 && game[1][1] == 1 && game[0][2] == 1) return 1;
    if (game[0][0] == 2 && game[1][1] == 2 && game[2][2] == 2) return 2;
    if (game[2][0] == 2 && game[1][1] == 2 && game[0][2] == 2) return 2;
    return 0;
}

void draw_map(RenderWindow* window, int jeu[3][3]){
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
            if (jeu[i][j] == 1){
                CircleShape circle;
                circle.setPosition(50 + i*WINDOW/3,50 +  j*WINDOW/3);
                circle.setFillColor(Color::Black);
                circle.setOutlineColor(Color::Blue);
                circle.setOutlineThickness(25);
                circle.setRadius(WINDOW/6 - 50);
                (*window).draw(circle);
            }
            if (jeu[i][j] == 2){              
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

    int jeu[3][3];
    for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
            jeu[i][j] = 0;
        }
    }
    bool player = false;
    int tour = 0;
    Event event;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (tour != -1){
            if (Mouse::isButtonPressed(Mouse::Left)){
                Vector2i pos = Mouse::getPosition(window);
                Vector2i case_pos = Vector2i(pos.x / (WINDOW/3),pos.y / (WINDOW/3));
                if (!jeu[case_pos.x][case_pos.y] && player){
                    player = !player;
                    tour++;
                    jeu[case_pos.x][case_pos.y] = 1;
                }
            }
            if (Mouse::isButtonPressed(Mouse::Right)){
                Vector2i pos = Mouse::getPosition(window);
                Vector2i case_pos = Vector2i(pos.x / (WINDOW/3),pos.y / (WINDOW/3));
                if (!jeu[case_pos.x][case_pos.y] && !player){
                    player = !player;
                    tour++;
                    jeu[case_pos.x][case_pos.y] = 2;
                }
            }
            window.clear();
            draw_map(&window, jeu);
        }
        else {
            window.clear(Color::Black);
            draw_map(&window, jeu);
            draw_menu(&window);
            if (Mouse::isButtonPressed(Mouse::Left)){
                Vector2i pos = Mouse::getPosition(window);

                RectangleShape menu(Vector2f(206,46));
                menu.setPosition(199,204);
                

                RectangleShape new_game(Vector2f(416,47));
                new_game.setPosition(94,353);

                if (menu.getGlobalBounds().contains(pos.x,pos.y)) window.close();

                if (new_game.getGlobalBounds().contains(pos.x,pos.y)) {
                    
                    for(int i = 0;i<3;i++){
                        for(int j = 0;j<3;j++){
                            jeu[i][j] = 0;
                        }
                    }
                    player = false;
                    tour = 0;                  
                    
                }

            }
        }
    
        if (winner(jeu) || tour == 9) { tour = -1;}
    
        
        window.display();
    }
    return 0;
}
