#pragma once

#include <functional>
#include "Player.h"
#include <vector>
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

struct City {
    City(int id1, std::vector<std::vector<double>> plots, std::function<bool(Player&, std::vector<int>)> accept1,
                std::function<void(Player&, std::vector<int>)> pre_buy1, std::function<void(Player&, std::vector<int>)> post_buy1,
                std::function<void(Player&, std::vector<int>)> pre_sell1, std::function<void(Player&, std::vector<int>)> post_sell1) {
        id = id1;
        for (int i = 0; i < plots.size(); i++) {
            items.push_back(Plot{plots[i][0], (int)plots[i][1], plots[i][2], (int)plots[i][3]});
        }
        delta_per_round = std::vector<int>(11);
        accept = accept1;
        pre_buy = pre_buy1;
        post_buy = post_buy1;
        pre_sell = pre_sell1;
        post_sell = post_sell1;
        army = {1000, 1000, 1000};
        strategy = 3;
    }
    
    int id;
    std::vector<Plot> items;
    std::vector<int> delta_per_round;
    std::function<bool(Player&, std::vector<int>)> accept;
    std::function<void(Player&, std::vector<int>)> pre_buy;
    std::function<void(Player&, std::vector<int>)> post_buy;
    std::function<void(Player&, std::vector<int>)> pre_sell;
    std::function<void(Player&, std::vector<int>)> post_sell;
    
    std::vector<int> army;
    int strategy;
    
    bool check_available_for_buy_resources(std::vector<int> res) {
        if (res.size() != delta_per_round.size())
            return false;
        for (int i = 0; i < res.size(); i++) {
            if (res[i] > get_limit_Q(i)) {
                return false;
            }
        }
        return true;
    }
    void change_available_for_buy_resources(std::vector<int> delta) { 
        for (int i = 0; i < delta.size(); i++) {
            delta_per_round[i] += delta[i];
        }
    }
    int get_limit_Q(int res_id) {
        return items[res_id].D_b - items[res_id].y_delta - delta_per_round[res_id];
    }
    double get_price(int res_id) {
        return items[res_id].countP(); 
    }
    void update_plots() {
        for (int i = 0; i < items.size(); i++) {
            items[i].y_delta += delta_per_round[i];
            delta_per_round[i] = 0;
        }
    } 
};
