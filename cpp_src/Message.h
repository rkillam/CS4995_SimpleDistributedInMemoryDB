#ifndef MESSAGE_H
#define MESSAGE_H

#include <unordered_map>
#include <zmqpp/zmqpp.hpp>


enum MessageType {
    INVALID,

    MSG_ACK,
    MSG_NACK,

    MSG_OK,
    MSG_INSERT,
    MSG_LOOKUP,
    MSG_KEY_ERROR,

    MSG_SHUTDOWN
};

class Message {
    public:
        MessageType msgType;
        std::unordered_map<std::string,std::string> data;

        Message();
        Message(MessageType, std::unordered_map<std::string,std::string>);

        static void from_zmq_message(zmqpp::message& zmq_message, Message& message);
        friend zmqpp::message& operator<<(zmqpp::message& message, const Message& msg);
};


#endif
