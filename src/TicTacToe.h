#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

int const WINDOW = 600;
const int PORT = 11542;

enum class Player { O,X,None };
enum class GameWindow { Menu,Credit,Play,Rematch,End };

class TicTacToe{
    private : 
        int tour;
        Player map[3][3];
        Player player;
        Player turn;
        TcpSocket opponent;
        GameWindow window;
        Font font;

        void init_rematch();
        void init();
        void init_socket(Player player);
        Player winner();
        void draw(RenderWindow* window);
        void play(int i,int j,Player player);
        bool is_ongoing();
        void end(RenderWindow* window);
        void accept_rematch(RenderWindow* window);
        void menu(RenderWindow* window);

    public:
        void run(RenderWindow* window);
};

struct Move { int i,j;};