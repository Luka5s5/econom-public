#include "Game.h"
std::vector<int> Game::get_player_army(int id){
    if(id<0){
        return cities[-id-1].army;
    }
    else{
        return players[id].army;
    }
}

int Game::get_player_strat(int id){
    if(id<0){
        return cities[-id-1].strat;
    }
    else{
        return players[id].strat;
    }
}

void Game::kill_troop(int id, int troop_type){
    if(id<0){
        cities[-id-1].army[troop_type]--;
    }
    else{
        players[id].army[troop_type]--;
    }
}