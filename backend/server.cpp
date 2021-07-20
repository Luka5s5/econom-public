#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <nlohmann/json.hpp>

#include <Game.h>

// for convenience
using json = nlohmann::json;


#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <set>

/*#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>*/
#include <websocketpp/common/thread.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

/* on_open insert connection_hdl into channel
 * on_close remove connection_hdl from channel
 * on_message queue send to all channels
 */

static const std::string token = "cdJc33MFUUkk0npgtc2c";

static const char* city_names[] = {
    "Варант",
    "Либерти",
    "Мордор",
    "Кель'Талас"
};

json serializePlayer(Player& player){
    json pl;
    pl["id"] = player.id;
    pl["name"] = player.name;
    pl["treaties"] = player.treaties;
    pl["strategy"] = player.strategy;
    pl["army"] = player.army;
    pl["resources"] = player.resources;
    pl["buildings"] = player.building_levels;
    pl["money"] = player.currencies;
    auto resp = Game::current().get_victory_points(player.id);
    pl["victory_points"] = resp.int_number;
    return pl;
}

json serializeCity(City& city){
    json ct;
    ct["id"] = city.id;
    ct["name"] = city_names[city.id];
    ct["army"] = city.army;
    ct["resources"] = json::array({});
    for(int i = 0; i < city.items.size(); i++){
        json aa;
        aa["price"] = city.get_price(i);
        aa["limit"] = city.get_limit_Q(i);
        ct["resources"].push_back(aa);
    }
    return ct;
}

json serializeWar(War& war){
    json w;
    w["attacker"] = war.attacker_id;
    w["defender"] = war.defender_id;
    w["attackers_list"] = war.a_side_ids;
    w["defenders_list"] = war.d_side_ids;
    w["step"] = war.step;
    std::vector<int> a_army(3);
    for(auto a : war.a_army){
        a_army[a.first]++;
    }
    w["attacker_army"] = a_army;
    w["attackers_initial_army"] = war.total_att;
    std::vector<int> d_army(3);
    for(auto a : war.d_army){
        d_army[a.first]++;
    }
    w["defender_army"] = d_army;
    w["defenders_initial_army"] = war.total_def;
    return w;
}

json getAllInfo(){
    Game& game = Game::current();
    json response;
    response["resource_names"] = {"золото",
        "железо",
        "медь",
        "дерево",
        "зерно",
        "мясо",
        "вино",
        "священные тексты",
        "повозки",
        "шахтёрские инструменты",
        "алхимические реагенты"  
    };
    response["army_names"] = {"Кавалерия",
        "Копейщики",
        "Лучники"
    };
    response["building_names"] = {"Золотой рудник",
        "Железный рудник",
        "Медный рудник",
        "Лесопилка",
        "Поля",
        "Ферма",
        "Виноградники",
        "Шахтёрская ассоциация",
        "Алхимическая гильдия",
        "Вольная торговая гильдия",
        "Монастырь"
    };
    response["teamlist"] = json::array({});
    response["is_cycle"] = game.is_cycle;
    for(auto pl : game.players){
        response["teamlist"].push_back(serializePlayer(pl));
    }
    
    response["cities"] = json::array({});
    for(auto city : game.cities){
        response["cities"].push_back(serializeCity(city));
    }
    
    response["cities"] = json::array({});
    for(auto city : game.cities){
        response["cities"].push_back(serializeCity(city));
    }
    
    response["wars"] = json::array({});
    for(auto war : game.wars){
        response["wars"].push_back(serializeWar(war));
    }
    
    return response;
    
}



enum action_type {
    SUBSCRIBE,
    UNSUBSCRIBE,
    MESSAGE
};

struct action {
    action(action_type t, connection_hdl h) : type(t), hdl(h) {}
    action(action_type t, connection_hdl h, server::message_ptr m)
      : type(t), hdl(h), msg(m) {}

    action_type type;
    websocketpp::connection_hdl hdl;
    server::message_ptr msg;
};

class broadcast_server {
public:
    bool parse_message(websocketpp::connection_hdl conn, server::message_ptr msg) {
        Game& game = Game::current();
        std::cout << "~~~~~ " << msg->get_payload() << std::endl;
        auto j = json::parse(msg->get_payload());
        if(j["type"] == "update-all"){
            json response = getAllInfo();
            m_server.send(conn,response.dump(),msg->get_opcode());
            return false;
        }
        if(!j.contains("token") || (j.contains("token") && j["token"] == "")) {
            json response;
            response["type"] = "response";
            response["message"] = "No token, operation is forbidden";
            m_server.send(conn,response.dump(),msg->get_opcode());
            return false;
        }
        if(j.contains("token") && j["token"] != token){
            std::cout << "invalid token " << j["token"] << std::endl;
            json response;
            response["type"] = "response";
            response["message"] = "Invalid token, operation is forbidden";
            m_server.send(conn,response.dump(),msg->get_opcode());
            return false;
        }
        if(j["type"] == "register"){
            Response r = game.register_player(j["name"]);
            std::cout << std::string(j["name"]) << std::endl;
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else
        if(j["type"] == "newcycle"){
            Response r = game.start_cycle();
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else
        if(j["type"] == "endcycle"){
            Response r = game.end_cycle();
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else
        if(j["type"] == "exchange"){
            int id1 = j["team1"];
            int id2 = j["team2"];
            std::vector<int> resources = j["resources"];
            std::vector<double> money = j["money"];
            Response r = game.trade(id1,id2,money,resources);
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else 
        if(j["type"] == "sign"){
            int id1 = j["team1"];
            int id2 = j["team2"];
            int type = j["treatyType"];
            Response r = game.add_treaty(id1,id2,type);
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else 
        if(j["type"] == "terminate"){
            int id1 = j["team1"];
            int id2 = j["team2"];
            int type = j["treatyType"];
            Response r = game.remove_treaty(id1,id2,type);
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else 
        if(j["type"] == "war"){
            int id1 = j["team1"];
            int id2 = j["team2"];
            Response r = game.declare_war(id1,id2);
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else
        if(j["type"] == "trade"){
            int id_team = j["team"];
            int id_city = j["city"];
            std::vector<int> resources = j["resources"];
            Response r = game.sell_query(id_team,id_city,resources);
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else
        if(j["type"] == "proceed_top_war"){
            Response r = game.proceed_top_war();
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else
        if(j["type"] == "concede"){
            int who = j["is_attack_won"];
            Response r = game.concede_top_war(who);
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else
        if(j["type"] == "peace"){
            Response r = game.stop_top_war();
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else
        if(j["type"] == "change_strat"){
            Response r = game.set_strategy(j["team"],j["strat"]);
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
            return r.result;
        }else
        if(j["type"] == "upgrade-building"){
            Response r = game.upgrade_building(j["team"],j["building"]);
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode());
        }else
        if(j["type"] == "buy_army"){
            Response r = game.upgrade_army(j["team"],j["army"]);
            json response;
            response["type"] = "response";
            response["message"] = r.response;
            m_server.send(conn,response.dump(),msg->get_opcode()); 
        }
        return false;
    }
    
    broadcast_server() {
        m_server.set_access_channels(websocketpp::log::alevel::all);
        m_server.set_error_channels(websocketpp::log::elevel::all);
        // Initialize Asio Transport
        m_server.init_asio();

        // Register handler callbacks
        m_server.set_open_handler(bind(&broadcast_server::on_open,this,::_1));
        m_server.set_close_handler(bind(&broadcast_server::on_close,this,::_1));
        m_server.set_message_handler(bind(&broadcast_server::on_message,this,::_1,::_2));
    }

    void run(uint16_t port) {
        // listen on specified port
        m_server.listen(port);

        // Start the server accept loop
        m_server.start_accept();

        // Start the ASIO io_service run loop
        try {
            m_server.run();
        } catch (const std::exception & e) {
            std::cout << e.what() << std::endl;
        }
    }

    void on_open(connection_hdl hdl) {
        {
            lock_guard<mutex> guard(m_action_lock);
            //std::cout << "on_open" << std::endl;
            m_actions.push(action(SUBSCRIBE,hdl));
        }
        m_action_cond.notify_one();
    }

    void on_close(connection_hdl hdl) {
        {
            lock_guard<mutex> guard(m_action_lock);
            //std::cout << "on_close" << std::endl;
            m_actions.push(action(UNSUBSCRIBE,hdl));
        }
        m_action_cond.notify_one();
    }

    void on_message(connection_hdl hdl, server::message_ptr msg) {
        // queue message up for sending by processing thread
        {
            lock_guard<mutex> guard(m_action_lock);
            //std::cout << "on_message" << std::endl;
            m_actions.push(action(MESSAGE,hdl,msg));
        }
        m_action_cond.notify_one();
    }

    void process_messages() {
        while(1) {
            unique_lock<mutex> lock(m_action_lock);

            while(m_actions.empty()) {
                m_action_cond.wait(lock);
            }

            action a = m_actions.front();
            m_actions.pop();

            lock.unlock();

            if (a.type == SUBSCRIBE) {
                lock_guard<mutex> guard(m_connection_lock);
                m_connections.insert(a.hdl);
            } else if (a.type == UNSUBSCRIBE) {
                lock_guard<mutex> guard(m_connection_lock);
                m_connections.erase(a.hdl);
            } else if (a.type == MESSAGE) {
                lock_guard<mutex> guard(m_connection_lock);
                bool broadcast = parse_message(a.hdl,a.msg);
                if(broadcast){
                    json response = getAllInfo();
                    con_list::iterator it;
                    for (it = m_connections.begin(); it != m_connections.end(); ++it) {
                        m_server.send(*it,response.dump(),a.msg->get_opcode());
                    }
                }
            } else {
                // undefined.
            }
        }
    }
private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl> > con_list;

    server m_server;
    con_list m_connections;
    std::queue<action> m_actions;

    mutex m_action_lock;
    mutex m_connection_lock;
    condition_variable m_action_cond;
};

int main(int argc, char *argv[]) {
    if(argc > 1)
        Game::current().load(argv[1]);
    Game::current().init();
    try {
        
    broadcast_server server_instance;

    // Start a thread to run the processing loop
    thread t(bind(&broadcast_server::process_messages,&server_instance));

    // Run the asio loop with the main thread
    server_instance.run(9002);

    t.join();

    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}

