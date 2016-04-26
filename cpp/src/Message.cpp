#include "Message.h"

#include <iostream>
#include <sstream>
#include <string>

#include "utils.h"


const char MSG_DELIMITER ='`';


Message::Message() {
    this->msgType = INVALID;
    this->data = std::unordered_map<std::string,std::string>();
}


Message::Message(MessageType msgType, std::unordered_map<std::string,std::string>) {
    this->msgType = msgType;
    this->data = data;
}

zmqpp::message& operator<<(zmqpp::message &message, const Message& msg) {
    std::ostringstream oss;
    oss << msg.msgType;

    for(auto it = msg.data.begin(); it != msg.data.end(); ++it) {
        oss << std::endl << it->first << MSG_DELIMITER << it->second;
    }

    message << oss.str();
    return message;
}

void Message::from_zmq_message(zmqpp::message& zmq_message, Message& message) {
    std::string msg;
    zmq_message >> msg;
    std::vector<std::string> lines = split(msg, '\n');

    message.msgType = static_cast<MessageType>(std::stoi(lines.at(0)));

    lines.erase(lines.begin());

    for(auto line = lines.begin(); line != lines.end(); ++line) {
        std::vector<std::string> key_pair = split((*line), MSG_DELIMITER);

        message.data[key_pair.at(0)] = key_pair.at(1);
    }
}
