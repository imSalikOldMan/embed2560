// ������� ��� ������ ������ �������
// ������� �������� ������� � �������, ������� ����
// ������� �������� ���������� �������� � �������� ���
// ���������� ��� header � ����� Header Files
// ���������� ��� cpp � ����� Source Files
// ���������� board.txt � ������� ��� ��� Add Local Board.txt
// ������� ���������� ino
// ��������� ��� ��������

/*
 Name:		Embed2560_2019_11_07.ino
 Created:	07.11.2019 
 Author:	salik
*/

/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

Pick one up today in the adafruit shop!
------> http://www.adafruit.com/category/63_98

This example is for a 128x32 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

// ��� ������� mudbus.h ��� ������ ���������� �� ethernet2 (W5500)
// ������� ���������� ZAPROS � ���������� Mudbus
// Mb.C[] ���������� ����, Mb.I[] ������������ ����, Mb.IR[] �������� �������� ��������


#include "Main.h"

Adafruit_SSD1306 display (OLED_RESET);		// ��������� ������ SSD1306

//Function codes 1(read coils), 3(read registers), 5(write coil), 6(write register)
//signed int Mb.R[0 to 125] and bool Mb.C[0 to 128] MB_N_R MB_N_C
//Port 502 (defined in Mudbus.h) MB_PORT
//Avoid pins 4,10,11,12,13 when using ethernet shield
//bool C  [100]   [C]oil						R/W			01,05,15
//bool I  [100]   [I]nput						R			02
//int  IR [64]	  [I]nput[R]egistr				R			04
//int  R  [64]	  [R]egistr(Holding Registr)    R/W			03,06,16
Mudbus Mb;									// ��������� ������ mudbus

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
// http://www.pjrc.com/teensy/td_libs_OneWire.html
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library
// ���������� ������ 1wire ������������
OneWire  ds (PIN_DS18b20);							// on pin D3 (a 4.7K resistor is necessary)

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};					// ���  ��� byte, �������� uint8_t
IPAddress ip (192, 168, 1, 25);
IPAddress gateway (192, 168, 1, 1);
IPAddress subnet (255, 255, 255, 0);

// ��� ����������
DataDevice DEVICE;


void setup ( ) {
	// ��������� �������
	wdt_disable ( );
	// ������������ �����
	initPIN ( );
	Serial.begin (9600);
	// initialize the oled display device 
	display.begin (SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
	// �������������� �������� �� ������
	firstScreen ( );
	delay (5000);
	// initialize the ethernet device 
	Ethernet.begin (mac, ip, gateway, subnet);
	// �������� ������� 1 ��� (�� �����������)
	wdt_enable (WDTO_1S);
}

void loop ( ) {
	// START
	// ����� MODBUS
	Mb.Run ( );

	// ��������
	packDeviceData (ms_CALL_DS18b20);

	// ������ � ���������
	// �� �������, � ������ ������������� ��������� ������

	// ����������
	unpackDeviceData ( );

	// �����������
	screen2 (ms_CALL_DISPLAY);

	// END
	// ���������� ���� ������� ������� �� Modbus
	ZAPROS = false;
	// ���������� �������
	wdt_reset ( );
}



// �������
// ������� ���� ������������
// - ������ ������, ��������� ������ ��� ���������� ��� ��������
// - ��������� ������� �� �������� ( ����� ����������� ������ � �����) �����, �� ������ ������������ RASPBERRY !!!!!!!!!!!!!!
// - �������� watchdog																						   !!!!!!!!!!!!!			������ ��� ������������ ��� ������
// - ��� ������


// �������� �������� ��������
// ���� ��� ��� ������� ���������� �������, �������� � �� ����������� ��������
// ���� ��� ������ ��� ����������� ��� ������