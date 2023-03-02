#ifndef DUINOCOIN_ASYNCWEBSERVER_H
#define DUINOCOIN_ASYNCWEBSERVER_H

#include <Arduino.h>

void server_setup();
void ws_sendAll(String payload);

#endif