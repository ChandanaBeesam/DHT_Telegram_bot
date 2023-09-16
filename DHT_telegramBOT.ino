#include<WiFi.h>
#include<HTTPClient.h>
#include "DHT.h"
#include<UniversalTelegramBot.h>
#include<ArduinoJson.h>
#include<WiFiClientSecure.h>

DHT dht(4,DHT11);

#define BOTtoken "6--BOT TOKEN--"

float temperature;
float humidity;

const char* ssid="HIGHsenberg";
const char* password="Hemanth@2002";

int botRequestDelay = 500;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken,client);

void handleNewMessages(int newMessagesCount){
  int i;
  for(i=0;i<newMessagesCount;i++){
    String chat_id=String(bot.messages[i].chat_id);
    String messageText= bot.messages[i].text;
    String fromUser = bot.messages[i].from_name;
    Serial.println("Message from "+fromUser+" : "+messageText);
    if(messageText=="/start"){
      String welcome = "Welcome "+fromUser+"\n";
      welcome+="/temp to display current temperature\n";
      welcome+="/humid to display current humidity\n";
      bot.sendMessage(chat_id,welcome,"");
    }
    if(messageText=="/temp"){ //State command th check the current state of LED
      temperature=dht.readTemperature();  // Reading temperature and humidity.
      String state="The temperature is "+String(temperature);
      if(isnan(temperature)){
        bot.sendMessage(chat_id,"Failed to receive temperature","");
      }
      bot.sendMessage(chat_id,state,"");
    }
    if(messageText=="/humid"){ //State command th check the current state of LED
      humidity=dht.readHumidity();  // Reading temperature and humidity.
      String state="The humidity is "+String(humidity);
      if(isnan(humidity)){
        bot.sendMessage(chat_id,"Failed to receive humidity","");
      }
      bot.sendMessage(chat_id,state,"");
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  client.setInsecure();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\n  WiFi connected");
  dht.begin();
}

void loop() {
  if (WiFi.status()==WL_CONNECTED)  {
    int newMessagesCount = bot.getUpdates(bot.last_message_received + 1);//No of new messages received
    while(newMessagesCount) { //Run until the new message count is not zero
      Serial.println("Got response");
      handleNewMessages(newMessagesCount);
      newMessagesCount = bot.getUpdates(bot.last_message_received + 1);
    }
    delay(500); // delay of 0.5 seconds
  }
}
