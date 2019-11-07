// TODO развитие проекта
// получить температуру с датчика ONEWIRE DS18b20
// опрос 1 датчика на линии, без адресов
// в разработке - контроль CRC, ошибок, наличие датчика, тип датчика

#include "getTemperature.h"

float getTemperature (OneWire &ds) {
	uint8_t data[12];
	float celsius;

	ds.reset ( );
	ds.write (0xCC);					// пофиг на адреса (SKIP ROM)
	ds.write (0x44,1);					// запускаем преобразование
	ds.reset ( );
	ds.write (0xCC);					// пофиг на адреса (SKIP ROM)
	ds.write (0xBE);					// читаем scratchpad датчика на пине

	for (int i = 0; i < 9; i++) {		// Читаем 8 байт. А зачем нам все 8? Достаточно первых двух!
		data[i] = ds.read ( );
	}
	int16_t raw = (data[1] << 8) | data[0];		// int кодировка сотоит из двух байт, причем каждая единичка значит 0.0625 градуса
	celsius = (float)raw / 16.0;				// Превращаем int во float и делим на 16, что равно умножению на 0.0625, потому что
	return celsius;								// 1 делить на 16 равно 0.0625

}
