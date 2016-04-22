#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <zmqpp/zmqpp.hpp>


enum MessageType {
    MSG_ACK,
    MSG_NACK,
    MSG_ENDPOINT,
    MSG_INSERT,
    MSG_SHUTDOWN
};

class Message {
    public:
        MessageType msgType;
        std::unordered_map<std::string,std::string> data;

        Message();
        Message(MessageType, std::unordered_map<std::string,std::string>);

        // std::string operator[](const std::string& key) const {return data[key];}  // Get
        // std::string& operator[](const std::string& key) {return data[key];}  // Set

        friend zmqpp::message& operator<<(zmqpp::message &message, const Message& msg);
        // friend zmqpp::message& operator>>(zmqpp::message &message, Message& msg);
};


#endif
