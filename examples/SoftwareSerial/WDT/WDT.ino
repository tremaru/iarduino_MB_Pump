// ПРИМЕР УПРАВЛЕНИЯ НАСОСАМИ С ВКЛЮЧЁННЫМ СТОРОЖЕВЫМ ТАЙМЕРОМ:                    //
// Введите любой символ в монитор последовательного порта...                       //
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
     Serial.begin(9600); while(!Serial);                                           //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     rs485.begin(9600); while(!rs485);                                             //   Инициируем работу с программной шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
     pump.begin(2);                                                                //   Инициируем работу с насосами, указав адрес модуля. Если адрес не указан pump.begin(), то он будет найден, но это займёт некоторое время.
//   pump.setFrequency(PUMP_ALL, 500, 1000, 2000);                                 //   Указываем частоту ШИМ для управления мощностью насосов A,B,C (по умолчанию 1000Гц). В примере: A=500Гц, B=1000Гц, C=2000Гц.
//   pump.setFrequency(PUMP_ALL, 500, 1000);                                       //   Указываем частоту ШИМ для управления мощностью насосов A,B (для модуля с двумя насосами).
//   pump.setPower(PUMP_ALL, 192, 128, 64);                                        //   Указываем мощность насосов A,B,C (по умолчанию 100%). В примере: A=192=75%, B=128=50%, C=64=25%.
//   pump.setPower(PUMP_ALL, 192, 128);                                            //   Указываем мощность насосов A,B (для модуля с двумя насосами).
     pump.enableWDT(200);                                                          //   Включаем сторожевой таймер указав время в мс. Если между обращениями к функции pump.resetWDT() пройдёт указанное время, насосы сами отключатся.
//   pump.disableWDT();                                                            //   Отключаем сторожевой таймер.
     pump.setState(PUMP_ALL, true, true, true);                                    //   Включаем все три насоса (для модуля с тремя насосами).
//   pump.setState(PUMP_ALL, true, true);                                          //   Включаем все два насоса (для модуля с двумя насосами).
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
     pump.resetWDT();                                                              //   Сбрасываем сторожевой таймер, не давая ему сработать.
     delay(50);                                                                    //   Задержка между обращениями к функции pump.resetWDT() не должна превышать время указанное в pump.enableWDT().
//   Ждём ввода любого символа в монитор последовательного порта:                  //
     while( Serial.available() );                                                  //   Если символ введён, то входим в бесконечный цикл while(). Функция pump.resetWDT() больше вызываться не будет и сработает сторожевой таймер насосов.
//   Serial.println((String) "Сторожевой таймер="+pump.getStateWDT() );            //   Функция pump.getStateWDT() позволяет узнать включён или отключён сторожевой таймер функциями enableWDT(), disableWDT().
}                                                                                  //   Если сторожевой таймер сработал (отключил насосы), то сам он не отключается и позволит включить насосы после pump.resetWDT().
