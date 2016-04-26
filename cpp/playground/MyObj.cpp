#include "MyObj.h"

MyObj::MyObj() {
    this->x = 0;
    this->y = 0;
}

MyObj::MyObj(int x, int y) {
    this->x = x;
    this->y = y;
}

zmqpp::message& operator<<(zmqpp::message &message, const MyObj& obj) {
    message << obj.x << obj.y;
    return message;
}

zmqpp::message& operator>>(zmqpp::message &message, MyObj& obj) {
    message >> obj.x >> obj.y;
    return message;
}
