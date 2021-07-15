#include <iostream>
#include "Game.h"
int main() {
    Game::current().init();
    int pid1 = Game::current().register_player().int_number;
    int pid2 = Game::current().register_player().int_number;
    Game::current().players[0].print();
    Game::current().players[1].print();
    Game::current().start_cycle();
    // Game::current().trade(0, {0})
    Game::current().end_cycle();
}
