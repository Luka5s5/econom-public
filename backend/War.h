#include"Game.h"
#include<vector>
#include<cstdlib>
#include<ctime>
#include<algorithm>
struct War {
	War(int _attacker_id, int _defender_id);
	std::vector<int> a_side_ids;
	std::vector<int> d_side_ids;
	// first - troopid, second - playerid
	std::vector<std::pair<int,int>> a_army;
	std::vector<std::pair<int,int>> d_army;
	int attacker_id;
	int defender_id;
	//От 0 до 4: 0 не началась 1 30% 2 60% 3 90% 4 100%
	int step;
	bool add_attacker(int id);
	bool add_defender(int id);
	bool progress_war();
	bool init_war();
	bool someone_won(int who);
};
