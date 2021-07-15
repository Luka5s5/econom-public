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
    std::cout << "~~ " << j["energy"] << std::endl;
    print_server.send(conn,"Helloworld",msg->get_opcode());
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
