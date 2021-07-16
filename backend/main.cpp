#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

typedef websocketpp::server<websocketpp::config::asio> server;

server print_server;

void on_message(websocketpp::connection_hdl conn, server::message_ptr msg) {
    std::cout << "~~~~~ " << msg->get_payload() << std::endl;
    auto j = json::parse(msg->get_payload());
    if(j["type"] == "update-all"){
        json response;
        std::cout << "1\n";
        response["resource_names"] = {"iron","gold","copper","lumber","grain","meat","wine"};
        std::cout << "1\n";
        response["teamlist"] = {
            json::object({{"name","team1"},{"id",1},
                         {"treaties",json::array({
                             json::array({2}),
                             json::array({})
                        })}}),
            json::object({{"name","team2"},{"id",2},
                         {"treaties",json::array({
                             json::array({1}),
                             json::array({})
                        })}})
        };
        std::cout << "1\n";
        response["cities"] = json::array({
            json::object({{"resources",json::array({
                json::object({{"limit",500},{"price",100}}),
                json::object({{"limit",50},{"price",10}}),
                json::object({{"limit",500},{"price",100}}),
                json::object({{"limit",50},{"price",10}}),
                json::object({{"limit",500},{"price",100}}),
                json::object({{"limit",50},{"price",10}}),
                json::object({{"limit",50},{"price",10}})
            })},{"name","varant"},{"id",0}}),
            json::object({{"resources",json::array({
                json::object({{"limit",5000},{"price",100}}),
                json::object({{"limit",500},{"price",10}}),
                json::object({{"limit",5000},{"price",100}}),
                json::object({{"limit",500},{"price",100}}),
                json::object({{"limit",5000},{"price",1000}}),
                json::object({{"limit",50},{"price",10000}}),
                json::object({{"limit",500},{"price",10}})
            })},{"name","liberty"},{"id",1}})
        });
        print_server.send(conn,response.dump(),msg->get_opcode());
    }
}

int main() {

    print_server.set_message_handler(&on_message);
    print_server.set_access_channels(websocketpp::log::alevel::all);
    print_server.set_error_channels(websocketpp::log::elevel::all);

    print_server.init_asio();
    print_server.listen(9002);
    print_server.start_accept();
    std::cout << "prepared to run\n";
    print_server.run();
}
