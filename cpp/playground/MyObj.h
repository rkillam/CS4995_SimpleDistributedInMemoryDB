#include <iostream>
#include <zmqpp/zmqpp.hpp>


class MyObj {
    public:
        int x;
        int y;
        MyObj();
        MyObj(int, int);

        friend zmqpp::message& operator<<(zmqpp::message &message, const MyObj& obj);
        friend zmqpp::message& operator>>(zmqpp::message &message, MyObj& obj);
};
