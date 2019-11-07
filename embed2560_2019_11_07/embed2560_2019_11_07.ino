// создано как пустой проект ардуино
// указать названия проекта и решения, указать путь
// выбрать создание отдельного каталога и создание гит
// скопировал все header в папку Header Files
// скопировал все cpp в папку Source Files
// скопировал board.txt и добавил его как Add Local Board.txt
// спастил содержимое ino
// собралось без вопросов

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

// был поменен mudbus.h для замены библиотеки на ethernet2 (W5500)
// введена переменная ZAPROS в библиотеку Mudbus
// Mb.C[] управление реле, Mb.I[] сигнализация реле, Mb.IR[] значение аналогов передача


#include "Main.h"

Adafruit_SSD1306 display (OLED_RESET);		// экземпляр класса SSD1306

//Function codes 1(read coils), 3(read registers), 5(write coil), 6(write register)
//signed int Mb.R[0 to 125] and bool Mb.C[0 to 128] MB_N_R MB_N_C
//Port 502 (defined in Mudbus.h) MB_PORT
//Avoid pins 4,10,11,12,13 when using ethernet shield
//bool C  [100]   [C]oil						R/W			01,05,15
//bool I  [100]   [I]nput						R			02
//int  IR [64]	  [I]nput[R]egistr				R			04
//int  R  [64]	  [R]egistr(Holding Registr)    R/W			03,06,16
Mudbus Mb;									// экземпляр класса mudbus

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
// http://www.pjrc.com/teensy/td_libs_OneWire.html
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library
// экземпляры класса 1wire термодатчика
OneWire  ds (PIN_DS18b20);							// on pin D3 (a 4.7K resistor is necessary)

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};					// был  тип byte, поставил uint8_t
IPAddress ip (192, 168, 1, 25);
IPAddress gateway (192, 168, 1, 1);
IPAddress subnet (255, 255, 255, 0);

// мои переменные
DataDevice DEVICE;


void setup ( ) {
	// отключаем ватчдог
	wdt_disable ( );
	// иницализация пинов
	initPIN ( );
	Serial.begin (9600);
	// initialize the oled display device 
	display.begin (SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
	// первоначальная заставка на экране
	firstScreen ( );
	delay (5000);
	// initialize the ethernet device 
	Ethernet.begin (mac, ip, gateway, subnet);
	// включаем ватчдог 1 сек (по результатам)
	wdt_enable (WDTO_1S);
}

void loop ( ) {
	// START
	// вызов MODBUS
	Mb.Run ( );

	// УПАКОВКА
	packDeviceData (ms_CALL_DS18b20);

	// ЛОГИКА и ОБРАБОТКА
	// на будущее, в случае необходимости обработки данных

	// РАСПАКОВКА
	unpackDeviceData ( );

	// ОТОБРАЖЕНИЕ
	screen2 (ms_CALL_DISPLAY);

	// END
	// сбрасываем флаг наличия запроса по Modbus
	ZAPROS = false;
	// сбрасываем ватчдог
	wdt_reset ( );
}



// ХОТЕЛКИ
// принять байт конфигурации
// - логика работы, запомнить выхода при дисконекте или сбросить
// - заморозка выходов на заданное ( иметь возможность менять и время) время, на момент перезагрузки RASPBERRY !!!!!!!!!!!!!!
// - добавить watchdog																						   !!!!!!!!!!!!!			подвис при отсоединении усб шнурка
// - еще логика


// фиксация изменеия карандаш
// один раз при жестком отрубилове питания, подвисла и не запускалась сетевуха
// один раз подвис при отсоединеии усб шнурка