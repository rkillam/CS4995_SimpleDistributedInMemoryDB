#include "Message.h"


const std::string MSG_DELIMITER = "<MESSAGE_DELIMITER>";


Message::Message() {
    this->msgType = MSG_NACK;
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

// zmqpp::message& operator>>(zmqpp::message &message, Message& msg) {
    // int msgType;
    // message >> msgType >> msg.data;
//
    // msg.msgType = static_cast<MessageType>(msgType);
//
    // return message;
// }
