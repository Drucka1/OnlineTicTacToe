#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

int const WINDOW = 600;
const int PORT = 11972;

enum class Player { O,X,None };

class Game{
    private : 
        int tour;
        Player map[3][3];
        Player turn;
        TcpSocket opponent;
        void return_match();
        void init(Player player);
        Player winner();
        void draw(RenderWindow* window);
        void play(int i,int j,Player player);
        bool is_ongoing();
        void menu(RenderWindow* window);
        void accept_return_match(RenderWindow* window);

    public:
        void run(RenderWindow* window,Player player);
};

struct Move { int i,j;};