// TODO �������� �������
// �������� ����������� � ������� ONEWIRE DS18b20
// ����� 1 ������� �� �����, ��� �������
// � ���������� - �������� CRC, ������, ������� �������, ��� �������

#include "getTemperature.h"

float getTemperature (OneWire &ds) {
	uint8_t data[12];
	float celsius;

	ds.reset ( );
	ds.write (0xCC);					// ����� �� ������ (SKIP ROM)
	ds.write (0x44,1);					// ��������� ��������������
	ds.reset ( );
	ds.write (0xCC);					// ����� �� ������ (SKIP ROM)
	ds.write (0xBE);					// ������ scratchpad ������� �� ����

	for (int i = 0; i < 9; i++) {		// ������ 8 ����. � ����� ��� ��� 8? ���������� ������ ����!
		data[i] = ds.read ( );
	}
	int16_t raw = (data[1] << 8) | data[0];		// int ��������� ������ �� ���� ����, ������ ������ �������� ������ 0.0625 �������
	celsius = (float)raw / 16.0;				// ���������� int �� float � ����� �� 16, ��� ����� ��������� �� 0.0625, ������ ���
	return celsius;								// 1 ������ �� 16 ����� 0.0625

}
