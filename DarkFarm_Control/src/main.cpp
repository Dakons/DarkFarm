#include <Arduino.h>

#include <microDS3231.h> //Библиотека для таймера

// Обозначаем пины таймера
#define SDA_TIMER 10
#define SCL_TIMER 11

MicroDS3231 rtc; // Создаем объект таймера

struct Light_Preset //Структура для пресета света
{

byte FirstTimeMarkerWork;  //Когда свет включается
byte SecondTimeMarkerWork; //Когда свет выключается

};

struct Water_Preset //Структура для пресета полива
{

byte Period;  //периодичность включений
byte Duration; //длительность полива

};

void setup()
{

    Serial.begin(115200);
    // Запускаем таймер, устанавливаем время
    if (rtc.begin())
    {
        rtc.setTime(0, 0, 0, 0, 0, 0); // Установка времени void setTime(int8_t seconds, int8_t minutes, int8_t hours, int8_t date, int8_t month, int16_t year);
    }
    else
    {
        Serial.println("Таймер не найден");
    }


}


void loop()
{

}