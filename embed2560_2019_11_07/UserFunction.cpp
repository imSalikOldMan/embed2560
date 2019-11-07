// 2019 апрель 02
// 
// 

#include "UserFunction.h"


// упаковка данных в структуру
void packDeviceData (unsigned int msONEWIRE) {
	static unsigned long previosMillis;
	// опрос датчика DS18b20 через каждые n милисекунд, чтобы не греть датчик
	if (millis ( ) - previosMillis > msONEWIRE) {
		previosMillis = millis ( );
		// получить температуру в структуру DeviceDATA
		DEVICE.Temperature1 = getTemperature (ds);				
	}
	// получить время цикла в структуру DeviceDATA
	DEVICE.timeCycle = getTimeCycleAverageNcycles ( );
	// получить статус запроса в структуру DeviceDATA
	DEVICE.statusModbusLink = getStatusModbusLink ( );
	// получить входные команды из таблицы MODBUS
	DEVICE.RELAY_0_ON = !Mb.C[0];
	DEVICE.RELAY_1_ON = !Mb.C[1];
	DEVICE.RELAY_2_ON = !Mb.C[2];
	DEVICE.RELAY_3_ON = !Mb.C[3];
}

// распаковка данных
void unpackDeviceData (void) {
	// из структуры данных
	// передаем температуры в модбас регистры (float TO 2word)
	sendAnalogToModbusRegister ( );
	// подача команды на выходы из структуры DeviceDATA
	digitalWrite (PIN_RELAY_0, DEVICE.RELAY_0_ON);
	digitalWrite (PIN_RELAY_1, DEVICE.RELAY_1_ON);
	digitalWrite (PIN_RELAY_2, DEVICE.RELAY_2_ON);
	digitalWrite (PIN_RELAY_3, DEVICE.RELAY_3_ON);
	// запись в таблицу сигнализации MODBUS из структуры DeviceDATA
	Mb.I[0] = !DEVICE.RELAY_0_ON;
	Mb.I[1] = !DEVICE.RELAY_1_ON;
	Mb.I[2] = !DEVICE.RELAY_2_ON;
	Mb.I[3] = !DEVICE.RELAY_3_ON;
}

// отображение на дисплее
// версия без задержки
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

	// моя отображалка текущего IP
	display.println (Ethernet.localIP ( ));

	display.display ( );

}
// версия с задержкой
void screen2 (unsigned int msTask) {
	String sMsgOFF = "OFF  ";
	String sMsgON  = "ON   ";
	static unsigned long previosMillis;
	// если время не прошло - возврат
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

	// моя отображалка текущего IP
	// + проверка изменения адресса IP, если да переконфигурирование
	// TODO неправильно, что находиться здесь
	if (Ethernet.localIP ( ) == ip) { display.println (Ethernet.localIP ( )); }
		else {	display.println ("error IP");
				delay (3000);
				Ethernet.begin (mac, ip, gateway, subnet);
				delay (3000);
		}

	display.display ( );

	Serial.println ("test serial");
}

// заставка на дисплее
void firstScreen (void) {
	
	display.clearDisplay ( );
	display.setTextSize (1);
	display.setTextColor (WHITE);
	display.setCursor (0, 0);
	display.println ("Modbus     TCP CONFIG");	// максимально в строку 21 символ
	display.println ("IP:      192.168.1.25");	// максимально в строку 21 символ
	display.println ("PORT:             502");	// максимально в строку 21 символ
	display.println ("MAC DE:AD:BE:EF:FE:ED");	// максимально в строку 21 символ
	display.display ( );

}

// инициализация пинов
void initPIN (void){
	// иницализация пинов для реле, уровень HIGH выключает, LOW включает
	pinMode (PIN_RELAY_0, OUTPUT);
	pinMode (PIN_RELAY_1, OUTPUT);
	pinMode (PIN_RELAY_2, OUTPUT);
	pinMode (PIN_RELAY_3, OUTPUT);
	digitalWrite (PIN_RELAY_0, HIGH);
	digitalWrite (PIN_RELAY_1, HIGH);
	digitalWrite (PIN_RELAY_2, HIGH);
	digitalWrite (PIN_RELAY_3, HIGH);

}

// получить статус запроса по MODBUS TCP
int getStatusModbusLink (void) {

	int STATUS;						// пересмотреть тип, int еще и знаковый, достаточно bool
	static unsigned int i;			// была ошибка, был не static. а как же это работало?
	unsigned int nCycNoReq = 2000;	// скока циклов отсутвует запрос

	if (ZAPROS)							{ i = 0;			STATUS = 1; }	// если был запрос сброс счетчика, статус КОННЕКТ
	if (!ZAPROS && (i >= nCycNoReq))	{ i = nCycNoReq;	STATUS = 0; }	// если количество отсутствующих запросов больше nCycNoReq циклов то - ДИСКОНЕКТ
	if (!ZAPROS && (i < nCycNoReq))		{ i = i + 1;		STATUS = 1; }	// нет запроса меньше nCycNoReq, статус КОННЕКТ , i++

	return STATUS;


}

// получить время цикла текущее
unsigned long getTimeCycleAverageNcycles (void) {
	// вернуться еще раз, сохранение переменной после вызова функции
	// имеет значение, что ,были объявлены тут без static
	static unsigned long	previosMillis;			// Переменная для хранения точки отсчета
	static unsigned	long	SUMtimeCycleAverage;
	static unsigned long	arrayTimeCycle[NUMBER_OF_CYCLE_AVERAGE];	// это #DEFINE

	unsigned long			timeCycleCurrent;		// Переменная для хранения точки отсчета
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
	static unsigned long	previosMillis;			// Переменная для хранения точки отсчета
	static unsigned long    timeCyclePREV;
	unsigned long			timeCycle;				// Переменная для хранения точки отсчета
	unsigned long			timeAverage;
	
	timeCycle = (millis ( ) - previosMillis);
	previosMillis = millis ( );
	
	timeAverage = (timeCycle + timeCyclePREV) / 2;
	timeCyclePREV = timeCycle;
	return timeAverage;

}


// передача аналоговых параметров в регистры MODBUS
void sendAnalogToModbusRegister (void) {

	uint16_t mbTemp[2];
	float2modbus (DEVICE.Temperature1, &mbTemp[0]);
	Mb.IR[0] = mbTemp[0];
	Mb.IR[1] = mbTemp[1];

}



