// ПРИМЕР ЧТЕНИЯ ДАННЫХ С ДАТЧИКОВ УРОВНЯ ЖИДКОСТИ:                                //
                                                                                   //
#include <iarduino_Modbus.h>                                                       //   Подключаем библиотеку для работы по протоколу Modbus.
#include <iarduino_MB_Pump.h>                                                      //   Подключаем библиотеку для работы с насосами.
                                                                                   //
ModbusClient     modbus(Serial1, 2);                                               //   Создаём объект для работы по протоколу Modbus указывая объект шины UART-RS485 для протокола и вывод DE конвертера UART-RS485.
iarduino_MB_Pump pump(modbus);                                                     //   Создаём объект для работы с насосами указывая объект протокола Modbus.
                                                                                   //
void setup(){                                                                      //
     Serial.begin(9600); while(!Serial);                                           //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     Serial1.begin(9600); while(!Serial1);                                         //   Инициируем работу с шиной UART-RS485 указав её скорость.
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
     int i;                                                                        //
                                                                                   //
//   Получаем состояние каждого датчика уровня жидкости по отдельности:            //
     i=pump.digitalRead(PUMP_SENSOR_MIN_A);                                        //   Запрашиваем состояние датчика MIN A.
     Serial.print(  "minA="); if(i<0){Serial.print("err");}else{Serial.print(i);}  //   Выводим     состояние датчика MIN A в монитор последовательного порта.
     i=pump.digitalRead(PUMP_SENSOR_MID_A);                                        //   Запрашиваем состояние датчика MID A.
     Serial.print(", midA="); if(i<0){Serial.print("err");}else{Serial.print(i);}  //   Выводим     состояние датчика MID A в монитор последовательного порта.
     i=pump.digitalRead(PUMP_SENSOR_MIN_B);                                        //   Запрашиваем состояние датчика MIN B.
     Serial.print(", minB="); if(i<0){Serial.print("err");}else{Serial.print(i);}  //   Выводим     состояние датчика MIN B в монитор последовательного порта.
     i=pump.digitalRead(PUMP_SENSOR_MID_B);                                        //   Запрашиваем состояние датчика MID B.
     Serial.print(", midB="); if(i<0){Serial.print("err");}else{Serial.print(i);}  //   Выводим     состояние датчика MID B в монитор последовательного порта.
     i=pump.digitalRead(PUMP_SENSOR_MIN_C);                                        //   Запрашиваем состояние датчика MIN C.
     Serial.print(", minC="); if(i<0){Serial.print("err");}else{Serial.print(i);}  //   Выводим     состояние датчика MIN C в монитор последовательного порта.
     i=pump.digitalRead(PUMP_SENSOR_MID_C);                                        //   Запрашиваем состояние датчика MID C.
     Serial.print(", midC="); if(i<0){Serial.print("err");}else{Serial.print(i);}  //   Выводим     состояние датчика MID C в монитор последовательного порта.
     Serial.println('.');                                                          //
     delay(1000);                                                                  //
                                                                                   //
//   Получаем состояние всех датчиков уровня жидкости одним запросом:              //
     i=pump.digitalRead(PUMP_SENSOR_ALL);                                          //   Запрашиваем состояние всех датчиков. Ответом является байт i, шесть младших битов которого отражают состояние датчиков.
     if( i<0 ){                                                                    //
        Serial.print("minA=err, midA=err, minB=err, midB=err, minC=err, midC=err");//   Выводим сообщение об ошибке.
     }else{                                                                        //
        Serial.print(  "minA="); Serial.print( bitRead(i,0) );                     //   Выводим значение 0 бита байта i, который отражает состояние датчика MIN A.
        Serial.print(", midA="); Serial.print( bitRead(i,1) );                     //   Выводим значение 1 бита байта i, который отражает состояние датчика MID A.
        Serial.print(", minB="); Serial.print( bitRead(i,2) );                     //   Выводим значение 2 бита байта i, который отражает состояние датчика MIN B.
        Serial.print(", midB="); Serial.print( bitRead(i,3) );                     //   Выводим значение 3 бита байта i, который отражает состояние датчика MID B.
        Serial.print(", minC="); Serial.print( bitRead(i,4) );                     //   Выводим значение 4 бита байта i, который отражает состояние датчика MIN C.
        Serial.print(", midC="); Serial.print( bitRead(i,5) );                     //   Выводим значение 5 бита байта i, который отражает состояние датчика MID C.
     }                                                                             //
     Serial.println('.');                                                          //
     delay(1000);                                                                  //
                                                                                   //
//   Данные выводимые в монитор последовательного порта одинаковы для 2 вариантов. //
}                                                                                  //
