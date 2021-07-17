#pragma once

#include "Player.h"
#include "City.h"
#include "War.h"
#include "Response.h"
#include <string>
#include <vector>
#include <fstream>
#include <deque>

struct Game {
public:
    // Tech
    void init(); // Use this method with first call of current().
    static Game& current();
    Response register_player();
    Response start_cycle();
    Response end_cycle();

    Response dumpload(std::string filename);
    Response load(std::string filename);

    // Cities - Markets
    Response sell_query(int player_id, int city_id, std::vector<int> resource_array);

    // War
    Response set_strategy(int player_id, int new_strategy);
    Response declare_war(int id_att, int id_def);
    Response add_top_war(int is_attack, int id);
    Response proceed_top_war();
    Response concede_top_war(int attack_won);
    Response stop_top_war();

    // Relationships
    Response trade(int sender_id, int reciever_id, std::vector<double> currencies, std::vector<int> resources);
    Response add_treaty(int player1_id, int player2_id, int type_of_treaty);
    Response remove_treaty(int player1_id, int player2_id, int type_of_treaty);

    // Upgrades
    Response upgrade_army(int player_id, std::vector<int> army);
    Response upgrade_building(int player_id, std::vector<int> buildings);

    // Not for frontend
    std::vector<int> get_player_army(int id);
    int get_player_strat(int id);
    void kill_troop(int id, int troop_type);
    Response get_victory_points(int player_id);

    std::vector<Player> players;
    std::vector<City> cities;
    std::deque<War> wars;
    int current_round = 1;

private:
    Game() = default; 
    static Game* game;
};