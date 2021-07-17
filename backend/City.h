#pragma once

#include <functional>
#include "Player.h"
#include <vector>
#include <fstream>

struct Plot;

struct City {
    City(int id1, std::vector<std::vector<double>> plots, std::function<bool(Player&, std::vector<int>&)> accept1,
                std::function<void(Player&, std::vector<int>&)> pre_action1, std::function<void(Player&, std::vector<int>&)> post_action1);
    bool check_available_for_buy_resources(std::vector<int> res);
    void change_available_for_buy_resources(std::vector<int> delta);
    int get_limit_Q(int res_id);
    double get_price(int res_id);
    void update_plots();

    // Tech
    void show(); // to console
    void load(std::ifstream& file);
    void dumpload(std::ofstream& file);

    // Not for frontend
    template <typename T>
    void print(std::ofstream& file, std::vector<T> v);
    template<typename T>
    void read(std::ifstream& file, std::vector<T>& v);

    int id;
    std::vector<Plot> items;
    std::vector<int> delta_per_round;
    std::function<bool(Player&, std::vector<int>&)> accept;
    std::function<void(Player&, std::vector<int>&)> pre_action;
    std::function<void(Player&, std::vector<int>&)> post_action;
    std::vector<int> army;
    int strategy;
};

//S_k * P + S_b(=0) + y_delta = D_k * P + D_b
//P = (D_b - y_delta) / (S_k - D_k)
struct Plot {
    double S_k;
    int S_b;
    double D_k;
    int D_b;
    int y_delta;
    int countQ() {
        return D_k * countP() + D_b;
    }
    double countP() {
        return (D_b - y_delta) / (S_k - D_k);
    }
};
