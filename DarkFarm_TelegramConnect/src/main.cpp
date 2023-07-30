#include <Arduino.h>
#include <FastBot.h>



#define WIFI_SSID "pppp"
#define WIFI_PASS "44444444"
#define BOT_TOKEN "6167120438:AAF0Of85063TVWqB7AlDqOJzp2AIjXauFI0"


FastBot bot(BOT_TOKEN);



void connectWiFi();
void Read(FB_msg& msg);
void newMsg(FB_msg& msg);
void Double(FB_msg& msg);

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
    // выводим ID чата, имя юзера и текст сообщения
  Serial.print(msg.chatID);     // ID чата 
  Serial.print(", ");
  Serial.print(msg.username);   // логин
  Serial.print(", ");
  Serial.println(msg.text);     // текст

/*
  if (msg.text == "/say_hello") bot.sendMessage("Hello!", msg.chatID);
  else if (msg.text == "/command1") digitalWrite(LED_BUILTIN, LOW);
  else if (msg.text == "/command2") digitalWrite(LED_BUILTIN, HIGH);

*/
}


void Read(FB_msg& msg)
 {
  // выводим ID чата, имя юзера и текст сообщения
  Serial.print(msg.chatID);     // ID чата 
  Serial.print(", ");
  Serial.print(msg.username);   // логин
  Serial.print(", ");
  Serial.println(msg.text);     // текст
}


void Double(FB_msg& msg) 
{
  // отправить сообщение обратно
  bot.sendMessage(msg.text, msg.chatID);  
}