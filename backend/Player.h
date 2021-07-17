#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <fstream>

struct Player {
    Player(int id1);
    void mine_resources();
    void change_buildings(std::vector<int> delta);
    void change_army(std::vector<int> delta);
    void change_currencies(std::vector<double> delta);
    void change_resources(std::vector<int> delta);
    bool check_available_currencies(std::vector<double> required);
    bool check_available_resources(std::vector<int> required);
    bool has_treaty(int player_id, int type_of_treaty);
    void add_treaty(int player_id, int type_of_treaty);
    void remove_treaty(int player_id, int type_of_treaty);

    // Tech
    void show(); // to console
    void dumpload(std::ofstream& file);
    void load(std::ifstream& file);

    // Not for frontend
    template <typename T>
    void show(std::string name, std::vector<T> v);
    template <typename T>
    void show(std::string name, std::set<T> v);
    template <typename T>
    void print(std::ofstream& file, std::vector<T> v);
    template <typename T>
    void print(std::ofstream& file, std::set<T> v);
    template <typename T>
    void read(std::ifstream& file, std::vector<T>& v);
    template <typename T>
    void read(std::ifstream& file, std::set<T>& v);
    template<typename T>
    bool less_or_equal(std::vector<T> a, std::vector<T> b);
    template<typename T>
    void change_vector(std::vector<T>& a, std::vector<T> b);

    int id;
    std::vector<double> currencies;
    std::vector<int> resources;
    std::vector<int> building_levels;
    std::vector<int> army;
    std::vector<std::set<int>> treaties;
    int last_attack;
    int ban;
    int strategy;
    std::vector<int> city_used;
    std::vector<int> building_used;
    std::string name;
};
