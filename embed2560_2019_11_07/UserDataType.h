#pragma once

// ��������� ������ ��� ����������� ������ � ����������
struct DataDevice {
	float			Temperature1;			// ����������� ����� 1
	float			Temperature2;			// ����������� ����� 2
	float			Temperature3;			// ����������� ����� 3
	boolean			RELAY_0_ON;				// ������� �� ��������� ����
	boolean			RELAY_1_ON;				// ������� �� ��������� ����
	boolean			RELAY_2_ON;				// ������� �� ��������� ����
	boolean			RELAY_3_ON;				// ������� �� ��������� ����
	unsigned long	timeCycle;				// ����� ����� ���������
	unsigned int	statusModbusLink;		// ������� ������� �� TCP Modbus

};