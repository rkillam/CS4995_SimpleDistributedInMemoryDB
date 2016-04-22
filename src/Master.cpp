#include <iostream>
#include <libconfig.h++>
#include <string>
#include <zmqpp/zmqpp.hpp>

#include "Message.h"


class SlaveConnection {
    private:
        std::string protocol;
        std::string ip;
        std::string port;

        std::string push_endpoint;
        std::string pull_endpoint;

        // initialize the 0MQ context
        zmqpp::context context;

    public:
        SlaveConnection(std::string protocol, std::string ip, std::string port) {
            this->protocol = protocol;
            this->ip = ip;
            this->port = port;

            this->push_endpoint = this->protocol + "://" + this->ip + ":" + this->port;
            this->pull_endpoint = this->protocol + "://" + "*" + ":" + this->port;

            // TODO: stop hard coding local IP
            std::string myIP = "192.168.30.5";
            std::string myEndpoint = this->protocol + "://" + myIP + ":" + this->port;

            Message endpointMsg = Message(ENDPOINT, myEndpoint);
            this->send_message(endpointMsg);
        }

        void send_message(Message& msg) {
            zmqpp::socket push_socket = zmqpp::socket(this->context, zmqpp::socket_type::push);
            push_socket.connect(this->push_endpoint);

            zmqpp::message message;
            message << msg;

            push_socket.send(message);

            push_socket.disconnect(this->push_endpoint);

            Message* ack = 0;
            while(ack == 0 || ack->msgType != ACK) {
                if(ack != 0) {
                    delete ack;
                    ack = 0;
                }

                try {
                    ack = this->read_message();
                }
                catch(zmqpp::zmq_internal_exception& e) {
                    std::cout << "got exception?" << std::endl;
                }
            }

            std::cout << "Received ACK" << std::endl;
            delete ack;
        }

        Message* read_message() {
            zmqpp::socket pull_socket = zmqpp::socket(this->context, zmqpp::socket_type::pull);
            pull_socket.bind(this->pull_endpoint);

            zmqpp::message message;
            pull_socket.receive(message);

            Message* msg = new Message();
            message >> (*msg);

            pull_socket.disconnect(this->pull_endpoint);

            return msg;
        }

        std::string get_push_endpoint() {
            return this->push_endpoint;
        }
};


int main(int argc, char *argv[]) {
    libconfig::Config conf;
    conf.readFile("db.conf");

    const libconfig::Setting& root = conf.getRoot();
    std::string port = root["PORT"];

    const libconfig::Setting& slave_settings = conf.lookup("Master.Slaves");

    std::vector<SlaveConnection> slaves;
    for(
        libconfig::Setting::const_iterator setting_iter = slave_settings.begin();
        setting_iter != slave_settings.end();
        ++setting_iter) {

        std::string ip = (*setting_iter);

        std::cout << "Creating new slave at " << ip << std::endl;
        slaves.push_back(SlaveConnection(protocol, ip, port));
    }

    std::cout << slaves.size() << std::endl;
    for(unsigned int i = 0; i < 3; ++i) {
        for(std::vector<SlaveConnection>::iterator slave = slaves.begin();
            slave != slaves.end();
            ++slave) {

            std::cout << slave->get_push_endpoint() << std::endl;

            if(i < 2) {
                std::cout << "sending hello" << std::endl;
                Message hello = Message(INSERT, "hello");
                slave->send_message(hello);
                std::cout << "sent hello" << std::endl;
            }
            else {
                std::cout << "sending shutdown" << std::endl;
                Message shutdown = Message(INSERT, "hello");
                slave->send_message(shutdown);
                std::cout << "sent shutdown" << std::endl;
            }
        }
    }

    return 0;
}
