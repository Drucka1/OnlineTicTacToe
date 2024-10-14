#include <iostream>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "game.h"

using namespace std;
using namespace sf;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <server|client>" << endl;
        return 1;
    }
    
    RenderWindow window(VideoMode(WINDOW + 2, WINDOW + 2), "TicTacToe", Style::Close);
    Game game;

    if (strcmp(argv[1], "server") == 0) {
        game.run(&window, Player::X);
    } else if (strcmp(argv[1], "client") == 0) {
        game.run(&window, Player::O);
    }

    return 0;
}
