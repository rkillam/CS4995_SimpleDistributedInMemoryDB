#include <iostream>
#include <libconfig.h++>
#include <unordered_map>
#include <zmqpp/zmqpp.hpp>

#include "Message.h"
#include "utils.h"

int main (int argc, char* argv[]){
    libconfig::Config conf;
    conf.readFile("db.conf");

    const libconfig::Setting& root = conf.getRoot();
    std::string port = root["MASTER_PORT"];

    zmqpp::context context;
    zmqpp::socket requester(context, zmqpp::socket_type::request);

    requester.connect("tcp://localhost:" + port);

    std::string cmd = "";
    while(cmd != "q") {
        std::cout << "q to quit" << std::endl
                  << "i to insert key value pair" << std::endl
                  << "l to lookup key value pair" << std::endl
                  << "> ";

        std::cin >> cmd;

        Message msg;
        if(cmd == "i") {
            std::cout << "Key > ";

            std::string key;
            std::cin >> key;

            msg.msgType = MSG_INSERT;
            msg.data["key"] = key;
        }
        else if(cmd == "l") {
            // std::cout << "Key > ";
            // std::cin >> msg;
        }

        // FIXME
        if(true) {
            zmqpp::message message;
            message << msg;

            requester.send(message);
            receive_entire_message(requester, message);

            std::string reply;
            message >> reply;
            std::cout << "Received reply: " << reply << std::endl;
        }
    }

    return 0;
}
