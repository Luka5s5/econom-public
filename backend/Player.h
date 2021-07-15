#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <string>

struct Player {
    template <typename T>
    void print(std::string name, std::vector<T> v) {
        std::cout << name << ": ";
        for (auto t: v) 
            std::cout << t << " ";
        std::cout << std::endl;
    }
    template <typename T>
    void print(std::string name, std::set<T> v) {
        std::cout << name << ": ";
        for (auto t: v) 
            std::cout << t << " ";
        std::cout << std::endl;
    }
    void print() {
        std::cout << id << std::endl;
        print("curr", currencies);
        print("res", resources);
        print("builds", building_levels);
        print("treaties0", treaties[0]);
        print("treaties1", treaties[1]);
        print("army", army);
        std::cout << "last_attack: " << last_attack << std::endl;
        std::cout << "ban: " << ban << std::endl;
        std::cout << "strategy: " << strategy << std::endl;
        std::cout << std::endl; 
    }

    Player(int id1) {
        id = id1;
        currencies = std::vector<double>(4);
        resources = std::vector<int>(11);
        building_levels = std::vector<int>(11);
        for (int i = 0; i < 7; i++) {
            building_levels[i] = 1;
        }
        treaties = std::vector<std::set<int>>(2);
        army = std::vector<int>(3);
        last_attack = 100;
        ban = 0; 
        strategy = 3;
    }
    int id;
    std::vector<double> currencies;
    std::vector<int> resources;
    std::vector<int> building_levels;
    std::vector<int> army;
    std::vector<std::set<int>> treaties; // 3
    int last_attack;
    int ban;
    int strategy;
    void mine_resources() {
        resources[0] += building_levels[0] * 4;
        resources[1] += building_levels[1] * 8;
        resources[2] += building_levels[2] * 4;
        resources[3] += building_levels[3] * 8;
        resources[4] += building_levels[4] * 6;
        resources[5] += building_levels[5] * 4;
        resources[6] += building_levels[6] * building_levels[6];
        for (int i = 0; i <= 2; i++) {
            resources[i] += building_levels[7] * (building_levels[i] - 1);
        }
        for (int i = 3; i <= 5; i++) {
            resources[i] += building_levels[8] * (building_levels[i] - 1);
        }
        for (int i = 0; i < 7; i++) {
            resources[i] += building_levels[9] * 2;
        }
        resources[6] += building_levels[8] * (building_levels[6] + 1); 
    }
    bool check_available_currencies(std::vector<double> required) {
        return less_or_equal(required, currencies);
    } 
    bool check_available_resources(std::vector<int> required) {
        return less_or_equal(required, resources);
    }
    template<typename T>
    bool less_or_equal(std::vector<T> a, std::vector<T> b) {
        if (a.size() != b.size()) {
           return false; 
        }
        for (int i = 0; i < a.size(); i++) {
            if (a[i] > b[i])
                return false; 
        }
        return true;
    }
    void change_currencies(std::vector<double> delta) {
        change_vector(currencies, delta);
    } 
    void change_resources(std::vector<int> delta) {
        change_vector(resources, delta);
    }
    template<typename T> 
    void change_vector(std::vector<T>& a, std::vector<T> b) {
        for (int i = 0; i < a.size(); i++) {
            a[i] += b[i]; 
        }
    }
    bool has_treaty(int player_id, int type_of_treaty) {
        return treaties[type_of_treaty].find(player_id) != treaties[type_of_treaty].end(); 
    }
    void add_treaty(int player_id, int type_of_treaty) {
        treaties[type_of_treaty].insert(player_id);
    }
    void remove_treaty(int player_id, int type_of_treaty) {
        treaties[type_of_treaty].erase(player_id);
    }
};
