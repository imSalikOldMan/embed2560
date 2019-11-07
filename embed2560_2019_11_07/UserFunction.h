// UserFunction.h
// мои функции, вынесенные за MAIN

#pragma once
#include "Main.h"

extern Adafruit_SSD1306 display;
extern OneWire  ds;
extern DataDevice DEVICE;
extern Mudbus Mb;

extern IPAddress ip;
extern uint8_t mac[];
extern IPAddress gateway;
extern IPAddress subnet;

// инициализация пинов
void initPIN (void);
// заставка на дисплее
void firstScreen (void);
// упаковка данных
void packDeviceData (unsigned int msONEWIRE);
// распаковка данных
void unpackDeviceData (void);
// отображение на дисплее
// версия без задержки
void screen (void);
// версия с задержкой ms
void screen2 (unsigned int msTask);



// получить статус запроса по MODBUS TCP
int getStatusModbusLink (void);

// получить время цикла текущее
unsigned long getTimeCycleAverageNcycles (void);
unsigned long getTimeCycleAverage2 (void);

// передача аналоговых параметров в регистры MODBUS
void sendAnalogToModbusRegister (void);