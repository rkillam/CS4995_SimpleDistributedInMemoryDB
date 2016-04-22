#include <iostream>
#include <libconfig.h++>
#include <unistd.h>
#include <zmqpp/zmqpp.hpp>

#include "Message.h"
#include "utils.h"

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
        receive_entire_message(requester, in_message);

        std::string request;
        in_message >> request;

        std::cout << "Received request: " << request << std::endl;

        // Do some 'work'
        usleep(123456);

        //  Send reply back to client
        zmqpp::message out_message;
        out_message << "Stored: " + request;
        requester.send(out_message);
    }
}

