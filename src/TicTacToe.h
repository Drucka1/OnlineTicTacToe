#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

int const WINDOW = 600;
const int PORT = 11922;

enum class Player { O,X,None };
enum class GameWindow { Menu,Credit,Play,Rematch,End };

enum class MessageType { Move,Quit,AskRematch,AnswerRematch };

struct Message {
    MessageType type;
    union {
        struct Move {int i, j;} move;
        bool answerRematch;
    } data;
};

class TicTacToe{
    private : 
        int tour;
        Player map[3][3];
        Player player;
        Player turn;
        TcpSocket opponent;
        GameWindow window;
        Font font;

        void init();
        void init_match();
        void init_socket(Player player);
        Player winner();
        void draw(RenderWindow* window);
        void play(int i,int j,Player player);
        bool is_ongoing();
        void end(RenderWindow* window);
        void accept_rematch(RenderWindow* window);
        void menu(RenderWindow* window);
        void read();
        void send(Message msg);

    public:
        void run(RenderWindow* window);
};