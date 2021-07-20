#include "City.h"

City::City(int id1, std::vector<std::vector<double>> plots, std::function<bool(Player &, std::vector<int> &)> accept1,
           std::function<void(Player &, std::vector<int> &)> pre_action1,
           std::function<void(Player &, std::vector<int> &)> post_action1) {
    id = id1;
    for (int i = 0; i < plots.size(); i++) {
        items.push_back(Plot{plots[i][0], (int)plots[i][1], plots[i][2], (int)plots[i][3]});
    }
    delta_per_round = std::vector<int>(11);
    accept = accept1;
    pre_action = pre_action1;
    post_action = post_action1;
    army = {1000, 1000, 1000};
    strategy = 3;
}

void City::show() {
    std::cout<<"id: "<<id<<std::endl;
    for(auto i:items)
        std::cout << i.countP() << " ";
    std::cout<<std::endl;
}

bool City::check_available_for_buy_resources(std::vector<int> res) {
    if (res.size() != delta_per_round.size())
        return false;
    for (int i = 0; i < res.size(); i++) {
        if (res[i] > get_limit_Q(i)) {
            return false;
        }
    }
    return true;
}

void City::change_available_for_buy_resources(std::vector<int> delta) {
    for (int i = 0; i < delta.size(); i++) {
        delta_per_round[i] -= delta[i];
    }
}

int City::get_limit_Q(int res_id) {
    return items[res_id].D_b - items[res_id].y_delta - delta_per_round[res_id];
}

double City::get_price(int res_id) {
    return items[res_id].countP();
}

void City::update_plots() {
    for (int i = 0; i < items.size(); i++) {
        items[i].y_delta += delta_per_round[i];
        items[i].y_delta /= 2;
        delta_per_round[i] = 0;
    }
}

template<typename T>
void City::print(std::ofstream &file, std::vector<T> v) {
    file << v.size() << " ";
    for (auto t: v)
        file << t << " ";
    file << std::endl;
}

void City::dumpload(std::ofstream &file) {
    file << id << std::endl;
    file << items.size() << " ";
    for (int i = 0; i < items.size(); i++) {
        file << items[i].y_delta << " ";
    }
    file << std::endl;
    print(file, delta_per_round);
    print(file, army);
    file << strategy << std::endl;
    file << std::endl;
}

template<typename T>
void City::read(std::ifstream &file, std::vector<T> &v) {
    int size = 0;
    file >> size;
    v.resize(size);
    for (auto& t: v) {
        file >> t;
    }
}

void City::load(std::ifstream &file) {
    file >> id;
    int size = 0;
    file >> size;
    for (int i = 0; i < items.size(); i++) {
        file >> items[i].y_delta;
    }
    read(file, delta_per_round);
    read(file, army);
    file >> strategy;
}
