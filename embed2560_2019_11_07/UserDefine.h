#pragma once


#define OLED_RESET			4				// совпадает с реле, ПРОВЕРИТЬ!!!
#define ZAPROS				Mb.zapros		// наличие запроса от ПЛК

// пины выходы управления реле
#define PIN_RELAY_0			2
#define PIN_RELAY_1			4
#define PIN_RELAY_2			6
#define PIN_RELAY_3			8
// датчик температуры
#define PIN_DS18b20			3
// прочие настройки
#define ms_CALL_DISPLAY				1000
#define ms_CALL_DS18b20				1000
#define NUMBER_OF_CYCLE_AVERAGE		5
