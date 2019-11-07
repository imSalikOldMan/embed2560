#pragma once


// взял за правило все неосновное выносить в Main.h
// в дальнейшем можно еще разделить на UserData.h UserDefine.h

#include "arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <avr/wdt.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Ethernet2.h>					// Used for Ethernet w5500
#include "Mudbus.h"
#include <OneWire.h>

#include "getTemperature.h"
#include "float2modbus.h"

#include "UserDefine.h"
#include "UserDataType.h"
#include "UserFunction.h"

