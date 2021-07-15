#include <Player.h>
#include <City.h>
#include <War.h>

struct Game {
public:
    Game& current() {
        if (game == nullptr) {
            game = new Game();
        }
        return *game;
    }
    std::vector<Player> players;
    std::vector<City> cities;
    std::vector<War> wars;
private:
    Game() = default;
    static Game* game; 
};
