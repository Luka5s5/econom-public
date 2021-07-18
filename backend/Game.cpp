#include "Game.h"

template <typename T>
std::vector<T> changed_sign(std::vector<T> v) {
    std::vector<T> v1 = v;
    for (auto& t: v1) {
        t = -t;
    }
    return v1;
}

Game* Game::game = nullptr;

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
        return cities[-id-1].strategy;
    }
    else{
        return players[id].strategy;
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


Response Game::upgrade_building(int player_id, std::vector<int> buildings) {
    Player& player = players[player_id];
    bool accepted = true;
    for (int i = 0; i < buildings.size(); i++) {
        if (buildings[i] && player.building_used[i]) {
            return Response{false, "Такое здание уже улучшалось сегодня"};
        }
    }
    std::vector<int> resources(11);
    resources[1] += 8 * player.building_levels[0] * buildings[0];
    resources[3] += 6 * player.building_levels[0] * buildings[0];

    resources[1] += 8 * player.building_levels[1] * buildings[1];
    resources[3] += 6 * player.building_levels[1] * buildings[1];

    resources[1] += 8 * player.building_levels[2] * buildings[2];
    resources[3] += 6 * player.building_levels[2] * buildings[2];

    resources[1] += 8 * player.building_levels[3] * buildings[3];
    resources[2] += 8 * player.building_levels[3] * buildings[3];

    resources[1] += 4 * player.building_levels[4] * buildings[4];
    resources[4] += 2 * player.building_levels[4] * buildings[4];
    resources[2] += 1 * player.building_levels[4] * buildings[4];

    resources[4] += 8 * player.building_levels[5] * buildings[5];
    resources[3] += 10 * player.building_levels[5] * buildings[5];
    resources[2] += 1 * player.building_levels[5] * buildings[5];

    resources[2] += 8 * player.building_levels[6] * buildings[6];
    resources[0] += 8 * player.building_levels[6] * buildings[6];
    resources[3] += 20 * player.building_levels[6] * buildings[6];

    resources[2] += (10 * player.building_levels[7] + 10) * buildings[7];
    resources[0] += (10 * player.building_levels[7] + 10) * buildings[7];
    resources[3] += (10 * player.building_levels[7] + 10) * buildings[7];
    resources[9] += (player.building_levels[7] + 1) * buildings[7];


    resources[2] += (10 * player.building_levels[8] + 10) * buildings[8];
    resources[0] += (10 * player.building_levels[8] + 10) * buildings[8];
    resources[1] += (10 * player.building_levels[8] + 10) * buildings[8];
    resources[10] += (player.building_levels[8] + 1) * buildings[8];


    resources[0] += (20 * player.building_levels[9] + 20) * buildings[9];
    resources[6] += (10 * player.building_levels[9] + 10) * buildings[9];
    resources[8] += (player.building_levels[9] + 1) * buildings[9];


    resources[3] += (20 * player.building_levels[10] + 20) * buildings[10];
    resources[0] += (10 * player.building_levels[10] + 10) * buildings[10];
    resources[7] += (player.building_levels[10] + 1) * buildings[10];


    accepted = accepted && player.check_available_resources(resources);
    if (!accepted) {
        return Response{false, "Недостаточно ресурсов"};
    }
    for (int i = 0; i < buildings.size(); i++) {
        player.building_used[i] |= buildings[i];
    }
    player.change_resources(changed_sign(resources));
    player.change_buildings(buildings);
    return Response{true, "Успешно."};
}

Response Game::upgrade_army(int player_id, std::vector<int> army) {
    Player& player = players[player_id];
    bool accepted = true;
    std::vector<int> resources(11);
    resources[1] += army[0];
    resources[4] += army[0];
    resources[5] += army[0];

    resources[1] += army[1];
    resources[5] += army[1];
    resources[6] += army[1];

    resources[1] += army[2];
    resources[3] += army[2];
    resources[5] += army[2];
    accepted = accepted && player.check_available_resources(resources);
    if (!accepted) {
        return Response{false, "Недостаточно ресурсов"};
    }
    player.change_resources(changed_sign(resources));
    player.change_army(army);
    return Response{true, "Армия пополнена"};
}

Response Game::set_strategy(int player_id, int new_strategy) {
    players[player_id].strategy = new_strategy;
    return Response{true, "Стратегия изменена"};
}

Response Game::get_victory_points(int player_id) {
    Player& player = players[player_id];
    int answer = 0;
    for (int i = 0; i < 7; i++) {
        answer += player.building_levels[i];
    }
    for (int i = 7; i < 9; i++) {
        answer += player.building_levels[i] * 2;
    }
    for (int i = 9; i < 11; i++) {
        answer += player.building_levels[i] * 3;
    }
    return Response{true, "Посчитано.", answer};
}

Response Game::start_cycle() {
    if (is_cycle) {
        return Response{false, "Цикл уже идёт."};
    }
    is_cycle = true;
    dumpload(std::to_string(current_round++) + ".txt");
    for (auto& city: cities) {
        city.update_plots();
    }
    for (auto& player: players) {
        player.mine_resources();
    }
    return Response{true, "Цикл начат."};
}

Response Game::end_cycle() {
    if (!is_cycle) {
        return Response{false, "Цикл собственно и не идёт"};
    }
    is_cycle = false;
    for (auto& player: players) {
        player.last_attack++;
        player.ban = 0;
        player.city_used.assign(4, 0);
        player.building_used.assign(11, 0);
    }
    return Response{true, "Цикл окончен."};
}

Response Game::register_player() {
    players.push_back(Player(players.size()));
    return Response{true, std::to_string(players.size() - 1) + " - Номер нового игрока", (int)players.size()-1};
}

Response Game::trade(int sender_id,
               int reciever_id,
               std::vector<double> currencies,
               std::vector<int> resources) {
    std::vector<double> currencies1 = currencies, currencies2 = currencies;
    std::vector<int> resources1 = resources, resources2 = resources;
    for (int i = 0; i < currencies.size(); i++) {
        if (currencies1[i] < 0)
            currencies1[i] = 0;
    }
    for (int i = 0; i < currencies.size(); i++) {
        if (resources1[i] < 0)
            resources1[i] = 0;
    }
    for (int i = 0; i < currencies.size(); i++) {
        if (currencies2[i] > 0)
            currencies2[i] = 0;
        else
            currencies2[i] *= (-1);
    }
    for (int i = 0; i < currencies.size(); i++) {
        if (resources2[i] > 0)
            resources2[i] = 0;
        else
            resources2[i] *= (-1);
    }
    Player& p1 = players[sender_id];
    Player& p2 = players[reciever_id];
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

Response Game::add_treaty(int player1_id, int player2_id, int type_of_treaty) {
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

Response Game::remove_treaty(int player1_id, int player2_id, int type_of_treaty) {
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

Response Game::sell_query(int player_id, int city_id, std::vector<int> resource_array) {
    Player& player = players[player_id];
    City& city = cities[city_id];
    if (!city.check_available_for_buy_resources(resource_array)) {
        return Response{false, "Нет спроса."};
    }
    if (!city.accept(player, resource_array)) {
        return Response{false, "Город не разрешил продажу перком"};
    }
    if (player.ban) {
        return Response{false, "На вас лежит бан за невыполнение договора"};
    }
    if (player.city_used[city_id]) {
        return Response{false, "Вы уже торговали сегодня в этом городе"};
    }
    std::vector<double> currency_array(4);
    for (int i = 0; i < resource_array.size(); i++) {
        currency_array[city_id] += city.get_price(i) * resource_array[i];
    }
    if (!player.check_available_resources(resource_array)) {
        return Response{false, "Недостаточно ресурсов"};
    }
    if (!player.check_available_currencies(changed_sign(currency_array))) {
        return Response{false, "Недостаточно валюты"};
    }
    player.city_used[city_id] = 1;
    city.pre_action(player, resource_array);
    player.change_currencies(currency_array);
    player.change_resources(changed_sign(resource_array));
    city.change_available_for_buy_resources(changed_sign(resource_array));
    city.post_action(player, resource_array);
    return Response{true, "Успешно."};
}

void Game::init() {
    auto bool_pass = [](Player& p, std::vector<int>& resources) {return true;};
    auto pass = [](Player& p, std::vector<int>& resources){return;};
    auto varant = [](Player& p, std::vector<int>& resources){ // после сделки нужно уменьшить золото на 10%
        p.resources[0] *= 0.9;
    };
    auto liberty = [](Player& p, std::vector<int>& resources){ // Перед покупкой нужно проверить на вино
        if (resources[6] != 0 && rand() % 5 == 0) {
            for (int i = 0; i < 11; i++) {
                if (resources[i] < 0) {
                    resources[i] = 0;
                }
            }
            p.change_resources(changed_sign(resources));
            resources.assign(11, 0);
        }
    };
    auto bool_pass_mordor = [](Player& p, std::vector<int>& resources) { // Проверка на последнюю войну
        return (p.last_attack <= 3);
    };
    auto bool_pass_talas = [](Player& p, std::vector<int>& resources) { // Проверка на последнюю войну
        return (p.last_attack > 1);
    };
    cities.push_back(City(0, {{1.4132, 0, -0.4548, 1000}, //Варант
                              {0.4843, 0, -0.4936, 1000},
                              {2.0313, 0, -2.0316, 1000},
                              {0.2164, 0, -0.2052, 1000},
                              {0.5062, 0, -0.5305, 1000},
                              {1.5636, 0, -0.4561, 1000},
                              {2.8551, 0, -1.0426, 1000},
                              {0.0009, 0, -0.0010, 100},
                              {0.0000000001, 0, -1, 1000000},
                              {0.0000000001, 0, -1, 1000000},
                              {0.0000000001, 0, -1, 1000000}
    }, bool_pass, pass, varant));
    cities.push_back(City(1, {{0.5947, 0, -0.1812, 1000}, // Либерти
                              {0.0344, 0, -0.3021, 1000},
                              {0.0685, 0, -0.2159, 1000},
                              {0.4264, 0, -0.0571, 1000},
                              {0.1650, 0, -0.0560, 1000},
                              {0.1454, 0, -0.0548, 1000},
                              {0.0091, 0, -0.0096, 500},
                              {0.0001, 0, -1, 1000000},
                              {0.0001, 0, -0.0001, 100},
                              {0.0001, 0, -1, 1000000},
                              {0.0001, 0, -1, 1000000}
    }, bool_pass, liberty, pass));
    cities.push_back(City(2, {{0.0954, 0, -0.1019, 1000}, // Мордор
                              {0.1033, 0, -0.0131, 1000},
                              {0.1750, 0, -0.0258, 1000},
                              {0.0097, 0, -0.0097, 1000},
                              {0.0256, 0, -0.0244, 1000},
                              {0.0238, 0, -0.0226, 1000},
                              {0.0048, 0, -0.0456, 1000},
                              {0.0001, 0, -1, 1000000},
                              {0.0001, 0, -1, 1000000},
                              {0.0001, 0, -0.0001, 100},
                              {0.0001, 0, -1, 1000000}
    }, bool_pass_mordor, pass, pass));
    cities.push_back(City(3, {{0.0964, 0, -0.1067, 1000}, // Кель-Талас
                              {0.0914, 0, -0.1035, 1000},
                              {0.1615, 0, -0.0525, 1000},
                              {0.2713, 0, -0.0950, 1000},
                              {0.1604, 0, -0.0473, 1000},
                              {0.1498, 0, -0.0463, 1000},
                              {0.1163, 0, -0.0959, 1000},
                              {0.0001, 0, -1, 1000000},
                              {0.0001, 0, -1, 1000000},
                              {0.0001, 0, -1, 1000000},
                              {0.0001, 0, -0.0001, 100}
    }, bool_pass_talas, pass, pass));
}

Response Game::dumpload(std::string filename) {
    std::ofstream file;
    file.open(filename);
    file << current_round << std::endl;
    file << players.size() << std::endl;
    for (auto& player: players)
        player.dumpload(file);
    for (auto& city: cities)
        city.dumpload(file);
    file.close();
    return Response{true, "Записано в " + filename};
}

Response Game::load(std::string filename) {
    players.clear();

    std::ifstream file;
    file.open(filename);
    file >> current_round;
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
    return Response{true, "Считано из " + filename};
}

Game& Game::current() {
    if (game == nullptr) {
        game = new Game();
    }
    return *game;
}

Response Game::declare_war(int id_att, int id_def) {
    bool flag=1;
    for(auto& war:wars) flag=(flag and war.attacker_id!=id_att or war.defender_id!=id_def);
    if(!flag)
        return Response{0,"Такая война уже объявлена"};
    wars.push_back(War(id_att,id_def));
    return Response{1,"Всё чикипуки"};
}

Response Game::add_top_war(int is_attack, int id) {
    bool flag=1;
    for(auto i:wars.front().a_side_ids){
        flag=(flag && i!=id);
    }
    for(auto i:wars.front().d_side_ids){
        flag=(flag && i!=id);
    }
    if(!flag) return Response{0,"уже зареган в войну"};
    if(is_attack==1){
        wars.front().add_attacker(id);
    }
    else{
        wars.front().add_defender(id);
    }
    return Response{1,"Все ок"};
}

Response Game::proceed_top_war() {
    if(wars.size()==0) return Response{0,"Нет воин"};
    if(wars.front().step==4){
        wars.pop_front();
        return Response{1,"Война закончена все посчитатно"};
    }
    wars.front().progress_war();
    return {1,"Война продвинулась(вроде)"};
}

Response Game::concede_top_war(int attack_won) {
    if(wars.size()==0) return Response{0,"Нет воин"};
    wars.front().step=4;
    wars.front().someone_won(attack_won);
    wars.pop_front();
    return Response{1,"Война закончена все посчитатно"};
}
Response Game::stop_top_war() {
    if(wars.size()==0) return Response{0,"Нет воин"};
    wars.pop_front();
    return Response{1,"Война закончилась миром"};
}
