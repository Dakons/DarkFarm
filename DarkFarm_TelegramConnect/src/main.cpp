#include <Arduino.h>
#include <FastBot.h>



#define WIFI_SSID "pppp"
#define WIFI_PASS "44444444"
#define BOT_TOKEN "6167120438:AAF0Of85063TVWqB7AlDqOJzp2AIjXauFI0"


FastBot bot(BOT_TOKEN);



void connectWiFi();
void newMsg(FB_msg& msg);


void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  //Serial.begin(115200);
  connectWiFi();
  bot.setChatID(-837409212);
  bot.attach(newMsg);
  //bot.attach(Read);
  //bot.attach(Double);
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
  digitalWrite(LED_BUILTIN, HIGH);
  
}

void newMsg(FB_msg& msg)
 {
  if (msg.text == "/say_hello@DarkFarmRemote_bot") bot.sendMessage("Hello!", msg.chatID);
  else if (msg.text == "/led_on@DarkFarmRemote_bot") digitalWrite(LED_BUILTIN, HIGH);
  else if (msg.text == "/led_off@DarkFarmRemote_bot") digitalWrite(LED_BUILTIN, LOW);
  else if (msg.text == "/led_info@DarkFarmRemote_bot") bot.sendMessage(String(digitalRead(LED_BUILTIN)), msg.chatID);

  Serial.print(msg.chatID);     // ID чата 
  Serial.print(", ");
  Serial.print(msg.username);   // логин
  Serial.print(", ");
  Serial.println(msg.text);     // текст
}
