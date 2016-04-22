#include "utils.h"

void receive_entire_message(zmqpp::socket& skt, zmqpp::message& msg) {
    skt.receive(msg);

    while(skt.has_more_parts()) {
        skt.receive(msg);
    }
}
