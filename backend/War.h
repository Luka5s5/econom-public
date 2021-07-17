#pragma once

#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<numeric>

struct War {
	War(int _attacker_id, int _defender_id);
    bool init_war();
	bool add_attacker(int id);
	bool add_defender(int id);
	bool progress_war();
	bool someone_won(int who);
	// bool someone_conceded(int who);

	// Tech
	void show(); // to console

    std::vector<int> a_side_ids;
    std::vector<int> d_side_ids;
    std::vector<int> a_size;
    std::vector<int> d_size;
    // first - troopid, second - playerid
    std::vector<std::pair<int,int>> a_army;
    std::vector<std::pair<int,int>> d_army;
    int attacker_id;
    int defender_id;
    int total_att;
    int total_def;
    //От 0 до 4: 0 не началась 1 30% 2 60% 3 90% 4 100%
    int step;
};
