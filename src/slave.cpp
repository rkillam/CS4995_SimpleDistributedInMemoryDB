#include <iostream>
#include <libconfig.h++>
#include <unistd.h>
#include <zmqpp/zmqpp.hpp>

#include "Message.h"

int main (int argc, char *argv[]) {
    libconfig::Config conf;
    conf.readFile("db.conf");

    const libconfig::Setting& root = conf.getRoot();
    std::string port = root["SLAVE_PORT"];

    zmqpp::context context;
    zmqpp::socket requester(context, zmqpp::socket_type::reply);

    requester.connect("tcp://localhost:" + port);

    while(true) {
        zmqpp::message in_message;
        zmqpp::message out_message;

        requester.receive(in_message);

        Message message;
        Message::from_zmq_message(in_message, message);

        if(message.msgType == MSG_INSERT) {
            std::cout << "Received request to insert: " << message.data["key"] << std::endl;

            // Do some 'work'
            usleep(123456);

            //  Send reply back to client
            out_message << "Stored: " + message.data["key"];
        }

        requester.send(out_message);
    }
}

