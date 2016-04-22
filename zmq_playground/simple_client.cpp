/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file is part of zmqpp.
 * Copyright (c) 2011-2015 Contributors as noted in the AUTHORS file.
 */

#include <zmqpp/zmqpp.hpp>
#include <string>
#include <iostream>
#include "MyObj.h"

using namespace std;

int main(int argc, char *argv[]) {
  const string endpoint = "tcp://192.168.30.5:4242";

  // initialize the 0MQ context
  zmqpp::context context;

  // generate a push socket
  zmqpp::socket_type type = zmqpp::socket_type::push;
  zmqpp::socket socket (context, type);

  // open the connection
  cout << "Opening connection to " << endpoint << "..." << endl;
  socket.connect(endpoint);

  // send a message
  cout << "Sending text and a number..." << endl;
  zmqpp::message message;

  // MyObj* obj = newMyObj(4, 2);
  MyObj obj = MyObj(4, 2);
  cout << "obj.x: " << obj.x << "; obj.y: " << obj.y << endl;

  // compose a message from a string and a number
  message << obj;
  socket.send(message);

  cout << "Sent message." << endl;
  cout << "Finished." << endl;
}
