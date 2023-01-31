#include <Arduino.h>

#include <microDS3231.h> //Библиотека для таймера

// Обозначаем пины таймера
#define SDA_TIMER 10
#define SCL_TIMER 11


// Обозначаем пины реле для воды и света
#define LIGHT_POWER 9
#define WATER_POWER 8


unsigned long TimeNow; //Для таймера полива

bool Watering_Flag = 0; //Флаг полива. 0 - поливать не надо. 1 - поливать надо 


MicroDS3231 rtc; // Создаем объект таймера

struct Light_Preset //Структура для пресета света
{

byte FirstTimeMarker;  //Когда свет включается
byte SecondTimeMarker; //Когда свет выключается

};

 Light_Preset LP_FIRST; //Создаем структуру пресета для света

struct Water_Preset //Структура для пресета полива
{

byte Period;  //периодичность включений Часы
byte Duration; //длительность полива Секунды
bool WateringStart; //Включать ли полив при запуске кода

};

Water_Preset WP_FIRST;

byte LastWaterTime; //Для сохранение времени последнего полива






/////////////////Прототипы функций

void LightControl();

void WaterControl();

void Watering();
//////////////////

void setup()
{

    //Настраиваем пресет света
LP_FIRST.FirstTimeMarker = 9;
LP_FIRST.SecondTimeMarker = 21;
   

   //Настраиваем пресет Полива
WP_FIRST.Period = 1;
WP_FIRST.Duration = 15;

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

LastWaterTime = rtc.getHours();
TimeNow = millis();
Watering_Flag = WP_FIRST.WateringStart; //Включаем полив или не включаем полив

}



void loop()
{

LightControl();

WaterControl();

Watering();


}



///Функции

void LightControl()
{
    if (rtc.getHours() >= LP_FIRST.FirstTimeMarker && rtc.getHours() <= LP_FIRST.SecondTimeMarker)
    {
        digitalWrite(LIGHT_POWER, HIGH);
    }

    else

    {
        digitalWrite(LIGHT_POWER, LOW);
    }
}


void WaterControl() //Контроль полива
{
    if (rtc.getHours() - LastWaterTime >= WP_FIRST.Period) //Проверяем прошло ли время периода
    {
        LastWaterTime = rtc.getHours(); //  нынешнее время ставим

        digitalWrite(WATER_POWER, HIGH); //Включаем полив
        TimeNow = millis();//Фиксируем начало полива
        Watering_Flag = 1; //Включаем флаг полива

    }
}

void Watering() //Выполнение полива
{
    if ((millis() - TimeNow > WP_FIRST.Duration * 1000) && Watering_Flag == 1)
    {
        digitalWrite(WATER_POWER, LOW);
        Watering_Flag = 0;
    }


}

void kek()
{

}

////