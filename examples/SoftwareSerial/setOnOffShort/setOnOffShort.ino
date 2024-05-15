// ПРИМЕР УПРАВЛЕНИЯ НАСОСАМИ БЕЗ ПРОВЕРКИ:                                        //
// Управление насосами с проверкой доступно в примере "setOnOff"                   //
                                                                                   //
#include <SoftwareSerial.h>                                                        //   Подключаем библиотеку для работы с программной шиной UART.
#include <iarduino_Modbus.h>                                                       //   Подключаем библиотеку для работы по протоколу Modbus.
#include <iarduino_MB_Pump.h>                                                      //   Подключаем библиотеку для работы с насосами.
                                                                                   //
SoftwareSerial   rs485(8,9);                                                       //   Создаём объект для работы с программной шиной UART-RS485 указывая выводы RX, TX.
ModbusClient     modbus(rs485, 2);                                                 //   Создаём объект для работы по протоколу Modbus указывая объект программной шины UART-RS485 и вывод DE конвертера UART-RS485.
iarduino_MB_Pump pump(modbus);                                                     //   Создаём объект для работы с насосами указывая объект протокола Modbus.
                                                                                   //
void setup(){                                                                      //
     rs485.begin(9600); // while(!rs485);                                          //   Инициируем работу с программной шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
     pump.begin(2);                                                                //   Инициируем работу с насосами, указав адрес модуля. Если адрес не указан pump.begin(), то он будет найден, но это займёт некоторое время.
//   pump.setFrequency(PUMP_ALL, 500, 1000, 2000);                                 //   Указываем частоту ШИМ для управления мощностью насосов A,B,C (по умолчанию 1000Гц). В примере: A=500Гц, B=1000Гц, C=2000Гц.
//   pump.setFrequency(PUMP_ALL, 500, 1000);                                       //   Указываем частоту ШИМ для управления мощностью насосов A,B (для модуля с двумя насосами).
//   pump.setPower(PUMP_ALL, 192, 128, 64);                                        //   Указываем мощность насосов A,B,C (по умолчанию 100%). В примере: A=192=75%, B=128=50%, C=64=25%.
//   pump.setPower(PUMP_ALL, 192, 128);                                            //   Указываем мощность насосов A,B (для модуля с двумя насосами).
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
     pump.setState(PUMP_A, true ); delay(1000);                                    //   Включаем  насос A и ждём 1 секунду.
     pump.setState(PUMP_A, false); delay(1000);                                    //   Отключаем насос A и ждём 1 секунду.
     pump.setState(PUMP_B, true ); delay(1000);                                    //   Включаем  насос B и ждём 1 секунду.
     pump.setState(PUMP_B, false); delay(1000);                                    //   Отключаем насос B и ждём 1 секунду.
     pump.setState(PUMP_C, true ); delay(1000);                                    //   Включаем  насос C и ждём 1 секунду.
     pump.setState(PUMP_C, false); delay(1000);                                    //   Отключаем насос C и ждём 1 секунду.
     pump.setState(PUMP_ALL, true , false, true ); delay(1000);                    //   Управляем всеми насосами (A-вкл , B-откл, C-вкл ) и ждём 1 секунду.
     pump.setState(PUMP_ALL, false, false, false); delay(1000);                    //   Управляем всеми насосами (A-откл, B-откл, C-откл) и ждём 1 секунду.
}