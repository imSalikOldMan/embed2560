// UserFunction.h
// ��� �������, ���������� �� MAIN

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

// ������������� �����
void initPIN (void);
// �������� �� �������
void firstScreen (void);
// �������� ������
void packDeviceData (unsigned int msONEWIRE);
// ���������� ������
void unpackDeviceData (void);
// ����������� �� �������
// ������ ��� ��������
void screen (void);
// ������ � ��������� ms
void screen2 (unsigned int msTask);



// �������� ������ ������� �� MODBUS TCP
int getStatusModbusLink (void);

// �������� ����� ����� �������
unsigned long getTimeCycleAverageNcycles (void);
unsigned long getTimeCycleAverage2 (void);

// �������� ���������� ���������� � �������� MODBUS
void sendAnalogToModbusRegister (void);