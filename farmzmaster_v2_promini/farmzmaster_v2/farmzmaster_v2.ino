 String SerialNumber= "2019090001
 "; 

#include <SPI.h>
#include <LoRa.h>
#include <EEPROM.h>
#include <Scheduler.h>
#include "MQ135.h"
#include "DHT.h"
  
#define ANALOGPIN A4
#define LED 13 

#define DHTPIN 5 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
  
 DHT dht(DHTPIN, DHTTYPE);
 String ver = "2.0";
 String BAND = "433";       // ตั้งค่า ยานความถี่รับส่งข้อมูล

 String eband = "";

 float airthreshold[3] = {400, 400, 800};
 int  modesensor = 0;
 int buttonState= 0;
 int i = 5;
 

  
MQ135 gasSensor = MQ135(ANALOGPIN);
  
void software_reboot(){
asm volatile ("  jmp 0");  
}  



void setup() {
  Serial.begin(115200);
  while (!Serial); 
  Serial.println("LoRa Sender"); 
  Serial.print("SerialNumber = ");
  Serial.println(SerialNumber);
  
  Serial.print("version = ");
  Serial.println(ver);  
     pinMode(LED, OUTPUT);
     pinMode(2, OUTPUT); 
     pinMode(9, OUTPUT);
     digitalWrite(2,1); 
     digitalWrite(9,0);     
 
   if (!LoRa.begin(BAND.toInt()*1000000)) {
       Serial.println("Starting LoRa failed!");        
        while (1);
      }
     Serial.println("Lora CONNECTED");  
     Serial.println("Lora Band "+BAND);     
    Scheduler.startLoop(setup1);
   }
 
 void loop() {  
 buttonState = digitalRead(9); 
 //Serial.println(buttonState);    
  if (buttonState == 0){     
    Serial.print(i);
    digitalWrite(LED,1); 
    delay(100); 
    digitalWrite(LED,0); 
    i--;    
  if (i < 1){
    Serial.println("SYS reset");    
    for (int i = 0; i < 20; ++i) { EEPROM.put(i, 0); }  
    delay(1000);
    software_reboot();
    }            
  }else {i = 5;}  
  delay(10);   
}


//update band
void setup1(){
onReceive(LoRa.parsePacket());  
}


 //mode sender
  void onReceive(int packetSize) {  
      
   int sensorValue = analogRead(A0);
   float outvoltage = sensorValue * (5.0 / 1023.0);
   int Level = 6*outvoltage;
   float ppm = gasSensor.getPPM();  
   float h = dht.readHumidity();
   float t = dht.readTemperature();       
   String Serialnum_mes ;
   if (isnan(t) || isnan(h)) {
    t = 0.00;
    h = 0.00;
  } 
  while(packetSize == 0) return;          // if there's no packet, return ถ้าไม่มีข้อมูลรีเทล
  byte byte_mes = LoRa.read();            // sender address  อ่านlora มาเก็บไว้ใน byte_mes
  String message = "";
  while (LoRa.available()) {
   message += (char)LoRa.read(); 
  }          
    String L1 = message.substring(message.indexOf("SN=") +3);
      Serialnum_mes = L1.substring(0,L1.indexOf(","));   
      Serial.print(": Serialnum_mes = "); 
      Serial.println(Serialnum_mes); 
             
  if (Serialnum_mes == SerialNumber){     
    LoRa.beginPacket();    
    LoRa.print("SERIAL=");
    LoRa.print(SerialNumber);
    LoRa.print(",");
    LoRa.print("Fiele1=");
    LoRa.print(h);   
    LoRa.print(",");
    LoRa.print("Fiele2=");
    LoRa.print(t);
    LoRa.print(",");
    LoRa.print("Fiele3=");
    LoRa.print(ppm);
    LoRa.print(",");
    LoRa.print("Fiele4=");
    LoRa.print("0.00");
    LoRa.print(",");                
    LoRa.endPacket();  

    Serial.print("SERIAL=");
    Serial.print(SerialNumber);
    Serial.print(",");
    Serial.print("Fiele1=");
    Serial.print(h);   
    Serial.print(",");
    Serial.print("Fiele2=");
    Serial.print(t);
    Serial.print(",");
    Serial.print("Fiele3=");
    Serial.print(ppm);
    Serial.print(",");
    Serial.print("Fiele4=");
    Serial.print("0.00");
    Serial.println(",");       
  }
 return;
}  


