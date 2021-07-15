#include <iostream>
#include "Game.h"

void test1() {
    Game::current().init();
    int pid1 = Game::current().register_player().int_number;
    int pid2 = Game::current().register_player().int_number;
    Game::current().start_cycle();
    Game::current().trade(0, {0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1, {0, 0, 0, 0}, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    Game::current().end_cycle();
    Game::current().dumpload("test1.txt");
}

void test2() {
    Game::current().init();
    Game::current().load("test1.txt");
    Game::current().dumpload("test2.txt");
}

void test3() {
    Game::current().init();
    int pid1 = Game::current().register_player().int_number;
    int pid2 = Game::current().register_player().int_number;
    Game::current().players[0].show();
    Game::current().players[1].show();
    Game::current().start_cycle();
    Game::current().end_cycle();
    Game::current().start_cycle();
    Game::current().end_cycle();
    Game::current().start_cycle();
    Game::current().end_cycle();
    Game::current().players[0].show();
    Game::current().players[1].show();
    Game::current().trade(0, {0,0,0,0},{13,0,0,0,0,0,0,0,0,0,0},1,{0,0,0,0},{1,0,0,0,0,0,0,0,0,0,0});
    std::cout<<"Traded\n";
    Game::current().players[0].show();
    Game::current().players[1].show();
    Game::current().add_treaty(0,1,0);
    Game::current().add_treaty(0,1,1);
    Game::current().players[0].show();
    Game::current().players[1].show();
    Game::current().remove_treaty(0,1,0);
    Game::current().players[0].show();
    Game::current().players[1].show();
    Game::current().remove_treaty(0,1,0);
    Game::current().remove_treaty(0,1,0);
    Game::current().add_treaty(0,1,1);
    Game::current().remove_treaty(0,1,0);
    Game::current().players[0].show();
    Game::current().players[1].show();
    for(int i=0;i<4;i++){
        Game::current().cities[i].show();
    }
    Game::current().sell_query(1,0,{10,0,0,0,0,0,0,0,0,0,0});
    Game::current().players[1].show();
    for(int i=0;i<4;i++){
        Game::current().cities[i].show();
    }
    Game::current().start_cycle();
    Game::current().end_cycle();
    Game::current().players[1].show();
    for(int i=0;i<4;i++){
        Game::current().cities[i].show();
    }
    Game::current().start_cycle();
    Game::current().end_cycle();
    Game::current().buy_query(1,0,{0,0,1,0,0,0,0,0,0,0,0});
    Game::current().players[1].show();
    for(int i=0;i<4;i++){
        Game::current().cities[i].show();
    }
    Game::current().dumpload("test3.txt");
}

int main() {
    test3();
}
