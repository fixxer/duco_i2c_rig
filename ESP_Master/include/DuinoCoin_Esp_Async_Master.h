#ifndef DUINOCOIN_ESP_ASYNC_MASTER_H
#define DUINOCOIN_ESP_ASYNC_MASTER_H

#if ESP8266
#define MINER "AVR I2C v3.4"
#define JOB "AVR"
#endif

#if ESP32
#define MINER "AVR I2C v3.4"
#define JOB "AVR"
#endif

#define REPEATED_WIRE_SEND_COUNT 1      // 1 for AVR, 8 for RP2040

void handleSystemEvents(void);

#endif