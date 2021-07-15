#include <vector>

struct Player {
    Player(int id1) {
        id = id1;
        currencies = std::vector<double>(4);
        resources = std::vector<int>(11);
        building_levels = std::vector<int>(11);
        for (int i = 0; i < 7; i++) {
            building_levels[i] = 1;
        }
        army = std::vector<int>(3);
        last_attack = -100;
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
