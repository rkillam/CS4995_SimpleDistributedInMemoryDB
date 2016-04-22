#include <iostream>
#include <libconfig.h++>
#include <unordered_map>
#include <zmqpp/zmqpp.hpp>

#include "Message.h"

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
            msg.msgType = MSG_INSERT;

            std::cout << "Key > ";
            std::string key;
            std::cin >> key;
            msg.data["key"] = key;

            std::cout << "Value > ";
            std::string value;
            std::cin >> value;
            msg.data["value"] = value;
        }
        else if(cmd == "l") {
            msg.msgType = MSG_LOOKUP;

            std::cout << "Key > ";
            std::string key;
            std::cin >> key;
            msg.data["key"] = key;
        }

        if(!msg.msgType == INVALID) {
            zmqpp::message message;
            message << msg;

            requester.send(message);
            requester.receive(message);

            std::string reply;
            message >> reply;
            std::cout << "Received reply: " << std::endl << reply << std::endl;
        }
    }

    return 0;
}
