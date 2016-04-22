#include <iostream>
#include <libconfig.h++>
#include <string>
#include <zmqpp/zmqpp.hpp>

#include "Message.h"


Message* readMessage(std::string pull_endpoint) {
    zmqpp::context context;
    zmqpp::socket socket (context, zmqpp::socket_type::pull);

    socket.bind(pull_endpoint);
    zmqpp::message message;

    socket.receive(message);
    Message* msg = new Message();
    message >> (*msg);

    return msg;
}

void sendMessage(Message& msg, std::string push_endpoint) {
    zmqpp::context context;
    zmqpp::socket push_socket = zmqpp::socket(context, zmqpp::socket_type::push);
    push_socket.connect(push_endpoint);

    zmqpp::message message;
    message << msg;

    push_socket.send(message);

    push_socket.disconnect(push_endpoint);
}

int main(int argc, char *argv[]) {
    std::cout << "Starting slave" << std::endl;

    libconfig::Config conf;

    conf.readFile("db.conf");

    const libconfig::Setting& root = conf.getRoot();
    std::string protocol = root["PROTOCOL"];
    std::string port = root["PORT"];

    const std::string pull_endpoint = protocol + "://" + "*" + ":" + port;

    std::cout << "Looking for Hello from Master" << std::endl;

    Message* msg = 0;
    while(msg == 0 || msg->msgType != ENDPOINT) {
        if(msg != 0) {
            delete msg;
            msg = 0;
        }

        std::cout << "About to read messge: " << std::endl;
        msg = readMessage(pull_endpoint);
    }

    std::string push_endpoint = msg->data;
    std::cout << "Pushing to master at " << push_endpoint << std::endl;
    delete msg;

    Message ack = Message(ACK);
    sendMessage(ack, push_endpoint);
    std::cout << "Sent ACK" << std::endl;

    msg = 0;
    while(msg == 0 || msg->msgType != SHUTDOWN) {
        if(msg != 0) {
            delete msg;
            msg = 0;
        }

        std::cout << "About to read message" << std::endl;
        msg = readMessage(pull_endpoint);
        std::cout << "Received message: " << msg->data << std::endl;

        Message ack = Message(ACK);
        sendMessage(ack, push_endpoint);
        std::cout << "Sent ACK" << std::endl;
    }

    return 0;
}
