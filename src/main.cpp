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
                sf::RectangleShape horizontal(sf::Vector2f(100 * 2, 25));
                horizontal.setFillColor(sf::Color::Red);
                horizontal.setOrigin(100, 25 / 2);
                horizontal.setPosition(WINDOW / 6 + i*WINDOW/3 , WINDOW / 6 + j*WINDOW/3);

                sf::RectangleShape vertical(sf::Vector2f(25, 100 * 2));
                vertical.setFillColor(sf::Color::Red);
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

int main(){
    RenderWindow window(VideoMode(WINDOW+2,WINDOW+2), "TicTac",Style::Close);

    int jeu[3][3];
    for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
            jeu[i][j] = 0;
        }
    }
    bool player = true;
    int tour = 0;
    Event event;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (tour != 9){
            if (Mouse::isButtonPressed(sf::Mouse::Left)){
                Vector2i pos = Mouse::getPosition(window);
                Vector2i case_pos = Vector2i(pos.x / (WINDOW/3),pos.y / (WINDOW/3));
                if (!jeu[case_pos.x][case_pos.y] && player){
                    player = !player;
                    tour++;
                    jeu[case_pos.x][case_pos.y] = 1;
                }
            }
            if (Mouse::isButtonPressed(sf::Mouse::Right)){
                Vector2i pos = Mouse::getPosition(window);
                Vector2i case_pos = Vector2i(pos.x / (WINDOW/3),pos.y / (WINDOW/3));
                if (!jeu[case_pos.x][case_pos.y] && !player){
                    player = !player;
                    tour++;
                    jeu[case_pos.x][case_pos.y] = 2;
                }
            }
        }
        
        
        if (winner(jeu) || tour == 9) {cout << "Partie finie | Vainqueur :" << winner(jeu) << endl; tour = 9;}

    
        draw_map(&window, jeu);
        window.display();
    }
    return 0;
}