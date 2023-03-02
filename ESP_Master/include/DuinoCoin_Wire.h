#ifndef DUINOCOIN_WIRE_H
#define DUINOCOIN_WIRE_H

#include <Arduino.h>

void wire_setup();
void wire_SendAll(String message);
boolean wire_exists(byte address);
void wire_sendJob(byte address, String lastblockhash, String newblockhash, int difficulty);
void Wire_sendln(byte address, String message);
void Wire_sendCmd(byte address, String message);
String wire_readLine(int address);

#endif