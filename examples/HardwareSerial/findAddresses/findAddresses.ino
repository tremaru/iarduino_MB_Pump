// ПРИМЕР ПОЛУЧЕНИЯ СПИСКА АДРЕСОВ ВСЕХ УСТРОЙСТВ НА ШИНЕ:                         //
                                                                                   //
#include <iarduino_Modbus.h>                                                       //   Подключаем библиотеку для работы по протоколу Modbus.
#include <iarduino_MB_Pump.h>                                                      //   Подключаем библиотеку для работы с насосами.
                                                                                   //
ModbusClient modbus(Serial1, 2);                                                   //   Создаём объект для работы по протоколу Modbus указывая объект шины UART-RS485 для протокола и вывод DE конвертера UART-RS485.
                                                                                   //
void setup(){                                                                      //
     Serial.begin(9600); while(!Serial);                                           //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     Serial1.begin(9600); while(!Serial1);                                         //   Инициируем работу с шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
     Serial.println("Поиск устройств...");                                         //
//   Выполняем поиск всех устройств на шине (занимает несколько секунд):           //
     modbus.findID();                                                              //   Ищем адреса всех устройств на шине.
//   modbus.findID( DEF_MODEL_PUMP1 );                                             //   Ищем адреса всех устройств с идентификатором модуля "1 насос"  = DEF_MODEL_PUMP1.
//   modbus.findID( DEF_MODEL_PUMP2 );                                             //   Ищем адреса всех устройств с идентификатором модуля "2 насоса" = DEF_MODEL_PUMP2.
//   modbus.findID( DEF_MODEL_PUMP3 );                                             //   Ищем адреса всех устройств с идентификатором модуля "3 насоса" = DEF_MODEL_PUMP3.
     if( modbus.available() ){                                                     //
     //  Устройства найдены:                                                       //
         Serial.print("Найдено ");                                                 //
         Serial.print( modbus.available() );                                       //
         Serial.print(" устройств с адресами: ");                                  //
     //  Выводим адреса устройств:                                                 //
         while( modbus.available() ){                                              //
             Serial.print( modbus.read() );                                        //
             Serial.print(", ");                                                   //
         }   Serial.println();                                                     //
     }else{  Serial.println("Не найдено ни одного устройства."); }                 //
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
}                                                                                  //