#include <iostream>
#include "Game.h"

void test1() {
    Game::current().init();
    // int pid1 = Game::current().register_player().int_number;
    // int pid2 = Game::current().register_player().int_number;
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
<<<<<<< HEAD
    Game::current().register_player();
    Game::current().register_player();
    
    Game::current().start_cycle();
    std::cout << Game::current().trade(0,{0,0,0,0},{1,0,0,0,0,0,0,0,0,0,0},1,{0,0,0,0},{0,1,0,0,0,0,0,0,0,0,0}).response << std::endl;
    std::cout << Game::current().trade(0,{1,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},1,{0,1,0,0},{0,0,0,0,0,0,0,0,0,0,0}).response << std::endl; 
    Game::current().players[0].show();
    Game::current().players[1].show();
    Game::current().end_cycle();
    std::cout << "----------------------------" << std::endl; 
    
=======
    // int pid1 = Game::current().register_player().int_number;
    // int pid2 = Game::current().register_player().int_number;
    // Game::current().players[0].show();
    // Game::current().players[1].show();
    // Game::current().start_cycle();
    // Game::current().end_cycle();
    // Game::current().start_cycle();
    // Game::current().end_cycle();
    // Game::current().players[0].show();
    // Game::current().players[1].show();
    // Game::current().trade(0, {0,0,0,0},{13,0,0,0,0,0,0,0,0,0,0},1,{0,0,0,0},{1,0,0,0,0,0,0,0,0,0,0});
    // std::cout<<"Traded\n";
    // Game::current().players[0].show();
    // Game::current().players[1].show();
    // Game::current().add_treaty(0,1,0);
    // Game::current().add_treaty(0,1,1);
    // Game::current().players[0].show();
    // Game::current().players[1].show();
    // Game::current().remove_treaty(0,1,0);
    // Game::current().players[0].show();
    // Game::current().players[1].show();
    // Game::current().remove_treaty(0,1,0);
    // Game::current().remove_treaty(0,1,0);
    // Game::current().add_treaty(0,1,1);
    // Game::current().remove_treaty(0,1,0);
    // Game::current().players[0].show();
    // Game::current().players[1].show();
    // for(int i=0;i<4;i++){
    //     Game::current().cities[i].show();
    // }
    // Game::current().sell_query(1,0,{10,0,0,0,0,0,0,0,0,0,0});
    // Game::current().players[1].show();
    // for(int i=0;i<4;i++){
    //     Game::current().cities[i].show();
    // }
    // Game::current().start_cycle();
    // Game::current().end_cycle();
    // Game::current().players[1].show();
    // for(int i=0;i<4;i++){
    //     Game::current().cities[i].show();
    // }
    // Game::current().start_cycle();
    // Game::current().end_cycle();
    // Game::current().buy_query(1,0,{0,0,1,0,0,0,0,0,0,0,0});
    // Game::current().players[1].show();
    // for(int i=0;i<4;i++){
    //     Game::current().cities[i].show();
    // }
>>>>>>> suka
    Game::current().start_cycle();
    std::cout << Game::current().add_treaty(0, 1, 0).response << std::endl;
    std::cout << Game::current().add_treaty(0, 1, 0).response << std::endl;
    std::cout << Game::current().remove_treaty(0, 1, 0).response << std::endl;
    std::cout << Game::current().remove_treaty(0, 1, 0).response << std::endl;
    std::cout << Game::current().add_treaty(0, 1, 1).response << std::endl;
    Game::current().players[0].show();
<<<<<<< HEAD
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

    
        
=======
    Game::current().upgrade_army(0,{2,2,2});
    Game::current().players[0].show();

    Game::current().declare_war(0,-1);
    Game::current().wars.front().show();
    Game::current().players[0].show();
    std::cout << Game::current().proceed_top_war().response << std::endl;
    std::cout << std::endl;
    Game::current().wars.front().show();
    Game::current().players[0].show();
    std::cout << Game::current().proceed_top_war().response << std::endl;
    Game::current().wars.front().show();
    Game::current().players[0].show();
    std::cout << Game::current().proceed_top_war().response << std::endl;
    Game::current().wars.front().show();
    Game::current().players[0].show();
    /*
    Game::current().proceed_top_war();
    Game::current().players[0].show();
    Game::current().proceed_top_war();
    Game::current().players[0].show();
    Game::current().proceed_top_war();
    Game::current().players[0].show();
    Game::current().wars.front().show();
    */
    // std::cout<<"WARED\n";
    // G
    // int pid1 = Game::current().register_player().int_number;
    // int pid2 = Game::current().register_player().int_number;
    // Game::current().proceed_top_war();
    // Game::current().proceed_top_war();
    // Game::current().proceed_top_war();
}

void testwar(){
    Game::current().init();
    int pid1 = Game::current().register_player().int_number;
    int pid2 = Game::current().register_player().int_number;
    int pid3 = Game::current().register_player().int_number;
>>>>>>> suka
    Game::current().start_cycle();
    Game::current().players[0].ban = 1;
    std::cout << Game::current().sell_query(0, 0, {-1,0,0,0,0,0,0,0,0,0,0}).response << std::endl;
    Game::current().end_cycle();
<<<<<<< HEAD
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
=======
    Game::current().start_cycle();
    Game::current().declare_war(0,1);
    Game::current().end_cycle();
    Game::current().upgrade_army(0,{1,1,1});
    Game::current().add_top_war(0,2);
    Game::current().wars.front().show();
    Game::current().proceed_top_war();
    Game::current().wars.front().show();
    Game::current().proceed_top_war();
    Game::current().wars.front().show();
    for(int i=0;i<=2;i++)
        Game::current().players[i].show();
}

int main() {
    testwar();
>>>>>>> suka
}
