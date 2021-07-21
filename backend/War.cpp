#include "War.h"
#include "Game.h"


// перевод айдишника
int gid(int id)
{
	return (-id - 1);
}

void War::show(){
	std::cout<<step<<"\n";

	std::cout<<"a_side_ids: ";
	for(auto i:a_side_ids)
		std::cout<<i<<" ";
	std::cout<<std::endl;
	std::cout<<"a_size: ";
	for(auto i:a_size)
		std::cout<<i<<" ";
	std::cout<<std::endl;

	std::cout<<"d_side_ids: ";
	for(auto i:d_side_ids)
		std::cout<<i<<" ";
	std::cout<<std::endl;
	std::cout<<"d_size: ";
	for(auto i:d_size)
		std::cout<<i<<" ";
	std::cout<<std::endl;

	std::cout<<"a_army: "<<a_army.size()<<std::endl;
	std::cout<<"d_army: "<<d_army.size()<<std::endl;

	std::cout<<attacker_id<<" "<<defender_id<<std::endl;
	std::cout<<total_att<<" "<<total_def<<std::endl;
}

War::War(int _attacker_id, int _defender_id)
{
	attacker_id = _attacker_id;
	defender_id = _defender_id;
	a_side_ids = {attacker_id};
	d_side_ids = {defender_id};
	step=0;
	total_def = total_att = 0;
	init_war();
}

// bool War::add_attacker(int id)
// {
// 	a_side_ids.push_back(id);
// 	return true;
// }

// bool War::add_defender(int id)
// {
// 	d_side_ids.push_back(id);
// 	return true;
// }

bool War::someone_won(int who){
	//attacker
	if(who==1){
		std::vector<int> resources(11);
		for(auto did:d_side_ids){
			double coeff=(did==defender_id?1:0.5);
			for(int i=0;i<11;i++){
				if(did<0){
					resources[i]+=coeff*Game::current().cities[-did-1].items[i].countQ();
				 	continue;
				}
				resources[i]+=coeff*Game::current().players[did].resources[i];
				Game::current().players[did].resources[i]-=(int)(coeff*Game::current().players[did].resources[i]);
			}
		}
		for(int i=0;i<a_side_ids.size();i++){
			int aid=a_side_ids[i];
			for(int j=0;j<11;j++){
				if(aid<0) continue;
				Game::current().players[aid].resources[j]+=(int)((a_size[i]/total_att)*resources[j]);
			}
		}
	}
	else{
		std::vector<int> resources(11);
		for(auto aid:a_side_ids){
			double coeff=(aid==attacker_id?1:0.5);
			for(int i=0;i<11;i++){
				if(aid<0) continue;
				resources[i]+=coeff*Game::current().players[aid].resources[i];
				Game::current().players[aid].resources[i]-=(int)(coeff*Game::current().players[aid].resources[i]);
			}
		}
		for(int i=0;i<d_side_ids.size();i++){
			int did=d_side_ids[i];
			for(int j=0;j<11;j++){
				if(did<0) continue;
				Game::current().players[did].resources[j]+=(int)((d_size[i]/total_def)*resources[j]);
			}
		}
	}
	return true;
}

bool War::init_war()
{
	total_att=0;
	total_def=0;
	a_size={};
	d_size={};
	for(auto aid:a_side_ids){
		std::vector<int> temp=Game::current().get_player_army(aid);
		a_size.push_back(std::accumulate(temp.begin(),temp.end(),0));
		total_att+=(a_size.back());
		if(aid<0) continue;
		Game::current().players[aid].last_attack=0;
	}
	for(auto did:d_side_ids){
		std::vector<int> temp=Game::current().get_player_army(did);
		d_size.push_back(std::accumulate(temp.begin(),temp.end(),0));
		total_def+=(d_size.back());
		if(did<0) continue;
	}

	a_army = {}, d_army = {};
	for (auto aid : a_side_ids)
	{
		std::vector<int> ith_army = Game::current().get_player_army(aid);
		for (int tp = 0; tp < 3; tp++)
			for (int i = 0; i < ith_army[tp]; i++){
				a_army.push_back(std::make_pair(tp,aid));
				// total_att++;
				}
	}
	for (auto did : d_side_ids)
	{
		std::vector<int> ith_army = Game::current().get_player_army(did);
		for (int tp = 0; tp < 3; tp++)
			for (int i = 0; i < ith_army[tp]; i++){
				d_army.push_back(std::make_pair(tp,did));
				// total_def++;			
			}
	}
	return true;
}

bool War::progress_war()
{
	if(total_att==0 && total_def==0){
		step=5;
	}
	if(total_att==0 && total_def!=0){
		step=5;
		someone_won(0);
	}
	if(total_att!=0 && total_def==0){
		step=5;
		someone_won(1);
	}
	step++;
	if (step == 1)
		init_war();
	if(step==5){
		step=4;
		return false;
	}
	std::srand(std::time(nullptr));
	double survivors_pc = (step == 4 ? 0 : 1 - step * 0.3);
	while (a_army.size() != 0 and d_army.size() != 0 and ((double)a_army.size() / total_att >= survivors_pc) and ((double)d_army.size() / total_def >= survivors_pc))
	{
		int a_troop_n = (std::rand()) % a_army.size();
		int a_troop_type = a_army[a_troop_n].first;
		int a_troop_id = a_army[a_troop_n].second;
		int a_strat = Game::current().get_player_strat(a_troop_id);
		int d_troop_n = (std::rand()) % d_army.size();
		int d_troop_type = d_army[d_troop_n].first;
		int d_troop_id = d_army[d_troop_n].second;
		int d_strat = Game::current().get_player_strat(d_troop_id);
		// 0 1, 1 2, 2 0
		double a_wins = 0.5;
		if ((a_troop_type + 1) % 3 == d_troop_type)
		{
			a_wins += 0.2;
		}
		if (a_troop_type == (d_troop_type + 1) % 3)
		{
			a_wins -= 0.2;
		}
		if (a_troop_type == a_strat)
		{
			a_wins += 0.1;
		}
		if ((a_troop_type + 1) % 3 == a_strat)
		{
			a_wins -= 0.1;
		}
		if (d_troop_type == d_strat)
		{
			a_wins -= 0.1;
		}
		if ((d_troop_type + 1) % 3 == d_strat)
		{
			a_wins += 0.1;
		}
		// Если кто-то проебался
		if (a_wins > 1)
			a_wins = 1;
		if (a_wins < 0)
			a_wins = 0;
		double roll = (((double)std::rand()) / RAND_MAX);
		if (roll <= a_wins)
		{
			d_army[d_troop_n] = std::make_pair(-1, -1);
			d_army.erase(std::find(d_army.begin(), d_army.end(), std::make_pair(-1, -1)));
			Game::current().kill_troop(d_troop_id,d_troop_type);
		}
		else{
			a_army[a_troop_n] = std::make_pair(-1,-1);
			a_army.erase(std::find(a_army.begin(),a_army.end(), std::make_pair(-1,-1)));
			Game::current().kill_troop(a_troop_id,a_troop_type);
		}
	}
	if(d_army.size()==0){
		step=4;
		someone_won(1);
	}
	else if(a_army.size()==0){
		step=4;
		someone_won(0);
	}
	return true;
}
// 1 оборонительный
bool War::add_by_treaty(int id){
	if(std::find(a_side_ids.begin(),a_side_ids.end(),id)!=a_side_ids.end() or std::find(d_side_ids.begin(),d_side_ids.end(),id)!=d_side_ids.end())
		return 0;
	if(Game::current().players[id].treaties[1].count(defender_id)){
		d_side_ids.push_back(id);
		init_war();
		return 1;
	}
	return 0;
}

bool War::add_indie_side(int id, int is_attacker){
	if(std::find(a_side_ids.begin(),a_side_ids.end(),id)!=a_side_ids.end() or std::find(d_side_ids.begin(),d_side_ids.end(),id)!=d_side_ids.end())
		return 0;
	if(is_attacker){
		for(auto did:d_side_ids){
			if(Game::current().players[id].treaties[0].count(did) || Game::current().players[id].treaties[1].count(did)){
				Game::current().players[id].ban = 1;
				Game::current().players[id].treaties[0].erase(did);
				Game::current().players[id].treaties[1].erase(did);
				Game::current().players[did].treaties[0].erase(id);
				Game::current().players[did].treaties[1].erase(id);
			}
		}
		a_side_ids.push_back(id);
	}
	else{
		for(auto aid:a_side_ids){
			if(Game::current().players[id].treaties[0].count(aid) || Game::current().players[id].treaties[1].count(aid)){
				Game::current().players[id].ban = 1;
				Game::current().players[id].treaties[0].erase(aid);
				Game::current().players[id].treaties[1].erase(aid);
				Game::current().players[aid].treaties[0].erase(id);
				Game::current().players[aid].treaties[1].erase(id);
			}
		}
		d_side_ids.push_back(id);
	}
	init_war();
	return 1;
}


bool War::break_defence_treaties(){
	for(int id=0;id<Game::current().players.size();id++){
		if(Game::current().players[id].treaties[1].count(defender_id)!=0 && std::find(d_side_ids.begin(),d_side_ids.end(),id)==d_side_ids.end()){
			Game::current().players[id].ban = 1;
			Game::current().players[id].treaties[0].erase(defender_id);
			Game::current().players[id].treaties[1].erase(defender_id);
			Game::current().players[defender_id].treaties[0].erase(id);
			Game::current().players[defender_id].treaties[1].erase(id);
		}
	}
	init_war();
	return 1;
}
