#pragma once

// структура данных для организации данных в устройстве
struct DataDevice {
	float			Temperature1;			// температура канал 1
	float			Temperature2;			// температура канал 2
	float			Temperature3;			// температура канал 3
	boolean			RELAY_0_ON;				// команда на включение реле
	boolean			RELAY_1_ON;				// команда на включение реле
	boolean			RELAY_2_ON;				// команда на включение реле
	boolean			RELAY_3_ON;				// команда на включение реле
	unsigned long	timeCycle;				// время цикла программы
	unsigned int	statusModbusLink;		// наличие запроса по TCP Modbus

};