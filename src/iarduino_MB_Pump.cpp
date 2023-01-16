#include "iarduino_MB_Pump.h"		//	╔═══════════════════════╗
									//	║ РЕГИСТР ДИАГНОСТИКИ   ║
									//	╠═══════════╤═══════════╣
									//	║ --------  │ --------  ║ Флаги не используются.
									//	╚═══════════╧═══════════╝
									//	╔═══════════════╦═══════╗
									//	║ РЕГИСТРЫ «DO» ║ Биты: ║
									//	╠═══════════════╬═══════╣
#define DO_HL_DATA		0			//	║ DO_HL_DATA    ║   0   ║ Включение светодиода обнаружения устройства.
#define DO_WDT			1			//	║ DO_WDT        ║   0   ║ Включение сторожевого таймера насосов.
#define DO_PUMP_A		2			//	║ DO_PUMP_A     ║   0   ║ Включение насоса A.
#define DO_PUMP_B		3			//	║ DO_PUMP_B     ║   0   ║ Включение насоса B.
#define DO_PUMP_C		4			//	║ DO_PUMP_C     ║   0   ║ Включение насоса C.
#define DO_PUMP_12V_A	5			//	║ DO_PUMP_12V_A ║   1   ║ Включение коррекции питания для насоса A до 12V.
#define DO_PUMP_12V_B	6			//	║ DO_PUMP_12V_B ║   1   ║ Включение коррекции питания для насоса B до 12V.
#define DO_PUMP_12V_C	7			//	║ DO_PUMP_12V_C ║   1   ║ Включение коррекции питания для насоса C до 12V.
									//	╟───────────────╫───────╢
//						0x0100		//	║ DO_CHANGE_ID  ║   0   ║ Флаг устанавливается самостоятельно после подтверждения смены адреса на шине.
//						0x0101		//	║ DO_BAN_ID 1   ║   0   ║ Бит  запрещает изменять или устанавливать адрес 1.      Только для режима смены адреса на случайное симло.
//						...			//	║ DO_BAN_ID ... ║   0   ║ Биты запрещают изменять или устанавливать адреса 2-246. Только для режима смены адреса на случайное симло.
//						0x01F7		//	║ DO_BAN_ID 247 ║   0   ║ Бит  запрещает изменять или устанавливать адрес 147.    Только для режима смены адреса на случайное симло.
									//	╚═══════════════╩═══════╝
									//	╔═══════════════╦═══════╗
									//	║ РЕГИСТРЫ «DI» ║ Биты: ║
									//	╠═══════════════╬═══════╣
#define DI_MIN_A		0			//	║ DI_MIN_A      ║   0   ║ Флаг нижнего  датчика уровня жидкости A.
#define DI_MID_A		1			//	║ DI_MID_A      ║   0   ║ Флаг среднего датчика уровня жидкости A.
#define DI_MIN_B		2			//	║ DI_MIN_B      ║   0   ║ Флаг нижнего  датчика уровня жидкости B.
#define DI_MID_B		3			//	║ DI_MID_B      ║   0   ║ Флаг среднего датчика уровня жидкости B.
#define DI_MIN_C		4			//	║ DI_MIN_C      ║   0   ║ Флаг нижнего  датчика уровня жидкости C.
#define DI_MID_C		5			//	║ DI_MID_C      ║   0   ║ Флаг среднего датчика уровня жидкости C.
									//	╚═══════════════╩═══════╝
									//	╔═══════════════╦═══════╗
									//	║ РЕГИСТРЫ «AO» ║Данные:║
									//	╠═══════════════╬═══════╣
#define AO_WDT			0			//	║ AO_WDT        ║ 10000 ║ Сторожевой таймер насосов, хранит мс до отключения насосов.
#define AO_FREQ_A		1			//	║ AO_FREQ_A     ║  1000 ║ Частота ШИМ насоса A, от 1Гц до 10'000Гц.
#define AO_FREQ_B		2			//	║ AO_FREQ_B     ║  1000 ║ Частота ШИМ насоса B, от 1Гц до 10'000Гц.
#define AO_FREQ_C		3			//	║ AO_FREQ_C     ║  1000 ║ Частота ШИМ насоса C, от 1Гц до 10'000Гц.
#define AO_PWM_A		4			//	║ AO_PWM_A      ║   255 ║ Коэффициент заполнения ШИМ включённого насоса A, от 1 до 255.
#define AO_PWM_B		5			//	║ AO_PWM_B      ║   255 ║ Коэффициент заполнения ШИМ включённого насоса B, от 1 до 255.
#define AO_PWM_C		6			//	║ AO_PWM_C      ║   255 ║ Коэффициент заполнения ШИМ включённого насоса C, от 1 до 255.
#define AO_TMR_A		7			//	║ AO_TMR_A      ║     0 ║ Время до отключения насоса A в секундах. Установка включает насос, сброс отключает, от 0,0 до 6'553,5 сек.
#define AO_TMR_B		8			//	║ AO_TMR_B      ║     0 ║ Время до отключения насоса B в секундах. Установка включает насос, сброс отключает, от 0,0 до 6'553,5 сек.
#define AO_TMR_C		9			//	║ AO_TMR_C      ║     0 ║ Время до отключения насоса C в секундах. Установка включает насос, сброс отключает, от 0,0 до 6'553,5 сек.
									//	╟───────────────╫───────╢
//						0x0100		//	║ AO_ACK_ID     ║   0   ║ Запись значения 0xF0A5 подтверждает смену адреса на шине. При чтении всегда возвращает 0.
#define AO_ACK_SPEED	0x0101		//	║ AO_ACK_SPEED  ║   0   ║ Запись значения 0xF0A5 подтверждает смену скорости  шины. При чтении всегда возвращает 0.
#define AO_ACK_TYPE		0x0102		//	║ AO_ACK_TYPE   ║   0   ║ Запись значения 0xF0A5 подтверждает смену типа протокола. При чтении всегда возвращает 0.
//						0x0110		//	║ AO_SET_ID     ║  xxID ║ Запись приводит к смене адреса на шине, xx=00 до отключения питания, xx=FF с сохранеием в ПЗУ. Требуется подтверждение. При чтении xx=случайное число.
#define AO_SET_SPEED	0x0111		//	║ AO_SET_SPEED  ║  xxSP ║ Запись приводит к смене скорости  шины, xx=00 до отключения питания, xx=FF с сохранеием в ПЗУ. Требуется подтверждение. При чтении xx=случайное число.
#define AO_SET_TYPE		0x0112		//	║ AO_SET_TYPE   ║  xxTP ║ Запись приводит к смене типа протокола, xx=00 до отключения питания, xx=FF с сохранеием в ПЗУ. Требуется подтверждение. При чтении xx=случайное число.
#define AO_USER_DATA	0x0120		//	║ AO_USER_DATA  ║   0   ║ Пользовательское число хранится в Flash памяти модуля.
									//	╚═══════════════╩═══════╝
									//	╔═══════════════╦═══════╗
									//	║ РЕГИСТРЫ «AI» ║Данные:║
									//	╠═══════════════╬═══════╣
#define AI_VIN			0			//	║ AI_VIN        ║   0   ║ Напряжение питания шины RS-485 в мВ.
									//	╚═══════════════╩═══════╝
																															//
//		КОНСТРУКТОР КЛАССА:																									//
		iarduino_MB_Pump::iarduino_MB_Pump(ModbusClient &obj){objModbus = &obj;}											//	&obj - ссылка на объект для работы по протоколу Modbus.
																															//
//		ФУНКЦИЯ ИНИЦИАЛИЗАЦИИ НАСОСОВ:																						//	Возвращает результат инициализации: true-успех / false-провал.
bool	iarduino_MB_Pump::begin(uint8_t id){																				//	id - адрес модуля на шине.
			objModbus->codeError=ERROR_GATEWAY_NO_DEVICE;																	//	Ошибкой выполнения данной функции может быть только отсутствие устройства.
		//	Самостоятельный поиск id устройства:																			//
			if( id==0 )						{ if( objModbus->findID(DEF_MODEL_PUMP3) ){ id=objModbus->read(); }}			//	Если адрес не указан, ищем адрес первого устройства с совпавшим идентификатором.
			if( id==0 )						{ if( objModbus->findID(DEF_MODEL_PUMP2) ){ id=objModbus->read(); }}			//	Если адрес не указан, ищем адрес первого устройства с совпавшим идентификатором.
			if( id==0 )						{ if( objModbus->findID(DEF_MODEL_PUMP1) ){ id=objModbus->read(); }}			//	Если адрес не указан, ищем адрес первого устройства с совпавшим идентификатором.
		//	Проверяем устройство:																							//
			if( id==0 )						{ return false; }																//	Адрес устройства не указан и не найден.
			if( objModbus->getInfo(id)<15 )	{ return false; }																//	Устройство не найдено, или информация об устройстве неполная.
			if( objModbus->read()!=0x77 )	{ return false; }																//	Идентификатор линейки устройств не соответствует устройствам iArduino.
				objModbus->read();																							//	Индикатор пуска не проверяем (00=OFF, FF=ON).
			if( objModbus->read()!=id )		{ return false; }																//	Адрес полученный из информации об устройстве не совпадает с фактическим адресом устройства.
			uint8_t i=objModbus->read();																					//	Получаем идентификатор устройства.
			if( i==DEF_MODEL_PUMP1 )		{ sumPUMP=1; }else																//	Идентификатор устройства совпал с DEF_MODEL_PUMP1, значит у него 1 насос.
			if( i==DEF_MODEL_PUMP2 )		{ sumPUMP=2; }else																//	Идентификатор устройства совпал с DEF_MODEL_PUMP2, значит у него 2 насоса.
			if( i==DEF_MODEL_PUMP3 )		{ sumPUMP=3; }else{ return false; }												//	Идентификатор устройства совпал с DEF_MODEL_PUMP3, значит у него 3 насоса, иначе возвращаем ошибку.
		//	Устройство прошло проверку:																						//
			valID=id;																										//	Сохраняем адрес устройства.
			valVers=objModbus->read();																						//	Сохраняем версию прошивки устройства.
			return true;																									//
}																															//
																															//
//		ФУНКЦИЯ ПЕРЕЗАГРУЗКИ НАСОСОВ:																						//	Возвращает результат перезагрузки: true-успех / false-провал.
bool	iarduino_MB_Pump::reset(void){																						//	
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			return ( objModbus->diagnostic(valID,1,0xFF00)<0? false:true );													//	Выполняем команду диагностики, функция 0x0001 с очисткой журнала событий 0xFF00.
}																															//
																															//
//		ФУНКЦИЯ СМЕНЫ ID НАСОСОВ:																							//	Возвращает результат смены ID: true-успех / false-провал.
bool	iarduino_MB_Pump::changeID(uint8_t id){																				//	id - новый адрес модуля на шине (1-247).
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( !objModbus->changeID(valID, id) ){ return false; }															//	Меняем адрес устройства с valID на id.
			valID=id; return true;																							//	Сохраняем навый адрес и возвращаем флаг успеха.
}																															//
																															//
//		ФУНКЦИЯ СМЕНЫ СКОРОСТИ ПЕРЕДАЧИ ДАННЫХ:																				//	Возвращает результат смены скорости: true-успех / false-провал. Скорость необходимо подтвердить в течении 2 секунд.
bool	iarduino_MB_Pump::setSpeedMB(uint32_t s){																			//	s - скорость передачи данных (2400/4800/9600/19200/38400/57600/115200)
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( s!=2400 && s!=4800 && s!=9600 && s!=19200 && s!=38400 && s!=57600 && s!=115200 ){ return false; }			//	Проверяем значение скорости.
			return objModbus->holdingRegisterWrite(valID, AO_SET_SPEED, 0xFF00|(s/2400));									//	Записываем значение 0xFFXX в регистр "Holding Register" AO[AO_SET_SPEED]. Функция возвращает 1 при успехе, 0 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ ПОДТВЕРЖДЕНИЯ СКОРОСТИ ПЕРЕДАЧИ ДАННЫХ:																		//	Возвращает результат подтверждения скорости: true-успех / false-провал. 
bool	iarduino_MB_Pump::ackSpeedMB(void){																					//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( !objModbus->holdingRegisterWrite(valID, AO_ACK_SPEED, 0xF0A5) ){ return false; }							//	Записываем значение 0xF0A5 в регистр "Holding Register" AO[AO_ACK_SPEED]. Функция возвращает 1 при успехе, 0 при неудаче.
			delay(50);																										//	Ждём 50 мс, пока модуль не сохранит новую скорость в flash память.
			return true;																									//	Возвращаем флаг успеха.
}																															//
																															//
//		ФУНКЦИЯ СМЕНЫ ТИПА ПРОТОКОЛА MODBUS:																				//	Возвращает результат смены типа протокола Modbus: true-успех / false-провал. Тип необходимо подтвердить в течении 2 секунд.
bool	iarduino_MB_Pump::setTypeMB(uint8_t type){																			//	type - тип протокола Modbus (MODBUS_RTU/MODBUS_ASCII).
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( type!=MODBUS_RTU && type!=MODBUS_ASCII ){ return false; }													//	Проверяем значение типа протокола Modbus.
			return objModbus->holdingRegisterWrite(valID, AO_SET_TYPE, (type==MODBUS_ASCII?0xFFFF:0xFF00));					//	Записываем значение 0xFFXX в регистр "Holding Register" AO[AO_SET_TYPE]. Функция возвращает 1 при успехе, 0 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ ПОДТВЕРЖДЕНИЯ ТИПА ПРОТОКОЛА MODBUS:																		//	Возвращает результат подтверждения типа протокола Modbus: true-успех / false-провал. 
bool	iarduino_MB_Pump::ackTypeMB(void){																					//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( !objModbus->holdingRegisterWrite(valID, AO_ACK_TYPE, 0xF0A5) ){ return false; }								//	Записываем значение 0xF0A5 в регистр "Holding Register" AO[AO_ACK_TYPE]. Функция возвращает 1 при успехе, 0 при неудаче.
			delay(50);																										//	Ждём 50 мс, пока модуль не сохранит новый тип протокола Modbus в flash память.
			return true;																									//	Возвращаем флаг успеха.
}																															//
																															//
//		ФУНКЦИЯ ЗАПИСИ ПОЛЬЗОВАТЕЛЬСКОГО ЗНАЧЕНИЯ В FLASH ПАМЯТЬ МОДУЛЯ:													//	Возвращает результат записи: true-успех / false-провал.
bool	iarduino_MB_Pump::writeFlash(uint16_t data){																		//	data - целое беззнаковое число от 0 до 65535.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( !objModbus->holdingRegisterWrite(valID, AO_USER_DATA, data) ){ return false; }								//	Записываем значение data в регистр "Holding Register" AO[AO_USER_DATA]. Функция возвращает 1 при успехе, 0 при неудаче.
			delay(50);																										//	Ждём 50 мс, пока модуль не сохранит записанное значение в flash память.
			return true;																									//	Возвращаем флаг успеха.
}																															//
																															//
//		ФУНКЦИЯ ЧТЕНИЯ ПОЛЬЗОВАТЕЛЬСКОГО ЗНАЧЕНИЯ ИЗ FLASH ПАМЯТИ:															//	Возвращает прочитанное значение, или -1 при провале чтения.
int32_t	iarduino_MB_Pump::readFlash(void){																					//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			return objModbus->holdingRegisterRead(valID, AO_USER_DATA);														//	Читаем значение из регистра "Holding Register" AO[AO_USER_DATA]. Функция возвращает значение, или -1 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ ПОЛУЧЕНИЯ НАПРЯЖЕНИЯ ПИТАНИЯ:																				//	Возвращает напряжение питания в Вольтах, или -1 при провале чтения.
float	iarduino_MB_Pump::getPWR(void){																						//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1.0f; }												//	Ошибка синтаксиса, модуль не инициализирован.
			int32_t Vcc = objModbus->inputRegisterRead(valID, AI_VIN);														//	Читаем напряжение из регистра "Input Register" AI[AI_VIN]. Функция возвращает значение, или -1 при неудаче.
			if( Vcc<0 ){ return -1.0f; }else{ return ((float)Vcc)/1000.0f; }												//	Возвращаем -1 (ошибка), или напряжение в Вольтах.
}																															//
																															//
//		ФУНКЦИЯ ИЗМЕНЕНИЯ СОСТОЯНИЯ СВЕТОДИОДА ОБНАРУЖЕНИЯ УСТРОЙСТВА:														//	Возвращает результат изменения состояния светодиода: true-успех / false-провал.
bool	iarduino_MB_Pump::setIDLED(bool f){																					//	f - состояние светодиода (true/false)
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			return objModbus->coilWrite(valID, DO_HL_DATA, f);																//	Записываем f в регистр "Coil" DO[DO_HL_DATA].
}																															//
																															//
//		ФУНКЦИЯ ПОЛУЧЕНИЯ ФЛАГОВ ОШИБОК ИЗ РЕГИСТРА ДИАГНОСТИКИ:															//	Возвращает значение регистра диагностики, каждый из 16 бит которого является флагом ошибки, или -1 при провале чтения.
int32_t	iarduino_MB_Pump::getErr(void){																						//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			return objModbus->diagnostic(valID, 2);																			//	Выполняем команду диагностики, номер функции диагностики 0x0002 = получить регистр диагностики. Функция возвращает данные результата выполнения функции диагностики, или -1 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ СМЕНЫ ЧАСТОТЫ ШИМ НАСОСОВ:																					//	Возвращает результат применения частоты: true-успех / false-провал.
bool	iarduino_MB_Pump::setFrequency(uint8_t num, uint16_t a, uint16_t b, uint16_t c){									//	num - PUMP_X/PUMP_ALL, a,b,c - частота в Гц (1-10000).
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( num==PUMP_A && sumPUMP>=1 ){ return objModbus->holdingRegisterWrite(valID, AO_FREQ_A, a); }else				//	Устанавливаем частоту ШИМ насоса A записав значение a в регистр "Holding Register" AO[AO_FREQ_A]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_B && sumPUMP>=2 ){ return objModbus->holdingRegisterWrite(valID, AO_FREQ_B, a); }else				//	Устанавливаем частоту ШИМ насоса B записав значение a в регистр "Holding Register" AO[AO_FREQ_B]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_C && sumPUMP>=3 ){ return objModbus->holdingRegisterWrite(valID, AO_FREQ_C, a); }else				//	Устанавливаем частоту ШИМ насоса C записав значение a в регистр "Holding Register" AO[AO_FREQ_C]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_ALL             ){ objModbus->beginTransmission(valID, HOLDING_REGISTERS, AO_FREQ_A, sumPUMP);	//	Инициируем передачу данных в регистры "Holding Register" (AO), начиная с регистра AO[AO_FREQ_A], всего sumPUMP регистров.
			                                 if( sumPUMP>=1 ){ objModbus->write( a ); }										//	Ставим частоту a в очередь на запись.
			                                 if( sumPUMP>=2 ){ objModbus->write( b ); }										//	Ставим частоту b в очередь на запись.
			                                 if( sumPUMP>=3 ){ objModbus->write( c ); }										//	Ставим частоту c в очередь на запись.
			                                 return objModbus->endTransmission();											//	Выполняем запись. Функция возвращает 1 при успехе, 0 при неудаче.
			}																												//
			return false;																									//
}																															//
																															//
//		ФУНКЦИЯ ЧТЕНИЯ ЧАСТОТЫ ШИМ НАСОСА:																					//	Возвращает частоту в Гц (1-10000), или -1 при провале чтения частоты.
int32_t	iarduino_MB_Pump::getFrequency(uint8_t num){																		//	num - PUMP_X.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			if( num==PUMP_A && sumPUMP>=1 ){ return objModbus->holdingRegisterRead(valID, AO_FREQ_A); }else					//	Читаем частоту ШИМ насоса A из регистра "Holding Register" AO[AO_FREQ_A]. Функция возвращает прочитанное значение (0...65535), или -1 при неудаче.
			if( num==PUMP_B && sumPUMP>=2 ){ return objModbus->holdingRegisterRead(valID, AO_FREQ_B); }else					//	Читаем частоту ШИМ насоса B из регистра "Holding Register" AO[AO_FREQ_B]. Функция возвращает прочитанное значение (0...65535), или -1 при неудаче.
			if( num==PUMP_C && sumPUMP>=3 ){ return objModbus->holdingRegisterRead(valID, AO_FREQ_C); }						//	Читаем частоту ШИМ насоса C из регистра "Holding Register" AO[AO_FREQ_C]. Функция возвращает прочитанное значение (0...65535), или -1 при неудаче.
			return -1;																										//	Если запрошены состояния всех насосов, возвращаем ошибку. Данная операция не поддерживается.
}																															//
																															//
//		ФУНКЦИЯ СМЕНЫ МОЩНОСТИ НАСОСОВ (ШИМ):																				//	Возвращает результат применения мощности: true-успех / false-провал.
bool	iarduino_MB_Pump::setPower(uint8_t num, uint8_t a, uint8_t b, uint8_t c){											//	num - PUMP_X/PUMP_ALL, a,b,c - мощность (1-255).
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( num==PUMP_A && sumPUMP>=1 ){ return objModbus->holdingRegisterWrite(valID, AO_PWM_A, a); }else				//	Устанавливаем мощность насоса A записав ШИМ a в регистр "Holding Register" AO[AO_PWM_A]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_B && sumPUMP>=2 ){ return objModbus->holdingRegisterWrite(valID, AO_PWM_B, a); }else				//	Устанавливаем мощность насоса B записав ШИМ a в регистр "Holding Register" AO[AO_PWM_B]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_C && sumPUMP>=3 ){ return objModbus->holdingRegisterWrite(valID, AO_PWM_C, a); }else				//	Устанавливаем мощность насоса C записав ШИМ a в регистр "Holding Register" AO[AO_PWM_C]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_ALL             ){ objModbus->beginTransmission(valID, HOLDING_REGISTERS, AO_PWM_A, sumPUMP);		//	Инициируем передачу данных в регистры "Holding Register" (AO), начиная с регистра AO[AO_PWM_A], всего sumPUMP регистров.
			                                 if( sumPUMP>=1 ){ objModbus->write( a ); }										//	Ставим ШИМ a в очередь на запись.
			                                 if( sumPUMP>=2 ){ objModbus->write( b ); }										//	Ставим ШИМ b в очередь на запись.
			                                 if( sumPUMP>=3 ){ objModbus->write( c ); }										//	Ставим ШИМ c в очередь на запись.
			                                 return objModbus->endTransmission();											//	Выполняем запись. Функция возвращает 1 при успехе, 0 при неудаче.
			}																												//
			return false;																									//
}																															//
																															//
//		ФУНКЦИЯ ЧТЕНИЯ МОЩНОСТИ НАСОСА (ШИМ):																				//	Возвращает мощность насоса (1-255), или -1 при провале чтения мощности.
int16_t	iarduino_MB_Pump::getPower(uint8_t num){																			//	num - PUMP_X.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			if( num==PUMP_A && sumPUMP>=1 ){ return objModbus->holdingRegisterRead(valID, AO_PWM_A); }else					//	Читаем ШИМ насоса A из регистра "Holding Register" AO[AO_PWM_A]. Функция возвращает прочитанное значение (0...65535), или -1 при неудаче.
			if( num==PUMP_B && sumPUMP>=2 ){ return objModbus->holdingRegisterRead(valID, AO_PWM_B); }else					//	Читаем ШИМ насоса B из регистра "Holding Register" AO[AO_PWM_B]. Функция возвращает прочитанное значение (0...65535), или -1 при неудаче.
			if( num==PUMP_C && sumPUMP>=3 ){ return objModbus->holdingRegisterRead(valID, AO_PWM_C); }						//	Читаем ШИМ насоса C из регистра "Holding Register" AO[AO_PWM_C]. Функция возвращает прочитанное значение (0...65535), или -1 при неудаче.
			return -1;																										//	Если запрошены состояния всех насосов, возвращаем ошибку. Данная операция не поддерживается.
}																															//
																															//
//		ФУНКЦИЯ СМЕНЫ СОСТОЯНИЯ НАСОСОВ:																					//	Возвращает результат изменения состояний: true-успех / false-провал.
bool	iarduino_MB_Pump::setState(uint8_t num, bool a, bool b, bool c){													//	num - PUMP_X/PUMP_ALL, a,b,c - состояния true/false.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( num==PUMP_A && sumPUMP>=1 ){ return objModbus->coilWrite(valID, DO_PUMP_A, a); }else						//	Управляем насосом A записав значение a в регистр "Coil" DO[DO_PUMP_A]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_B && sumPUMP>=2 ){ return objModbus->coilWrite(valID, DO_PUMP_B, a); }else						//	Управляем насосом B записав значение a в регистр "Coil" DO[DO_PUMP_B]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_C && sumPUMP>=3 ){ return objModbus->coilWrite(valID, DO_PUMP_C, a); }else						//	Управляем насосом C записав значение a в регистр "Coil" DO[DO_PUMP_C]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_ALL             ){ objModbus->beginTransmission(valID, COILS, DO_PUMP_A, sumPUMP);				//	Инициируем передачу данных в регистры "Coil" (DO), начиная с регистра DO[DO_PUMP_A], всего sumPUMP регистров.
			                                 if( sumPUMP>=1 ){ objModbus->write( a ); }										//	Ставим бит a в очередь на запись.
			                                 if( sumPUMP>=2 ){ objModbus->write( b ); }										//	Ставим бит b в очередь на запись.
			                                 if( sumPUMP>=3 ){ objModbus->write( c ); }										//	Ставим бит c в очередь на запись.
			                                 return objModbus->endTransmission();											//	Выполняем запись. Функция возвращает 1 при успехе, 0 при неудаче.
			}																												//
			return false;																									//
}																															//
																															//
//		ФУНКЦИЯ ЧТЕНИЯ СОСТОЯНИЯ НАСОСОВ:																					//	Возвращает состояние насоса: true-успех / false-провал, байт битов если читаются все насосы, или -1 при провале чтения состояния насоса.
int8_t	iarduino_MB_Pump::getState(uint8_t num){																			//	num - PUMP_X/PUMP_ALL.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			if( num==PUMP_A && sumPUMP>=1 ){ return objModbus->coilRead(valID, DO_PUMP_A); }else							//	Читаем состояние насоса A из регистра "Coil" DO[DO_PUMP_A]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_B && sumPUMP>=2 ){ return objModbus->coilRead(valID, DO_PUMP_B); }else							//	Читаем состояние насоса B из регистра "Coil" DO[DO_PUMP_B]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_C && sumPUMP>=3 ){ return objModbus->coilRead(valID, DO_PUMP_C); }else							//	Читаем состояние насоса C из регистра "Coil" DO[DO_PUMP_C]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_ALL             ){																				//	Если запрошены состояния всех насосов...
				if( objModbus->requestFrom(valID, COILS, DO_PUMP_A, sumPUMP) != sumPUMP ){ return -1; }						//	Читаем регистры "Coils" начиная с регистра DO[DO_PUMP_A], всего sumPUMP регистров. Функция возвращает количество прочитанных значений, или 0 при неудаче.
				uint8_t i=0;																								//
				if( sumPUMP>=1 ){ i += (objModbus->read()?1:0); }															//	Устанавливаем 0 бит байта i, если насос A включён.
				if( sumPUMP>=2 ){ i += (objModbus->read()?2:0); }															//	Устанавливаем 1 бит байта i, если насос B включён.
				if( sumPUMP>=3 ){ i += (objModbus->read()?4:0); }															//	Устанавливаем 2 бит байта i, если насос C включён.
				return i;																									//
			}																												//
			return -1;																										//
}																															//
																															//
//		ФУНКЦИЯ ВКЛЮЧЕНИЯ НАСОСОВ НА ВРЕМЯ:																					//	Возвращает результат применения времени: true-успех / false-провал.
bool	iarduino_MB_Pump::setTimeOn(uint8_t num, float a, float b, float c){												//	num - PUMP_X/PUMP_ALL, a,b,c - время в секундах (0,0-6553,5).
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( a<0 || b<0 || c<0 || a>6553.5f || b>6553.5f || c>6553.5f){ return false; }									//	Ошибка синтаксиса, некорректное время работы насосов.
			if( num==PUMP_A && sumPUMP>=1 ){ return objModbus->holdingRegisterWrite(valID,AO_TMR_A,(uint16_t)(a*10)); }else	//	Включаем насос A записав время a*10 в регистр "Holding Register" AO[AO_TMR_A]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_B && sumPUMP>=2 ){ return objModbus->holdingRegisterWrite(valID,AO_TMR_B,(uint16_t)(a*10)); }else	//	Включаем насос B записав время a*10 в регистр "Holding Register" AO[AO_TMR_B]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_C && sumPUMP>=3 ){ return objModbus->holdingRegisterWrite(valID,AO_TMR_C,(uint16_t)(a*10)); }else	//	Включаем насос C записав время a*10 в регистр "Holding Register" AO[AO_TMR_C]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_ALL             ){ objModbus->beginTransmission(valID, HOLDING_REGISTERS, AO_TMR_A, sumPUMP);		//	Инициируем передачу данных в регистры "Holding Register" (AO), начиная с регистра AO[AO_TMR_A], всего sumPUMP регистров.
			                                 if( sumPUMP>=1 ){ objModbus->write( (uint16_t)(a*10) ); }						//	Ставим время a*10 в очередь на запись.
			                                 if( sumPUMP>=2 ){ objModbus->write( (uint16_t)(b*10) ); }						//	Ставим время b*10 в очередь на запись.
			                                 if( sumPUMP>=3 ){ objModbus->write( (uint16_t)(c*10) ); }						//	Ставим время c*10 в очередь на запись.
			                                 return objModbus->endTransmission();											//	Выполняем запись. Функция возвращает 1 при успехе, 0 при неудаче.
			}																												//
			return false;																									//
}																															//
																															//
//		ФУНКЦИЯ ЧТЕНИЯ ОСТАВШЕГОСЯ ВРЕМЕНИ РАБОТЫ НАСОСА:																	//	Возвращает оставшееся время работы насоса в секундах (0,0-6553,5), или -1 при провале чтения времени.
float	iarduino_MB_Pump::getTimeOn(uint8_t num){																			//	num - PUMP_X.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			if( num==PUMP_A && sumPUMP>=1 ){ return (float)(objModbus->holdingRegisterRead(valID, AO_TMR_A))/10.0f; }else	//	Читаем время до отключения насоса A из регистра "Holding Register" AO[AO_TMR_A]. Функция возвращает количество секунд (0,0...6553,5), или -1 при неудаче.
			if( num==PUMP_B && sumPUMP>=2 ){ return (float)(objModbus->holdingRegisterRead(valID, AO_TMR_B))/10.0f; }else	//	Читаем время до отключения насоса B из регистра "Holding Register" AO[AO_TMR_B]. Функция возвращает количество секунд (0,0...6553,5), или -1 при неудаче.
			if( num==PUMP_C && sumPUMP>=3 ){ return (float)(objModbus->holdingRegisterRead(valID, AO_TMR_C))/10.0f; }		//	Читаем время до отключения насоса C из регистра "Holding Register" AO[AO_TMR_C]. Функция возвращает количество секунд (0,0...6553,5), или -1 при неудаче.
			return -1;																										//	Если запрошены состояния всех насосов, возвращаем ошибку. Данная операция не поддерживается.
}																															//
																															//
//		ФУНКЦИЯ КОРРЕКЦИИ ПИТАНИЯ ДЛЯ НАСОСОВ ДО 12В:																		//	Возвращает результат изменения коррекции: true-успех / false-провал.
bool	iarduino_MB_Pump::setPomp12V(uint8_t num, bool a, bool b, bool c){													//	num - PUMP_X/PUMP_ALL, a,b,c - коррекция true/false.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( num==PUMP_A && sumPUMP>=1 ){ return objModbus->coilWrite(valID, DO_PUMP_12V_A, a); }else					//	Управляем коррекцией питания насоса A записав значение a в регистр "Coil" DO[DO_PUMP_12V_A]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_B && sumPUMP>=2 ){ return objModbus->coilWrite(valID, DO_PUMP_12V_B, a); }else					//	Управляем коррекцией питания насоса B записав значение a в регистр "Coil" DO[DO_PUMP_12V_B]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_C && sumPUMP>=3 ){ return objModbus->coilWrite(valID, DO_PUMP_12V_C, a); }else					//	Управляем коррекцией питания насоса C записав значение a в регистр "Coil" DO[DO_PUMP_12V_C]. Функция возвращает 1 при успехе, 0 при неудаче.
			if( num==PUMP_ALL             ){ objModbus->beginTransmission(valID, COILS, DO_PUMP_12V_A, sumPUMP);			//	Инициируем передачу данных в регистры "Coil" (DO), начиная с регистра DO[DO_PUMP_12V_A], всего sumPUMP регистров.
			                                 if( sumPUMP>=1 ){ objModbus->write( a ); }										//	Ставим бит a в очередь на запись.
			                                 if( sumPUMP>=2 ){ objModbus->write( b ); }										//	Ставим бит b в очередь на запись.
			                                 if( sumPUMP>=3 ){ objModbus->write( c ); }										//	Ставим бит c в очередь на запись.
			                                 return objModbus->endTransmission();											//	Выполняем запись. Функция возвращает 1 при успехе, 0 при неудаче.
			}																												//
			return false;																									//
}																															//
																															//
//		ФУНКЦИЯ ЧТЕНИЯ НАЛИЧИЯ КОРРЕКЦИИ ПИТАНИЯ НАСОСОВ ДО 12В:															//
int8_t	iarduino_MB_Pump::getPomp12V(uint8_t num){																			//	Возвращает флаг наличия коррекции питания насоса: true-вкл / false-выкл, байт флагов если читаются все насосы, или -1 при провале чтения коррекции насоса.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			if( num==PUMP_A && sumPUMP>=1 ){ return objModbus->coilRead(valID, DO_PUMP_12V_A); }else						//	Читаем флаг коррекции питания насоса A из регистра "Coil" DO[DO_PUMP_12V_A]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_B && sumPUMP>=2 ){ return objModbus->coilRead(valID, DO_PUMP_12V_B); }else						//	Читаем флаг коррекции питания насоса B из регистра "Coil" DO[DO_PUMP_12V_B]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_C && sumPUMP>=3 ){ return objModbus->coilRead(valID, DO_PUMP_12V_C); }else						//	Читаем флаг коррекции питания насоса C из регистра "Coil" DO[DO_PUMP_12V_C]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_ALL             ){																				//	Если запрошены флаги наличия коррекции питания всех насосов...
				if( objModbus->requestFrom(valID, COILS, DO_PUMP_12V_A, sumPUMP) != sumPUMP ){ return -1; }					//	Читаем регистры "Coils" начиная с регистра DO[DO_PUMP_12V_A], всего sumPUMP регистров. Функция возвращает количество прочитанных значений, или 0 при неудаче.
				uint8_t i=0;																								//
				if( sumPUMP>=1 ){ i += (objModbus->read()?1:0); }															//	Устанавливаем 0 бит байта i, если для насоса A включёна коррекция питания.
				if( sumPUMP>=2 ){ i += (objModbus->read()?2:0); }															//	Устанавливаем 1 бит байта i, если для насоса B включёна коррекция питания.
				if( sumPUMP>=3 ){ i += (objModbus->read()?4:0); }															//	Устанавливаем 2 бит байта i, если для насоса C включёна коррекция питания.
				return i;																									//
			}																												//
			return -1;																										//
}																															//
																															//
//		ФУНКЦИЯ ЧТЕНИЯ СОСТОЯНИЯ ДАТЧИКОВ УРОВНЕЙ ЖИДКОСТИ:																	//	Возвращает уровень датчика 0/1, байт битов если читаются все датчики, или -1 при провале чтения уровня датчика.
int8_t	iarduino_MB_Pump::digitalRead(uint8_t num){																			//	num - один из вариантов PUMP_SENSOR_XXX_X.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			if( num==PUMP_SENSOR_ALL ){																						//
			//	Читаем состояние всех датчиков одним запросом:																//
				if( objModbus->requestFrom(valID, DISCRETE_INPUTS, DI_MIN_A, 6) != 6 ){ return -1; }						//	Читаем регистры "Discrete Inputs" начиная с регистра DI[DI_MIN_A], всего 6 регистров. Функция возвращает количество прочитанных значений, или 0 при неудаче.
				return (objModbus->read()?1:0)|(objModbus->read()?2:0)|(objModbus->read()?4:0)|(objModbus->read()?8:0)|(objModbus->read()?16:0)|(objModbus->read()?32:0); // Собираем байт с шестью прочитанными битами и возвращаем его.
			}else																											//
			//	Читаем состояние указанного датчика:																		//
			if( num==PUMP_SENSOR_MIN_A && sumPUMP>=1 ){ return objModbus->discreteInputRead(valID, DI_MIN_A); }else			//	Читаем регистр "Discrete Input" DI[DI_MIN_A]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_SENSOR_MID_A && sumPUMP>=1 ){ return objModbus->discreteInputRead(valID, DI_MID_A); }else			//	Читаем регистр "Discrete Input" DI[DI_MID_A]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_SENSOR_MIN_B && sumPUMP>=2 ){ return objModbus->discreteInputRead(valID, DI_MIN_B); }else			//	Читаем регистр "Discrete Input" DI[DI_MIN_B]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_SENSOR_MID_B && sumPUMP>=2 ){ return objModbus->discreteInputRead(valID, DI_MID_B); }else			//	Читаем регистр "Discrete Input" DI[DI_MID_B]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_SENSOR_MIN_C && sumPUMP>=3 ){ return objModbus->discreteInputRead(valID, DI_MIN_C); }else			//	Читаем регистр "Discrete Input" DI[DI_MIN_C]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			if( num==PUMP_SENSOR_MID_C && sumPUMP>=3 ){ return objModbus->discreteInputRead(valID, DI_MID_C); }else			//	Читаем регистр "Discrete Input" DI[DI_MID_C]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
			//	Параметр num отличается от значений PUMP_SENSOR_XXX_X:														//
				{ return -1; }																								//
}																															//
																															//
//		ФУНКЦИЯ РАЗРЕШЕНИЯ РАБОТЫ СТОРОЖЕВОГО ТАЙМЕРА НАСОСОВ:																//	Возвращает результат включения сторожевого таймера: true-успех / false-провал.
bool	iarduino_MB_Pump::enableWDT(uint16_t ms){																			//	ms - время в миллисекундах от 1 до 65535.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			valWDT = ms;																									//	Сохраняем время для функции resetWDT().
			if( !objModbus->holdingRegisterWrite(valID, AO_WDT, ms) ){ return false; }										//	Записываем значение ms в регистр "Holding Register" AO[AO_WDT]. Функция возвращает 1 при успехе, 0 при неудаче.
			return objModbus->coilWrite(valID, DO_WDT, 1);																	//	Устанавливаем регистр "Coil" DO[DO_WDT]. Функция возвращает 1 при успехе, 0 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ ЗАПРЕТА РАБОТЫ СТОРОЖЕВОГО ТАЙМЕРА НАСОСОВ:																	//	Возвращает результат отключения сторожевого таймера: true-успех / false-провал.
bool	iarduino_MB_Pump::disableWDT(void){																					//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			return objModbus->coilWrite(valID, DO_WDT, 0);																	//	Сбрасываем регистр "Coil" DO[DO_WDT]. Функция возвращает 1 при успехе, 0 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ СБРОСА (ПЕРЕЗАПУСК) СТОРОЖЕВОГО ТАЙМЕРА НАСОСОВ:															//	Возвращает результат сброса сторожевого таймера: true-успех / false-провал.
bool	iarduino_MB_Pump::resetWDT(void){																					//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			return objModbus->holdingRegisterWrite(valID, AO_WDT, valWDT);													//	Записываем значение valWDT в регистр "Holding Register" AO[AO_WDT]. Функция возвращает 1 при успехе, 0 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ ЧТЕНИЯ СОСТОЯНИЯ СТОРОЖЕВОГО ТАЙМЕРА НАСОСОВ:																//	Возвращает состояние сторожевого таймера: 1-включён, 0-выключён, или -1 при провале чтения состояния сторожевого таймера.
int8_t	iarduino_MB_Pump::getStateWDT(void){																				//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			return objModbus->coilRead(valID, DO_WDT);																		//	Читаем регистр "Coil" DO[DO_WDT]. Функция возвращает прочитанное значение (0/1), или -1 при неудаче.
}																															//
