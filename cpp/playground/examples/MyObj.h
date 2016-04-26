#include <zmqpp/zmqpp.hpp>

typedef struct MyObj_struct {
    int x;
    int y;
} MyObj;

MyObj* newMyObj(int, int);
MyObj* fromZmqMessage(zmqpp::message*);
