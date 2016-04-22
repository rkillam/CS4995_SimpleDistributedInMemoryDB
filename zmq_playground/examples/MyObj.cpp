#include "MyObj.h"

MyObj* newMyObj(int x, int y) {
    MyObj* obj = new MyObj;
    obj->x = x;
    obj->y = y;

    return obj;
}

MyObj* fromZmqMessage(zmqpp::message* message) {
    MyObj* obj = new MyObj;

    int x, y;
    (*message) >> x >> y;
    obj->x = x;
    obj->y = y;

    return obj;
}
