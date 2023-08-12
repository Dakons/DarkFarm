#include <Arduino.h>
#include <FastBot.h>
#include <EEPROM.h>
#include <microDS3231.h>

// Обозначаем пины реле для воды и света
#define LIGHT_POWER 9
#define WATER_POWER 8




#define WIFI_SSID "pppp"
#define WIFI_PASS "44444444"
#define BOT_TOKEN "6167120438:AAF0Of85063TVWqB7AlDqOJzp2AIjXauFI0"

#define EEPROM_SIZE 256


unsigned long TimeNow; //Для таймера полива

bool Watering_Flag = 0; //Флаг полива. 0 - поливать не надо. 1 - поливать надо 

MicroDS3231 rtc; // Создаем объект таймера


FastBot bot(BOT_TOKEN);

struct Working_Preset // Структура работы теплицы 
{
  // Для управления светом
  int lighton_timemarker;  // Когда свет включается
  int lightoff_timemarker; // Когда свет выключается

  // Для управления поливом
  int period_watering;   // периодичность включений часы
  int duration_watering; // длительность полива секунды
  // bool WateringStart; //Включать ли полив при запуске кода

  int LastWateringTime; // время последнего полива
};

Working_Preset WP_MAIN;


//Для общения с ТГ
void connectWiFi();
void newMsg(FB_msg &msg);
int extract(String Command, String Text);



//Для функционирования фермы
void Watering();
void WaterControl();
void LightControl();



void setup()
{
  EEPROM.begin(EEPROM_SIZE);
  pinMode(LED_BUILTIN, OUTPUT);
  // Serial.begin(115200);
  connectWiFi();
  bot.setChatID(-837409212);
  bot.attach(newMsg);
  // bot.attach(Read);
  // bot.attach(Double);
}

void loop()
{

  bot.tick();
}

void connectWiFi()
{

  delay(2000);
  Serial.begin(9600);
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (millis() > 15000)
      ESP.restart();
  }
  Serial.println("Connected");
  digitalWrite(LED_BUILTIN, HIGH);
}

void newMsg(FB_msg &msg)
{
  if (msg.text == "/say_hello@DarkFarmRemote_bot")
    bot.sendMessage("Hello!", msg.chatID);
  else if (msg.text == "/led_on@DarkFarmRemote_bot")
    digitalWrite(LED_BUILTIN, HIGH);
  else if (msg.text == "/led_off@DarkFarmRemote_bot")
    digitalWrite(LED_BUILTIN, LOW);
  else if (msg.text == "/led_info@DarkFarmRemote_bot")
    bot.sendMessage(String(digitalRead(LED_BUILTIN)), msg.chatID);

  else if (msg.text == "/all_info@DarkFarmRemote_bot")
  {
    EEPROM.get(0, WP_MAIN);
    bot.sendMessage("Время последнего полива(LastWateringTime): " + String(WP_MAIN.LastWateringTime), msg.chatID);

    bot.sendMessage("Длительность полива(duration_watering): " + String(WP_MAIN.duration_watering), msg.chatID);

    bot.sendMessage("Период полива(period_watering): " + String(WP_MAIN.period_watering), msg.chatID);

    bot.sendMessage("Когда свет выключается(lightoff_timemarker): " + String(WP_MAIN.lightoff_timemarker), msg.chatID);

    bot.sendMessage("Когда свет включается(lighton_timemarker): " + String(WP_MAIN.lighton_timemarker), msg.chatID);
  }

  else if (msg.text.startsWith("/change_duration_watering"))
  {

    if (extract("/change_lighton_timemarker", msg.text) == 0)
    {
      bot.sendMessage("Значение введено не корректно", msg.chatID);
      return;
    }

    WP_MAIN.duration_watering = extract("/change_duration_watering", msg.text);

    // EEPROM.put(0, WP_MAIN.duration_watering);
    EEPROM.put(0, WP_MAIN);
    EEPROM.commit();
    // EEPROM.end();
    EEPROM.get(0, WP_MAIN);
    bot.sendMessage("Новое значение: " + String(WP_MAIN.duration_watering), msg.chatID);
  }

  else if (msg.text.startsWith("/change_period_watering"))
  {

    if (extract("/change_lighton_timemarker", msg.text) == 0)
    {
      bot.sendMessage("Значение введено не корректно", msg.chatID);
      return;
    }

    WP_MAIN.period_watering = extract("/change_period_watering", msg.text);

    // EEPROM.put(0, WP_MAIN.period_watering);
    EEPROM.put(0, WP_MAIN);
    EEPROM.commit();
    // EEPROM.end();
    EEPROM.get(0, WP_MAIN);
    bot.sendMessage("Новое значение: " + String(WP_MAIN.period_watering), msg.chatID);
  }

  else if (msg.text.startsWith("/change_lightoff_timemarker"))
  {

    if (extract("/change_lighton_timemarker", msg.text) == 0)
    {
      bot.sendMessage("Значение введено не корректно", msg.chatID);
      return;
    }

    WP_MAIN.lightoff_timemarker = extract("/change_lightoff_timemarker", msg.text);
    // EEPROM.put(0, WP_MAIN.period_watering);
    // Serial.println(WP_MAIN.lightoff_timemarker);

    EEPROM.put(0, WP_MAIN);
    EEPROM.commit();
    // EEPROM.end();
    EEPROM.get(0, WP_MAIN);
    bot.sendMessage("Новое значение: " + String(WP_MAIN.lightoff_timemarker), msg.chatID);
  }

  else if (msg.text.startsWith("/change_lighton_timemarker"))
  {

    if (extract("/change_lighton_timemarker", msg.text) == 0)
    {
      bot.sendMessage("Значение введено не корректно", msg.chatID);
      return;
    }

    WP_MAIN.lighton_timemarker = extract("/change_lighton_timemarker", msg.text);
    // EEPROM.put(0, WP_MAIN.period_watering);

    EEPROM.put(0, WP_MAIN);
    EEPROM.commit();
    // EEPROM.end();
    EEPROM.get(0, WP_MAIN);
    bot.sendMessage("Новое значение: " + String(WP_MAIN.lighton_timemarker), msg.chatID);
  }


  Serial.print(msg.chatID); // ID чата
  Serial.print(", ");
  Serial.print(msg.username); // логин
  Serial.print(", ");
  Serial.println(msg.text); // текст
}

int extract(String Command, String Text)
{

  Text.replace("@DarkFarmRemote_bot", ""); // убираю отметку
  // Serial.println(Text);
  Text.replace(Command, ""); // убираю команду
  // Serial.println(Text);
  Text.trim(); // Удаляю пробелы
  // Serial.println(Text);
  return Text.toInt();
}


void LightControl()
{
    if (rtc.getHours() >= WP_MAIN.lighton_timemarker && rtc.getHours() <= WP_MAIN.lightoff_timemarker)
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
  EEPROM.get(0, WP_MAIN);
  byte LastWaterTime = WP_MAIN.LastWateringTime;
    if (rtc.getHours() - LastWaterTime >= WP_MAIN.period_watering) //Проверяем прошло ли время периода
    {
        LastWaterTime = rtc.getHours(); //  нынешнее время ставим

        WP_MAIN.LastWateringTime = LastWaterTime;
        EEPROM.put(0, WP_MAIN);
        EEPROM.commit();


        digitalWrite(WATER_POWER, HIGH); //Включаем полив


        TimeNow = millis();//Фиксируем начало полива
        Watering_Flag = 1; //Включаем флаг полива (Надо поливать)
    }
}

void Watering() //Выполнение полива
{
    if ((millis() - TimeNow > WP_MAIN.duration_watering * 1000) && Watering_Flag == 1)
    {
      digitalWrite(WATER_POWER, HIGH); //Включаю реле для полива
    }
    else 
    {
      digitalWrite(WATER_POWER, LOW); //Выключаю реле для полива
      Watering_Flag = 0;
    }
}