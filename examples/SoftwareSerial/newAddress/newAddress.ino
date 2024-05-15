// ПРИМЕР СМЕНЫ АДРЕСА:                                                            //
                                                                                   //
uint8_t nowAddress = 2;                                                            //   Текущий адрес ( 1 - 247 ).
uint8_t newAddress = 10;                                                           //   Новый адрес ( 1 - 247 ).
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
     int f;                                                                        //
     Serial.begin(9600);   while(!Serial);                                         //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     rs485.begin(9600); // while(!rs485 );                                         //   Инициируем работу с программной шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
//   Инициируем работу с насосами:                                                 //
     f =    pump.begin(nowAddress);                                                //   Инициируем работу с насосами, указав текущий адрес модуля nowAddress.
     if(f){ Serial.println("Модуль насосов найден");            }                  //   Если адрес не указан f=pump.begin(), то он будет найден, но это займёт некоторое время.
     else { Serial.println("Модуль насосов не найден"); return; }                  //
//   Меняем адрес модуля насосов:                                                  //
     f =    pump.changeID(newAddress);                                             //   Меняем адрес модуля насосов на новый newAddress.
     if(f){ Serial.println("Адрес изменён");            }                          //
     else { Serial.println("Адрес не изменён"); return; }                          //
//   Выводим сообшение о модуле насосов:                                           //
     Serial.println((String) "Текущий адрес      = "+pump.getID()      );          //   Выводим текущий адрес модуля насосов.
     Serial.println((String) "Версия прошивки    = "+pump.getVersion() );          //   Выводим версию прошивки модуля насосов.
     Serial.println((String) "Количество насосов = "+pump.getSumPump() );          //   Выводим количество насосов в модуле.
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
//   Мигаем светодиодом обнаружения устройства (на разъёме):                       //
     pump.setIDLED(false); delay(1000);                                            //
     pump.setIDLED(true ); delay(1000);                                            //
}                                                                                  //
