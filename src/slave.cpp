#include <iostream>
#include <libconfig.h++>
#include <unistd.h>
#include <unordered_map>
#include <zmqpp/zmqpp.hpp>

#include "Message.h"

int main (int argc, char *argv[]) {
    libconfig::Config conf;
    conf.readFile("db.conf");

    const libconfig::Setting& root = conf.getRoot();
    std::string master_ip = root["MASTER_IP"];
    std::string port = root["SLAVE_PORT"];

    zmqpp::context context;
    zmqpp::socket requester(context, zmqpp::socket_type::reply);

    requester.connect("tcp://" + master_ip + ":" + port);

    std::unordered_map<std::string,std::string> kv_store;
    while(true) {
        zmqpp::message in_message;
        zmqpp::message out_message;

        requester.receive(in_message);

        Message message;
        Message::from_zmq_message(in_message, message);
        std::cout << "Remote" << std::endl;

        if(message.msgType == MSG_INSERT) {
            kv_store[message.data.at("key")] = message.data.at("value");

            message = Message();
            message.msgType = MSG_OK;
            out_message << message;
        }
        else if(message.msgType == MSG_LOOKUP) {
            Message reply;

            try {
                reply.data["value"] = kv_store.at(message.data.at("key"));

                reply.msgType = MSG_OK;
            }
            catch(std::out_of_range& e) {
                reply.msgType = MSG_KEY_ERROR;
            }

            out_message << reply;
        }

        // Send reply back to client
        requester.send(out_message);
    }
}
