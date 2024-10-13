
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
        void play(int i,int j,Player player);
        bool is_ongoing();
        void run(RenderWindow* window,int socket,Player player);
        void menu(RenderWindow* window);
};

struct Move { int i,j;};