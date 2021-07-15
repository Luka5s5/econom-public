#include <vector>

struct Player {
    int id;
    std::vector<double> currencies;
    std::vector<int> resources;
    std::vector<int> building_levels;
    std::vector<int> army;
    std::vector<std::set<int>> treaties; // 3
    int fee;
    int last_attack;
    int strategy; 
};
