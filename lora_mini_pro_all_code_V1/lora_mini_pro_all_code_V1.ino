#include <SPI.h>
#include <LoRa.h>
#include "EmonLib.h"
#include <EEPROM.h>
#include <Scheduler.h>
#include "MQ135.h"
#include "DHT.h"

  
#define ANALOGPIN A4
#define LED 13 

#define DHTPIN 5 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

 String BAND = "433";       // ตั้งค่า ยานความถี่รับส่งข้อมูล
 String SerialNumber= "";  
 String eband = "";

float airthreshold[3] = {
  400, 400, 800};
 int  modesensor = 0;
 int buttonState= 0;
 int i = 5;
 char HP ; 
 byte Address = 0x1;         // ตั้งค่า Address อุปกรณ์ตัวsensor
 byte code_update = 0x63; 
 byte code_confirm = 0x64;
 byte code_reboot = 0x65; 
  
MQ135 gasSensor = MQ135(ANALOGPIN);
EnergyMonitor emon1;
EnergyMonitor emon2;
EnergyMonitor emon3; 

  
void software_reboot(){
asm volatile ("  jmp 0");  
}  



void setup() {
  Serial.begin(115200);
  while (!Serial); 
  Serial.println("LoRa Sender");
  emon1.current(A0, 111.1);
  emon2.current(A1, 111.1);
  emon3.current(A2, 111.1); 
  Serial.print("device id ");  
  Serial.println(Address);     
     pinMode(LED, OUTPUT);
     pinMode(2, OUTPUT); 
     pinMode(9, OUTPUT);
     digitalWrite(2,1); 
     digitalWrite(9,0);
         
     Serial.println("Reading EEPROM Band");
      for (int i = 0; i < 3; ++i)
    {
      eband += char(EEPROM.read(i));
    }
   Serial.print("eband : ");
   Serial.println(eband);
   delay(100);      
    
          
     //check esid form rom.read
      String resid = "";
      resid = EEPROM.read(0);
      Serial.print("resid=");
      Serial.println(resid);
     
        
          //Serial.println("DHTxx test!");
     if ( resid.length() > 1 ) {           
        digitalWrite(LED, HIGH);
        delay(1000); 
        digitalWrite(LED, LOW);
        if (!LoRa.begin(eband.toInt()*1000000)) {
          Serial.println("Starting LoRa failed!");
         
          while (1);
          }
        Serial.println("Lora2 CONNECTED");  
        
        modesensor = digitalRead(2); 
    Serial.print("mode = "); 
    Serial.println(modesensor); 
       
   if(modesensor == 0 ){
       Scheduler.startLoop(setup2);
     }else{
       Scheduler.startLoop(setup3);
    }
                                                         
 } else {
  //update band
     digitalWrite(LED, HIGH);      
    if (!LoRa.begin(BAND.toInt()*1000000)) {
    Serial.println("Starting LoRa failed!");   
    delay(1000);
    software_reboot();
    while (1);
  }
   Serial.println("Lora1 CONNECTED");
   
  Scheduler.startLoop(setup1);
  
  
 }
  
}
 
 void loop() {  
 buttonState = digitalRead(9); 
 //Serial.println(buttonState);    
  if (buttonState == 0){     
    Serial.print(i);
    digitalWrite(LED,1); 
    delay(1000); 
    digitalWrite(LED,0); 
    i--;    
  if (i < 1){
    Serial.println("SYS reset");    
    for (int i = 0; i < 10; ++i) { EEPROM.put(i, 0); }  
    delay(1000);
    software_reboot();
   }
            
  }else {i = 5;}   

  delay(10);   
}

void setup1(){
onReceive(LoRa.parsePacket());
  delay(10);
}


void setup2(){
onReceive1(LoRa.parsePacket()); 
  delay(10);
}

void setup3(){
onReceive2(LoRa.parsePacket()); 
  delay(10);
}



 void onReceive1(int packetSize) {  
  while(packetSize == 0) return;          // if there's no packet, return ถ้าไม่มีข้อมูลรีเทล
  Serial.print("RUN");
  byte handshake = LoRa.read();            // sender address  อ่านlora มาเก็บไว้ใน handshake
  String incoming = "";
  while (LoRa.available()) {
   incoming += (char)LoRa.read(); 
  }  
  if (handshake == Address){
  
         double Irms1 = emon1.calcIrms(1480);          
         double Irms2 = emon2.calcIrms(1480);         
         double Irms3 = emon3.calcIrms(1480);   
               
        LoRa.beginPacket();    
         // LoRa.println(counter);
          LoRa.write(Address);              // add destination address
        
          LoRa.print("SERIAL=");
          LoRa.print(SerialNumber);
          LoRa.print(",");                  
          LoRa.print("Current1=");
          LoRa.print(Irms1);
          LoRa.print(",");
          LoRa.print("Current2=");
          LoRa.print(Irms2);
          LoRa.print(",");
          LoRa.print("Current3=");
          LoRa.print(Irms3);
          LoRa.print(",");
          LoRa.print("Current4=");
          LoRa.print("🔛");
          LoRa.print(",");         
          LoRa.endPacket();
          Serial.println("sended");
          
          Serial.print("SERIAL=");
           Serial.println(SerialNumber);
           Serial.print("data1=");
           Serial.println(Irms1);
           Serial.print("data2=");
           Serial.println(Irms2);
           Serial.print("data3=");
           Serial.println(Irms3);
           
          
          Serial.println(incoming);
    }
    
    if (handshake == code_update) {  
          LoRa.beginPacket();          
          LoRa.write(Address);  
          LoRa.print("reset_ok");
          LoRa.endPacket();
          Serial.println("sended");
          delay(1000); 
                         
      Serial.println("SYS reset");    
      for (int i = 0; i < 10; ++i) { EEPROM.put(i, 0); }  
      delay(1000);
      software_reboot();
  }
}  


  void onReceive2(int packetSize) {
    
   int sensorValue = analogRead(A0);
   float outvoltage = sensorValue * (5.0 / 1023.0);
   int Level = 6*outvoltage;
   float ppm = gasSensor.getPPM();  
   float h = dht.readHumidity();
   float t = dht.readTemperature();       

   
  while(packetSize == 0) return;          // if there's no packet, return ถ้าไม่มีข้อมูลรีเทล
  byte handshake = LoRa.read();            // sender address  อ่านlora มาเก็บไว้ใน handshake
  String incoming = "";
  while (LoRa.available()) {
   incoming += (char)LoRa.read(); 
  }             
  if (handshake == Address){     
    LoRa.beginPacket();
    LoRa.write(Address);    
    LoRa.print("SERIAL=");
    LoRa.print(SerialNumber);
    LoRa.print(",");
    LoRa.print("Current1=");         
    LoRa.print(ppm);
    LoRa.print(",");
    LoRa.print("Current2=");
    LoRa.print(h);
    LoRa.print(",");
    LoRa.print("Current3=");
    LoRa.print(t);
    LoRa.print(",");
    LoRa.print("Current4=");
    LoRa.print(outvoltage);
    LoRa.print(",");                
    LoRa.endPacket();  

           Serial.print("SERIAL=");
           Serial.println(SerialNumber);
           Serial.print("data1=");
           Serial.println(ppm);
           Serial.print("data2=");
           Serial.println(h);
           Serial.print("data3=");
           Serial.println(t);
           Serial.print("data4=");
           Serial.println(outvoltage);
          
          Serial.println(incoming);
  }
  
    if (handshake == code_update) {  
          LoRa.beginPacket();          
          LoRa.write(Address);  
          LoRa.print("reset_ok");
          LoRa.endPacket();
          Serial.println("sended");
          delay(1000); 
                         
      Serial.println("SYS reset");    
      for (int i = 0; i < 10; ++i) { EEPROM.put(i, 0); }  
      delay(1000);
      software_reboot();
  }
}  
/*..................................................read band to update.....*/
 void onReceive(int packetSize) {
  while(packetSize == 0) return;          // if there's no packet, return ถ้าไม่มีข้อมูลรีเทล
 
  //byte address = LoRa.peek();            // sender address  อ่านlora มาเก็บไว้ใน handshake
  //byte handshake = LoRa.peek();
  byte handshake = LoRa.read();
  byte address = LoRa.peek(); 
  String incoming = "";
  int updatband_delay = 0;
  while (LoRa.available()) {
   incoming += (char)LoRa.read(); 
  } 
// Serial.print("handshake ="); 
// Serial.println(handshake);
// Serial.print("address =");
// Serial.println(address);
   
   if (handshake == Address) {
      Serial.println("handshake == Address");  
      LoRa.beginPacket();                  
      LoRa.write(code_update);  
      LoRa.write(Address); 
      //LoRa.print("ok");                    
      LoRa.endPacket();       
  }  
  
    if (handshake == code_update) {
      Serial.println(address); 
      Serial.println("handshake == code_update");
             LoRa.beginPacket();
             LoRa.write(code_update);  
             LoRa.write(code_confirm);    
             //LoRa.print("ok");
             LoRa.endPacket();   
          delay(100);
      Serial.print("check incoming :");       
      Serial.println(incoming);   
          
          if (incoming.toInt() > 1){  
      Serial.print("check incoming =");       
      Serial.println(incoming);                  
          for (int i = 0; i < 3; ++i)
            {
              EEPROM.put(i, incoming[i]);   //eeprom เขียน  ชื่อไวไฟ
             Serial.print("Wrote: ");
             Serial.println(incoming[i]); 
             delay(10);
            } 
            software_reboot();             
           }
            delay(100);               
   }       
 }
