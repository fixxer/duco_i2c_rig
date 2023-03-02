#ifndef DUINOCOIN_CLIENTS_H
#define DUINOCOIN_CLIENTS_H

#include <Arduino.h>

String clients_show();
String printMOTD();
void SetMiningKey(String key);
String getHostPort();
String SetHost(String h);
int SetPort(int p);
void SetHostPort(String h, int p);
void force_clients_reconnect();
void clients_loop();
void clients_waitFeedbackJobDone(byte i);
String clients_string();
void clients_waitMOTD(byte i);
void clients_requestMOTD(byte i);
void periodic_report(unsigned long interval);

#endif