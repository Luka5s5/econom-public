#pragma once

#include "Player.h"
#include "City.h"
#include "War.h"
#include "Response.h"
#include <string>
#include <vector>
#include <fstream>
#include <deque>

template <typename T>
std::vector<T> changed_sign(std::vector<T> v) {
    std::vector<T> v1 = v;
    for (auto& t: v1) {
        t = -t;
    }
    return v1;
}

struct Game {
public:
    std::vector<int> get_player_army(int id);
    int get_player_strat(int id);
    void kill_troop(int id, int troop_type);

    Response start_cycle() {
        for (auto& city: cities) {
            city.update_plots();
        }
        for (auto& player: players) {
            player.mine_resources();
        }
        return Response{true, "Цикл начат."};
    }
    
    Response end_cycle() {
        for (auto& player: players) {
            player.last_attack++;
            player.ban = 0;
        }
        return Response{true, "Цикл окончен."};
    }
 
    Response register_player() {
        players.push_back(Player(players.size()));
        return Response{true, std::to_string(players.size() - 1) + " - Номер нового игрока", (int)players.size()}; 
    }

    Response trade(int player1_id,
                    std::vector<double> currencies1,
                    std::vector<int> resources1,
                    int player2_id,
                    std::vector<double> currencies2,
                    std::vector<int> resources2) {
        Player& p1 = players[player1_id];
        Player& p2 = players[player2_id];
        bool accepted = p1.check_available_currencies(currencies1) &&
                        p1.check_available_resources(resources1) &&
                        p2.check_available_currencies(currencies2) &&
                        p2.check_available_resources(resources2);
        if (!accepted) {
            return Response{false, "Недостаточно средств на одном из балансов."};
        }
        
        p1.change_currencies(changed_sign(currencies1));
        p1.change_resources(changed_sign(resources1));
        p2.change_currencies(changed_sign(currencies2));
        p2.change_resources(changed_sign(resources2));
        p1.change_currencies(currencies2);
        p1.change_resources(resources2);
        p2.change_currencies(currencies1);
        p2.change_resources(resources1);
         
        return Response{true, "Успешно."};
    }

    Response add_treaty(int player1_id, int player2_id, int type_of_treaty) {
        Player& p1 = players[player1_id];
        Player& p2 = players[player2_id];
        bool accepted = (!p1.has_treaty(player2_id, type_of_treaty)) &&
                        (!p2.has_treaty(player1_id, type_of_treaty));
        if (!accepted) {
            return Response{true, "Такое соглашение уже существует."};
        }    
        p1.add_treaty(player2_id, type_of_treaty);
        p2.add_treaty(player1_id, type_of_treaty);
        return Response{true, "Успешно."};
    }   
    
    Response remove_treaty(int player1_id, int player2_id, int type_of_treaty) {
        Player& p1 = players[player1_id];
        Player& p2 = players[player2_id];
        bool accepted = (p1.has_treaty(player2_id, type_of_treaty)) &&
                        (p2.has_treaty(player1_id, type_of_treaty));
        if (!accepted) {
            return Response{true, "Такого соглашения не существует."};
        }    
        p1.remove_treaty(player2_id, type_of_treaty);
        p2.remove_treaty(player1_id, type_of_treaty);
        return Response{true, "Успешно."};
    } 

    Response buy_query(int player_id, int city_id, std::vector<int> resource_array) {
        Player& player = players[player_id];
        City& city = cities[city_id];
        std::vector<double> currency_array(4); 
        for (int i = 0; i < resource_array.size(); i++) {
            currency_array[city_id] += city.get_price(i) * resource_array[i];   
        }      
        bool accepted = player.check_available_currencies(currency_array) && city.accept(player, resource_array);
        if (!accepted) {
            return Response{false, "Недостаточно ресурсов игрока или есть проблема с перком."};
        }
        city.pre_buy(player, resource_array);
        player.change_resources(resource_array); 
        player.change_currencies(changed_sign(currency_array));
        city.change_available_for_buy_resources(resource_array);
        city.post_buy(player, resource_array);
        return Response{true, "Успешно."};
    }

    Response sell_query(int player_id, int city_id, std::vector<int> resource_array) {
        Player& player = players[player_id];
        City& city = cities[city_id]; 
        bool accepted = city.check_available_for_buy_resources(resource_array) &&
                        city.accept(player, resource_array) && player.check_available_resources(resource_array);
        if (!accepted) {
            return Response{false, "Нет спроса или недостаточно ресурсов игрока или есть проблема с перком."};
        } 
        std::vector<double> currency_array(4);
        for (int i = 0; i < resource_array.size(); i++) {
            currency_array[city_id] += city.get_price(i) * resource_array[i];   
        }
        city.pre_sell(player, resource_array);
        player.change_currencies(currency_array); 
        player.change_resources(changed_sign(resource_array));
        city.change_available_for_buy_resources(changed_sign(resource_array));
        city.post_sell(player, resource_array);
        return Response{true, "Успешно."};
    }

    void init() {
        auto bool_pass = [](Player& p, std::vector<int> resources) {return true;};
        auto pass = [](Player& p, std::vector<int> resources){return;};
        cities.push_back(City(0, {{1.4132, 0, -0.4548, 1000}, //Варант
                                {0.4843, 0, -0.4936, 1000},
                                {2.0313, 0, -2.0316, 1000},
                                {0.2164, 0, -0.2052, 1000},
                                {0.5062, 0, -0.5305, 1000},
                                {1.5636, 0, -0.4561, 1000},
                                {2.8551, 0, -1.0426, 1000},
                                {0.0009, 0, -0.0010, 100},
                                {100, 0, -1, 1000000},
                                {100, 0, -1, 1000000},
                                {100, 0, -1, 1000000}
        }, bool_pass, pass, pass, pass, pass)); // после сделки нужно уменьшить золото на 10%
        cities.push_back(City(1, {{0.5947, 0, -0.1812, 1000}, // Либерти
                                {0.0344, 0, -0.3021, 1000},
                                {0.0685, 0, -0.2159, 1000},
                                {0.4264, 0, -0.0571, 1000},
                                {0.1650, 0, -0.0560, 1000},
                                {0.1454, 0, -0.0548, 1000},
                                {0.0091, 0, -0.0096, 500},
                                {100, 0, -1, 1000000},
                                {0.0001, 0, -0.0001, 100},
                                {100, 0, -1, 1000000},
                                {100, 0, -1, 1000000}
        }, bool_pass, pass, pass, pass, pass)); // Перед покупкой нужно проверить на вино
        cities.push_back(City(2, {{0.0954, 0, -0.1019, 1000}, // Мордор
                                {0.1033, 0, -0.0131, 1000},
                                {0.1750, 0, -0.0258, 1000},
                                {0.0097, 0, -0.0097, 1000},
                                {0.0256, 0, -0.0244, 1000},
                                {0.0238, 0, -0.0226, 1000},
                                {0.0048, 0, -0.0456, 1000},
                                {100, 0, -1, 1000000},
                                {100, 0, -1, 1000000},
                                {0.0001, 0, -0.0001, 100},
                                {100, 0, -1, 1000000}
        }, bool_pass, pass, pass, pass, pass)); // Нетривиальная проверка
        cities.push_back(City(3, {{0.0964, 0, -0.1067, 1000}, // Кель-Талас
                                {0.0914, 0, -0.1035, 1000},
                                {0.1615, 0, -0.0525, 1000},
                                {0.2713, 0, -0.0950, 1000},
                                {0.1604, 0, -0.0473, 1000},
                                {0.1498, 0, -0.0463, 1000},
                                {0.1163, 0, -0.0959, 1000},
                                {100, 0, -1, 1000000},
                                {100, 0, -1, 1000000},
                                {100, 0, -1, 1000000},
                                {0.0001, 0, -0.0001, 100}
        }, bool_pass, pass, pass, pass, pass)); // Нетривиальная проверка
    }

    void dumpload(std::string filename) {
        std::ofstream file;
        file.open(filename);
        file << players.size() << std::endl;
        for (auto& player: players) 
            player.dumpload(file);
        for (auto& city: cities)
            city.dumpload(file);
        file.close();
    }
    
    void load(std::string filename) {
        std::ifstream file;
        file.open(filename);
        int player_count = 0;
        file >> player_count;
        for (int i = 0; i < player_count; i++) {
            register_player();
        }
        for (auto& player: players) { 
            player.load(file);
        }
        for (auto& city: cities)
            city.load(file);
        file.close();
    }

    static Game& current() {
        if (game == nullptr) {
            game = new Game();
        }
        return *game;
    }
    
    std::vector<Player> players;
    std::vector<City> cities;
    std::deque<War> wars;
    int current_round;
    
    Response declare_war(int id_att, int id_def){
        bool flag=1;
        for(auto& war:wars) flag=(flag and war.attacker_id!=id_att or war.defender_id!=id_def);
        if(!flag)
            return Response{0,"Такая война уже объявлена"};
        wars.push_back(War(id_att,id_def));
        return Response{1,"Всё чикипуки"};
    }

    Response add_top_war(int is_attack, int id){
        bool flag=1;
        for(auto i:wars.front().a_side_ids){
            flag=(flag && i!=id);
        }
        for(auto i:wars.front().d_side_ids){
            flag=(flag && i!=id);
        }
        if(!flag) return Response{0,"уже зареган в войну"};
        if(is_attack==0){
            wars.front().add_attacker(id);
        }
        else{
            wars.front().add_defender(id);
        }
    }

    Response proceed_top_war(){
        if(wars.size()==0) return Response{0,"Нет воин"};
        if(wars.front().step==4){
            wars.pop_front();
            return Response{1,"Война закончена все посчитатно"};
        }
        wars.front().progress_war();
        return {1,"Война продвинулась(вроде)"};
    }

    Response concede_top_war(int attack_won){
        if(wars.size()==0) return Response{0,"Нет воин"};
        wars.front().step=4;
        wars.front().someone_won(attack_won);
        wars.pop_front();
        return Response{1,"Война закончена все посчитатно"};
    }
    Response stop_top_war(){
        if(wars.size()==0) return Response{0,"Нет воин"};
        wars.pop_front();
        return Response{1,"Война закончилась миром"};
    }
private:
    Game() = default; 
    static Game* game;
};

