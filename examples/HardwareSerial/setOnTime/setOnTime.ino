// ПРИМЕР ВКЛЮЧЕНИЯ НАСОСОВ НА УКАЗАННОЕ ВРЕМЯ:                                    //
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
     float i;                                                                      //
                                                                                   //
//   Включаем насос A:                                                             //
     Serial.print("Включаем насос A на 5 секунд: ");                               //
     i=pump.setTimeOn(PUMP_A, 5.0);                                                //   Отправляем команду включения насоса A на указанное количество секунд.
     if( i ){ Serial.println("Ок."); }else{ Serial.println("Err."); }              //   Выводим результат выполнения команды.
//   Проверяем состояние насоса A:                                                 //
     do{ i=pump.getTimeOn(PUMP_A);                                                 //   Получаем оставшееся время до отключения насоса A.
         Serial.println((String)"До отключения насоса A осталось "+i+" сек.");     //   Выводим оставшееся время до отключения насоса A.
         delay(200);                                                               //
     }while(i);                                                                    //   Повторяем цикл, если время ещё осталось.
     delay(1000);                                                                  //   Ждём 1 секунду.
                                                                                   //
//   Включаем насос B:                                                             //
     Serial.print("Включаем насос B на 5 секунд: ");                               //
     i=pump.setTimeOn(PUMP_B, 5.0);                                                //   Отправляем команду включения насоса B на указанное количество секунд.
     if( i ){ Serial.println("Ок."); }else{ Serial.println("Err."); }              //   Выводим результат выполнения команды.
//   Проверяем состояние насоса B:                                                 //
     do{ i=pump.getTimeOn(PUMP_B);                                                 //   Получаем оставшееся время до отключения насоса B.
         Serial.println((String)"До отключения насоса B осталось "+i+" сек.");     //   Выводим оставшееся время до отключения насоса B.
         delay(200);                                                               //
     }while(i);                                                                    //   Повторяем цикл, если время ещё осталось.
     delay(1000);                                                                  //   Ждём 1 секунду.
                                                                                   //
//   Включаем насос C:                                                             //
     Serial.print("Включаем насос C на 5 секунд: ");                               //
     i=pump.setTimeOn(PUMP_C, 5.0);                                                //   Отправляем команду включения насоса C на указанное количество секунд.
     if( i ){ Serial.println("Ок."); }else{ Serial.println("Err."); }              //   Выводим результат выполнения команды.
//   Проверяем состояние насоса C:                                                 //
     do{ i=pump.getTimeOn(PUMP_C);                                                 //   Получаем оставшееся время до отключения насоса C.
         Serial.println((String)"До отключения насоса C осталось "+i+" сек.");     //   Выводим оставшееся время до отключения насоса C.
         delay(200);                                                               //
     }while(i);                                                                    //   Повторяем цикл, если время ещё осталось.
     delay(1000);                                                                  //   Ждём 1 секунду.
                                                                                   //
//   Включаем насосы на разное время:                                              //
     Serial.print("Включаем насосы A на 5.0 сек, B на 7.3 сек, C на 9.7 сек: ");   //
     i=pump.setTimeOn(PUMP_ALL, 5.0, 7.3, 9.7);                                    //   Отправляем команду включения насосов на указанное время.
     if( i ){ Serial.println("Ок."); }else{ Serial.println("Err."); }              //   Выводим результат выполнения команды.
//   Проверяем состояние каждого насоса по отдельности:                            //
     bool f;                                                                       //
     do{ Serial.print("До отключения насосов A,B,C осталось ");                    //
         f=0;                                                                      //
         i=pump.getTimeOn(PUMP_A); if(i){f=1;} Serial.print((String) i+", " );     //   Получаем оставшееся время до отключения насоса A.
         i=pump.getTimeOn(PUMP_B); if(i){f=1;} Serial.print((String) i+", " );     //   Получаем оставшееся время до отключения насоса B.
         i=pump.getTimeOn(PUMP_C); if(i){f=1;} Serial.print((String) i+" сек.");   //   Получаем оставшееся время до отключения насоса С.
         Serial.println();                                                         //
         delay(200);                                                               //
     }while(f);                                                                    //   Повторяем цикл, если время ещё осталось.
     delay(1000);                                                                  //   Ждём 1 секунду.
                                                                                   //
//   Если функции pump.setTimeOn() указать 0.0 секунд, то насос сразу отключится.  //
}                                                                                  //