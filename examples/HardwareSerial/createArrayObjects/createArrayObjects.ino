// ПРИМЕР УПРАВЛЕНИЯ ВСЕМИ НАСОСАМИ НА ШИНЕ:                                       //
// без указания их адресов в скетче.                                               //
                                                                                   //
#include <iarduino_Modbus.h>                                                       //   Подключаем библиотеку для работы по протоколу Modbus.
#include <iarduino_MB_Pump.h>                                                      //   Подключаем библиотеку для работы с насосами.
                                                                                   //
ModbusClient      modbus(Serial1, 2);                                              //   Создаём объект для работы по протоколу Modbus указывая объект шины UART-RS485 для протокола и вывод DE конвертера UART-RS485.
iarduino_MB_Pump* pump;                                                            //   Создаём указатель который будет переопределён в массив объектов работы с насосами.
                                                                                   //
uint8_t sum=0;                                                                     //   Определяем переменную для хранения количества найденных модулей с насосами.
                                                                                   //
void setup(){                                                                      //
     Serial.begin(9600); while(!Serial);                                           //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     Serial1.begin(9600); while(!Serial1);                                         //   Инициируем работу с шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
     uint8_t i;                                                                    //
                                                                                   //
//   Выполняем поиск всех модулей на шине с 1 насосом (занимает несколько секунд): //
     Serial.print("Поиск модулей с 1 насосом ... " );                              //
     uint8_t sumID1 = modbus.findID( DEF_MODEL_PUMP1 ); sum+=sumID1;               //   Ищем адреса всех устройств с идентификатором модуля "1 насос" = DEF_MODEL_PUMP1.
     uint8_t arrID1[sumID1];                                                       //   Объявляем массив arrID1 для хранения найденных адресов.
     i=0; while( modbus.available() ){ arrID1[i++]=modbus.read(); }                //   Заполняем массив arrID1 найденными адресами.
     Serial.print("найдено "); Serial.print(sumID1); Serial.println(" модулей.");  //
                                                                                   //
//   Выполняем поиск всех модулей на шине с 2 насосами (занимает несколько секунд)://
     Serial.print("Поиск модулей с 2 насосами ... " );                             //
     uint8_t sumID2 = modbus.findID( DEF_MODEL_PUMP2 ); sum+=sumID2;               //   Ищем адреса всех устройств с идентификатором модуля "2 насоса" = DEF_MODEL_PUMP2.
     uint8_t arrID2[sumID2];                                                       //   Объявляем массив arrID2 для хранения найденных адресов.
     i=0; while( modbus.available() ){ arrID2[i++]=modbus.read(); }                //   Заполняем массив arrID2 найденными адресами.
     Serial.print("найдено "); Serial.print(sumID2); Serial.println(" модулей.");  //
                                                                                   //
//   Выполняем поиск всех модулей на шине с 3 насосами (занимает несколько секунд)://
     Serial.print("Поиск модулей с 3 насосами ... " );                             //
     uint8_t sumID3 = modbus.findID( DEF_MODEL_PUMP3 ); sum+=sumID3;               //   Ищем адреса всех устройств с идентификатором модуля "3 насоса" = DEF_MODEL_PUMP3.
     uint8_t arrID3[sumID3];                                                       //   Объявляем массив arrID3 для хранения найденных адресов.
     i=0; while( modbus.available() ){ arrID3[i++]=modbus.read(); }                //   Заполняем массив arrID3 найденными адресами.
     Serial.print("найдено "); Serial.print(sumID3); Serial.println(" модулей.");  //
                                                                                   //
     if( sum ){                                                                    //
     //  Переопределяем указатель pump в массив объектов:                          //
         pump = (iarduino_MB_Pump*) malloc( sizeof(iarduino_MB_Pump)*sum );        //   Выделяем под массив pump требуемый объем памяти.
         i=0;                                                                      //
         for(int j=0;j<sumID1;j++,i++){pump[i]=modbus; pump[i].begin(arrID1[j]);}  //   Инициируем работу с модулями имеющими 1 насос.
         for(int j=0;j<sumID2;j++,i++){pump[i]=modbus; pump[i].begin(arrID2[j]);}  //   Инициируем работу с модулями имеющими 2 насоса.
         for(int j=0;j<sumID3;j++,i++){pump[i]=modbus; pump[i].begin(arrID3[j]);}  //   Инициируем работу с модулями имеющими 3 насоса.
     }                                                                             //
                                                                                   //
//   Выводим сообщение:                                                            //
     Serial.print((String) "Всего найдено "+sum+" модулей насосов, с адресами: " );//
     for(i=0; i<sum; i++){ Serial.print( pump[i].getID() ); Serial.print(", "); }  //
     Serial.println();                                                             //
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
     for(uint8_t i=0, j; i<sum; i++, j=0){                                         //   Проходим по всем модулям с насосами.
         Serial.print( pump[i].getID() );                                          //   Выводим   ID текущего модуля с насосами.
         if( pump[i].setState(PUMP_A, true ) ){j++;}                               //   Включаем  насос A.
         if( pump[i].setIDLED(        true ) ){j++;}                               //   Включаем  светодиод обнаружения устройства (на разъёме).
         delay(1000);                                                              //
         if( pump[i].setState(PUMP_A, false) ){j++;}                               //   Отключаем насос A.
         if( pump[i].setIDLED(        false) ){j++;}                               //   Отключаем светодиод обнаружения устройства (на разъёме).
         Serial.println( (j<4)? "=ERR":"=OK" );                                    //   Выводим   Результат управления насосом.
         delay(1000);                                                              //
     }                                                                             //
}                                                                                  //
