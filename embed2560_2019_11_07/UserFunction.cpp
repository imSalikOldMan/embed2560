// 2019 ������ 02
// 
// 

#include "UserFunction.h"


// �������� ������ � ���������
void packDeviceData (unsigned int msONEWIRE) {
	static unsigned long previosMillis;
	// ����� ������� DS18b20 ����� ������ n ����������, ����� �� ����� ������
	if (millis ( ) - previosMillis > msONEWIRE) {
		previosMillis = millis ( );
		// �������� ����������� � ��������� DeviceDATA
		DEVICE.Temperature1 = getTemperature (ds);				
	}
	// �������� ����� ����� � ��������� DeviceDATA
	DEVICE.timeCycle = getTimeCycleAverageNcycles ( );
	// �������� ������ ������� � ��������� DeviceDATA
	DEVICE.statusModbusLink = getStatusModbusLink ( );
	// �������� ������� ������� �� ������� MODBUS
	DEVICE.RELAY_0_ON = !Mb.C[0];
	DEVICE.RELAY_1_ON = !Mb.C[1];
	DEVICE.RELAY_2_ON = !Mb.C[2];
	DEVICE.RELAY_3_ON = !Mb.C[3];
}

// ���������� ������
void unpackDeviceData (void) {
	// �� ��������� ������
	// �������� ����������� � ������ �������� (float TO 2word)
	sendAnalogToModbusRegister ( );
	// ������ ������� �� ������ �� ��������� DeviceDATA
	digitalWrite (PIN_RELAY_0, DEVICE.RELAY_0_ON);
	digitalWrite (PIN_RELAY_1, DEVICE.RELAY_1_ON);
	digitalWrite (PIN_RELAY_2, DEVICE.RELAY_2_ON);
	digitalWrite (PIN_RELAY_3, DEVICE.RELAY_3_ON);
	// ������ � ������� ������������ MODBUS �� ��������� DeviceDATA
	Mb.I[0] = !DEVICE.RELAY_0_ON;
	Mb.I[1] = !DEVICE.RELAY_1_ON;
	Mb.I[2] = !DEVICE.RELAY_2_ON;
	Mb.I[3] = !DEVICE.RELAY_3_ON;
}

// ����������� �� �������
// ������ ��� ��������
void screen (void) {

	display.clearDisplay ( );
	display.setTextSize (1);
	display.setCursor (0, 0);
	display.print ("connect: ");
	if (DEVICE.statusModbusLink == 0) { display.invertDisplay (1); display.println ("NO"); }
	if (DEVICE.statusModbusLink == 1) { display.invertDisplay (0); display.println ("YES"); }

	display.print ("cycle: ");
	display.print (DEVICE.timeCycle);
	display.println (" ms");

	display.setTextSize (1);
	display.print ("temp ");
	display.println (DEVICE.Temperature1);

	/*
	if (DEVICE.RELAY_0_ON) display.print(sMsgOFF);
		else display.print(sMsgON);
	if (DEVICE.RELAY_1_ON) display.print(sMsgOFF);
		else display.print(sMsgON);
	if (DEVICE.RELAY_2_ON) display.print(sMsgOFF);
		else display.print(sMsgON);
	if (DEVICE.RELAY_3_ON) display.print(sMsgOFF);
		else display.println(sMsgON);
	*/

	// ��� ����������� �������� IP
	display.println (Ethernet.localIP ( ));

	display.display ( );

}
// ������ � ���������
void screen2 (unsigned int msTask) {
	String sMsgOFF = "OFF  ";
	String sMsgON  = "ON   ";
	static unsigned long previosMillis;
	// ���� ����� �� ������ - �������
	if (millis ( ) - previosMillis < msTask) { return; }
	previosMillis = millis ( );

	display.clearDisplay ( );
	display.setTextSize (1);
	display.setCursor (0, 0);
	display.print ("connect: ");
	if (DEVICE.statusModbusLink == 0) { display.invertDisplay (1); display.println ("NO"); }
	if (DEVICE.statusModbusLink == 1) { display.invertDisplay (0); display.println ("YES"); }

	display.print ("cycle: ");
	display.print (DEVICE.timeCycle);
	display.println (" ms");

	display.setTextSize (1);
	display.print ("temp ");
	display.println (DEVICE.Temperature1);

	/*
	if (DEVICE.RELAY_0_ON) display.print(sMsgOFF);
		else display.print(sMsgON);
	if (DEVICE.RELAY_1_ON) display.print(sMsgOFF);
		else display.print(sMsgON);
	if (DEVICE.RELAY_2_ON) display.print(sMsgOFF);
		else display.print(sMsgON);
	if (DEVICE.RELAY_3_ON) display.print(sMsgOFF);
		else display.println(sMsgON);
	*/

	// ��� ����������� �������� IP
	// + �������� ��������� ������� IP, ���� �� ��������������������
	// TODO �����������, ��� ���������� �����
	if (Ethernet.localIP ( ) == ip) { display.println (Ethernet.localIP ( )); }
		else {	display.println ("error IP");
				delay (3000);
				Ethernet.begin (mac, ip, gateway, subnet);
				delay (3000);
		}

	display.display ( );

	Serial.println ("test serial");
}

// �������� �� �������
void firstScreen (void) {
	
	display.clearDisplay ( );
	display.setTextSize (1);
	display.setTextColor (WHITE);
	display.setCursor (0, 0);
	display.println ("Modbus     TCP CONFIG");	// ����������� � ������ 21 ������
	display.println ("IP:      192.168.1.25");	// ����������� � ������ 21 ������
	display.println ("PORT:             502");	// ����������� � ������ 21 ������
	display.println ("MAC DE:AD:BE:EF:FE:ED");	// ����������� � ������ 21 ������
	display.display ( );

}

// ������������� �����
void initPIN (void){
	// ������������ ����� ��� ����, ������� HIGH ���������, LOW ��������
	pinMode (PIN_RELAY_0, OUTPUT);
	pinMode (PIN_RELAY_1, OUTPUT);
	pinMode (PIN_RELAY_2, OUTPUT);
	pinMode (PIN_RELAY_3, OUTPUT);
	digitalWrite (PIN_RELAY_0, HIGH);
	digitalWrite (PIN_RELAY_1, HIGH);
	digitalWrite (PIN_RELAY_2, HIGH);
	digitalWrite (PIN_RELAY_3, HIGH);

}

// �������� ������ ������� �� MODBUS TCP
int getStatusModbusLink (void) {

	int STATUS;						// ������������ ���, int ��� � ��������, ���������� bool
	static unsigned int i;			// ���� ������, ��� �� static. � ��� �� ��� ��������?
	unsigned int nCycNoReq = 2000;	// ����� ������ ��������� ������

	if (ZAPROS)							{ i = 0;			STATUS = 1; }	// ���� ��� ������ ����� ��������, ������ �������
	if (!ZAPROS && (i >= nCycNoReq))	{ i = nCycNoReq;	STATUS = 0; }	// ���� ���������� ������������� �������� ������ nCycNoReq ������ �� - ���������
	if (!ZAPROS && (i < nCycNoReq))		{ i = i + 1;		STATUS = 1; }	// ��� ������� ������ nCycNoReq, ������ ������� , i++

	return STATUS;


}

// �������� ����� ����� �������
unsigned long getTimeCycleAverageNcycles (void) {
	// ��������� ��� ���, ���������� ���������� ����� ������ �������
	// ����� ��������, ��� ,���� ��������� ��� ��� static
	static unsigned long	previosMillis;			// ���������� ��� �������� ����� �������
	static unsigned	long	SUMtimeCycleAverage;
	static unsigned long	arrayTimeCycle[NUMBER_OF_CYCLE_AVERAGE];	// ��� #DEFINE

	unsigned long			timeCycleCurrent;		// ���������� ��� �������� ����� �������
	unsigned long			timeCycleAverage;
	int i;

	timeCycleCurrent = (millis ( ) - previosMillis);
	previosMillis = millis ( );

	SUMtimeCycleAverage = SUMtimeCycleAverage - arrayTimeCycle[i];
	SUMtimeCycleAverage = SUMtimeCycleAverage + timeCycleCurrent;
	arrayTimeCycle[i] = timeCycleCurrent;
	i++;
	if (i == NUMBER_OF_CYCLE_AVERAGE) { i = 0; }
	timeCycleAverage = (SUMtimeCycleAverage / NUMBER_OF_CYCLE_AVERAGE);

	return timeCycleAverage;

}

unsigned long getTimeCycleAverage2 (void) {
	static unsigned long	previosMillis;			// ���������� ��� �������� ����� �������
	static unsigned long    timeCyclePREV;
	unsigned long			timeCycle;				// ���������� ��� �������� ����� �������
	unsigned long			timeAverage;
	
	timeCycle = (millis ( ) - previosMillis);
	previosMillis = millis ( );
	
	timeAverage = (timeCycle + timeCyclePREV) / 2;
	timeCyclePREV = timeCycle;
	return timeAverage;

}


// �������� ���������� ���������� � �������� MODBUS
void sendAnalogToModbusRegister (void) {

	uint16_t mbTemp[2];
	float2modbus (DEVICE.Temperature1, &mbTemp[0]);
	Mb.IR[0] = mbTemp[0];
	Mb.IR[1] = mbTemp[1];

}



