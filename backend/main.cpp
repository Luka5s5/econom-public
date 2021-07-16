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

void test4() {
    Game::current().init();
    Game::current().register_player();
    Game::current().register_player();
    
    Game::current().start_cycle();
    std::cout << Game::current().trade(0,{0,0,0,0},{1,0,0,0,0,0,0,0,0,0,0},1,{0,0,0,0},{0,1,0,0,0,0,0,0,0,0,0}).response << std::endl;
    std::cout << Game::current().trade(0,{1,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},1,{0,1,0,0},{0,0,0,0,0,0,0,0,0,0,0}).response << std::endl; 
    Game::current().players[0].show();
    Game::current().players[1].show();
    Game::current().end_cycle();
    std::cout << "----------------------------" << std::endl; 
    
    Game::current().start_cycle();
    std::cout << Game::current().add_treaty(0, 1, 0).response << std::endl;
    std::cout << Game::current().add_treaty(0, 1, 0).response << std::endl;
    std::cout << Game::current().remove_treaty(0, 1, 0).response << std::endl;
    std::cout << Game::current().remove_treaty(0, 1, 0).response << std::endl;
    std::cout << Game::current().add_treaty(0, 1, 1).response << std::endl;
    Game::current().players[0].show();
    Game::current().players[1].show();
    Game::current().end_cycle();
    std::cout << "----------------------------" << std::endl;

    
    Game::current().start_cycle();
    Game::current().players[0].show();
    Game::current().cities[0].show();
    std::cout << Game::current().sell_query(0, 0, {10,0,0,0,0,0,0,0,0,0,0}).response << std::endl;
    std::cout << Game::current().sell_query(0, 0, {1000000,0,0,0,0,0,0,0,0,0,0}).response << std::endl; 
    Game::current().players[0].show();
    Game::current().cities[0].show();
    Game::current().end_cycle();
    std::cout << "----------------------------" << std::endl;


    Game::current().start_cycle();
    Game::current().players[0].show();
    Game::current().cities[0].show();
    std::cout << Game::current().sell_query(0, 0, {-1,0,0,0,0,0,0,0,0,0,0}).response << std::endl;
    std::cout << Game::current().sell_query(0, 0, {-1,0,0,0,0,0,0,0,0,0,0}).response << std::endl;
    Game::current().players[0].show();
    Game::current().cities[0].show();
    Game::current().end_cycle();
    std::cout << "----------------------------" << std::endl;

    
        
    Game::current().start_cycle();
    Game::current().players[0].ban = 1;
    std::cout << Game::current().sell_query(0, 0, {-1,0,0,0,0,0,0,0,0,0,0}).response << std::endl;
    Game::current().end_cycle();
    std::cout << "----------------------------" << std::endl;

    
    Game::current().start_cycle();
    std::cout << Game::current().sell_query(0, 0, {-1,0,0,0,0,0,0,0,0,0,0}).response << std::endl;
    Game::current().end_cycle();
    std::cout << "----------------------------" << std::endl;

    Game::current().start_cycle();
    std::cout << Game::current().upgrade_building(0, {1,0,0,0,0,0,0,0,0,0,0}).response << std::endl;
    Game::current().players[0].show();
    Game::current().end_cycle();
    std::cout << "----------------------------" << std::endl;

    
}

int main() {
    test4();
}
