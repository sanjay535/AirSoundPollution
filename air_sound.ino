#include <MQ135.h>
#include<LiquidCrystal.h>
#include "WiFi.h"
#include <WiFiClient.h>

const char* server = "api.thingspeak.com";
String apiKey ="0MKK8I4KZ1310HK4";
const char* ssid= "Satyam"; 
const char*  password= "parmanand";

LiquidCrystal lcd(23, 22, 5, 18, 19, 21);
const int ANALOGPIN =2;
const int redLed=25;
const int greenLed=33;
const int buzzer=32;
 MQ135 gasSensor = MQ135(ANALOGPIN);
void setup()
{
  
  
  pinMode(4, INPUT); //sound level
  pinMode(2, INPUT); // pin 2 read from GAS MQ135
  pinMode(25, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  lcd.begin(16, 2);
 Serial.begin(9600);
 
//  WiFi.begin(ssid, password);
// 
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.println("Connecting to WiFi..");
//  }
   
 }

void loop(){
  
  int mq135=round(gasSensor.getPPM());  // reading from gas sensor
  int adc= analogRead(4);       //reading from sound sensor
 int sound = round((adc+83.2073)/11.003);
Serial.print("AirQuality=");
Serial.print(mq135, DEC);  // prints the value read
Serial.println(" PPM");
lcd.setCursor(0,0);
lcd.print("AirQ=");
lcd.print(mq135, DEC);
lcd.print(" PPM");
lcd.setCursor(0,1);
lcd.print("Sound=");
lcd.print(sound, DEC);
lcd.print(" db");

if (mq135 < 140)
  {
    digitalWrite(redLed, LOW);
    digitalWrite(buzzer, LOW);
    
    
    Serial.print("AirQuality=");
      Serial.print(mq135, DEC);  // prints the value read
      Serial.println(" PPM(Normal Air Quality)");
     
  }
  else if(mq135>=140 && mq135<300)
  {
   digitalWrite(redLed, LOW);
    digitalWrite(buzzer, LOW);
    Serial.print("AirQuality=");
      Serial.print(mq135, DEC);  // prints the value read
      Serial.println(" PPM(Unhealthy Air Quality)");
 
  }
  else
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(buzzer, HIGH);
     Serial.print("AirQuality=");
     
      Serial.print(mq135, DEC);  // prints the value read
      Serial.println(" PPM(Bad Air Quality)");
      
  }
if(sound>70)
{
  Serial.print("SoundQuality=");
     
      Serial.print(sound, DEC);  // prints the value read
      Serial.println(" db (Noise level High)");
 }
else
{
  Serial.print("SoundQuality=");
     
      Serial.print(sound, DEC);  // prints the value read
      Serial.println(" db (Noise level Normal)");
 }
//sendTS(mq135);
delay(1000);    // wait 1000ms for next reading
}

void sendTS(int temp)
{  
   WiFiClient client;
  
   if (client.connect(server, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   //Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(temp);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }//end if
 
 client.stop();
}
