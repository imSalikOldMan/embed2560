// float2modbus.h

#ifndef _FLOAT2MODBUS_h
#define _FLOAT2MODBUS_h


#include "arduino.h"			// нужно для сборки - uint16_t, memcpy
void float2modbus (float floatVal, uint16_t* uint16Array);


#endif

