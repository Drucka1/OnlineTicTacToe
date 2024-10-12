
#include <SFML/Graphics.hpp>
using namespace sf;

int const WINDOW = 600;

enum class Player { O,X,None };

class Game{
    private : 
        int tour;
        Player map[3][3];
        Player turn;

    public:
        void init();
        Player winner();
        void draw(RenderWindow* window);
        void move(int i,int j,Player turn);
        bool is_ongoing();
};