    #if CONFIG_FREERTOS_UNICORE
    #define ARDUINO_RUNNING_CORE 0
    #else
    #define ARDUINO_RUNNING_CORE 1
    #endif



    #include "ThingSpeak.h"
    #include "secrets.h"
    #include <EEPROM.h>
    #include <SPI.h>
    #include <LoRa.h>
    #include <U8x8lib.h>
    #include <ArduinoOTA.h>
    #include <Update.h>
    #include <WiFi.h>
    #include <WiFiClient.h>
    #include <WebServer.h>
    #include <DNSServer.h>
    #include <ESPmDNS.h>
    #include <ESP32httpUpdate.h>
      WebServer server(80);
      
    #define LED 2     
    #define SS      18
    #define RST     14
    #define DI0     26
    
    String BAND = "433"; 
    String miniproreboot = "reboot"; 
    const char* host = "esp32-webupdate";
    const char* ssid = "FARMZ-🐥";
    const char* pass = "admin123";
    const char* update_path = "/update";
    const char* www_username = "root";
    const char* www_password = "password";
    char B[16];

    
    int checkd1 = 0 ;
    int checkd2 = 0 ;
    int checkd3 = 0 ;
    int checkd4 = 0 ;
    int checkd5 = 0 ;
    int checkd6 = 0 ;
    
    int checko1  ;
    int checko2  ;
    int checko3  ;
    int checko4  ;
    int checko5  ;
    int checko6  ;
    
    int sendsta = 2 ;
    int delay1= 9000; 
    int delay2= 5000;  
    int t;
    uint8_t c, r, d;
     
    int lora_rssi = 0;
    int Time = 0;
    int statusCode;
    int i = 4;
    int buttonState= 0;
    
    unsigned long ShowTime;
    long currentmillis=0;
    long days=0;
    long hours=0;
    long mins=0;
    long secs=0; 

    
    byte code_reboot = 0x65;
    byte code_confirm = 0x64;     
    byte code_update = 0x63;    
    byte msgCount = 0;            // count of outgoing messages
    byte localAddress = 0xFF;     // address of this device  ที่อยู่อุปกรณ์
    byte destination1 = 0x1;      // destination to send to
    byte destination2 = 0x2;      // destination to send to
    byte destination3 = 0x3;      // destination to send to
    byte destination4 = 0x4;      // destination to send to
    byte destination5 = 0x5;      // destination to send to
    byte destination6 = 0x6; 
    byte ok1 = 0x64; 
    byte ok_byte = 0x65; 
    byte check_code;
    byte check_code2 ;
    byte sender ;
    byte address;

    String band1 = "400";
    String band2 = "405";
    String band3 = "410";
    String band4 = "415";
    String band5 = "420";
    String band6 = "425";
    String band7 = "430";
    String band8 = "433";
    String band9 = "440";
    
    String outgoing;              // outgoing message
    String st;
    String content;
    String configwifi;
    String update;
    String reboot;
    String reconfig; 
    String p_status;
    String b_status;
     
    String esid = "" ;
    String epass = "" ;
    String edevice ;
    String eband = "";
    String apikey ;  
    String qband ;   

    String SerialNum = "";
    String data1 = "";
    String data2 = "";
    String data3 = "";
    String data4 = "";
    
    
   String eapikey1 ;
   String eapikey2 ;
   String eapikey3 ;
   String eapikey4 ;
   String eapikey5 ;
   String eapikey6 ;
   
   String myStatus = "";
   String eFarmzName ="";
   String statusip = ""; 
   unsigned long myChannelNumber = 1; 


    String SerialNumber= "201909001"; 
    
  WiFiClient  client; 
  
// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);   

void lorabegin(String BAND) {
   if (!LoRa.begin(BAND.toInt()*1000000)) {
    Serial.println("Starting LoRa failed!");
    delay(1000);
    ESP.restart();
    while (1);
    }
   Serial.println("Lora CONNECTED");
   Serial.println(BAND);
  }

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512); 
  MDNS.addService("http", "tcp", 80);  
  pinMode(22,INPUT);
  pinMode(23,OUTPUT);
  pinMode(LED, OUTPUT);
  
  digitalWrite(23, HIGH);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  //.................................................//OLED/config
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  fillDisplay();
  u8x8.setCursor(0,0);
  u8x8.setInverseFont(1);//คล่มสี
  u8x8.println("Booting>>>>>");
  Serial.println("Running Time");
  Serial.println("------------");  
 //..................................................//lora connect 
  Serial.println("configpin Lora");
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);
  

  
  // read eeprom for ssid and pass
  delay(100);
  Serial.println("Reading EEPROM ssid");  
  for (int i = 0; i < 24; ++i)
    {
      esid += char(EEPROM.read(i));
    }
  Serial.print("SSID: ");
  Serial.println(esid);
  
  delay(100);
  Serial.println("Reading EEPROM pass");  
  for (int i = 24; i < 42; ++i)
    {
      epass += char(EEPROM.read(i));
    }
  Serial.print("PASS: ");
  Serial.println(epass);  
  
  delay(100);
  Serial.println("Reading EEPROM device");     
  for (int i = 42; i < 43; ++i)
    {
      edevice += char(EEPROM.read(i));
    }
   Serial.print("edevice : ");
   Serial.println(edevice);
   
 delay(100);
  Serial.println("Reading EEPROM Band");
  for (int i = 44; i < 47; ++i)
    {
      eband += char(EEPROM.read(i));
    }
   Serial.print("eband : ");
   Serial.println(eband);
   /*.................................................apikey*/
  Serial.println("Reading EEPROM apikey1");
  for (int i = 47; i < 64; ++i)
    {
      eapikey1 += char(EEPROM.read(i));
    }
   Serial.print("apikey1 : ");
   Serial.println(eapikey1);
   
  Serial.println("Reading EEPROM apikey2");
  for (int i = 64; i < 81; ++i)
    {
      eapikey2 += char(EEPROM.read(i));
    }
   Serial.print("apikey2 : ");
   Serial.println(eapikey2);
   
  Serial.println("Reading EEPROM apikey3");
  for (int i = 81; i < 98; ++i)
    {
      eapikey3 += char(EEPROM.read(i));
    }
   Serial.print("apikey1 : ");
   Serial.println(eapikey3); 
   
  Serial.println("Reading EEPROM apikey4");
  for (int i = 98; i < 115; ++i)
    {
      eapikey4 += char(EEPROM.read(i));
    }
   Serial.print("apikey4 : ");
   Serial.println(eapikey4);  
   
  Serial.println("Reading EEPROM apikey5");
  for (int i = 115; i < 132; ++i)
    {
      eapikey5 += char(EEPROM.read(i));
    }
   Serial.print("apikey1 : ");
   Serial.println(eapikey5);
   
  Serial.println("Reading EEPROM apikey6");
  for (int i = 132; i < 149; ++i)
    {
      eapikey6 += char(EEPROM.read(i));
    }
   Serial.print("apikey1 : ");
   Serial.println(eapikey6);


  Serial.println("Reading EEPROM Band");
  for (int i = 149; i < 165; ++i)
    {
      eFarmzName += char(EEPROM.read(i));
    }

 delay(100);
//check esid form rom.read
      String resid = "";
      resid = EEPROM.read(0);
      Serial.print("resid=");
      Serial.println(resid);
    
   
  if ( resid.length() > 1 ) {   //ถาม esid มีค่ามากกว่า 1 
      Serial.println(esid.length());
      Serial.println(esid.c_str());
      ThingSpeak.begin(client); 
      WiFi.begin(esid.c_str(), epass.c_str());  
      if (testWifi()) {
        launchWeb(0);       //เรียกใช้ ฟั่งซั่น  กำหนดตัวแปล เป็น 0         
        lorabegin(eband);
//...................................................//lora connect 
        delay(100); 
        xTaskCreatePinnedToCore(loop1, "loop1", 4096, NULL, 2, NULL, ARDUINO_RUNNING_CORE);        //เรียกใช้ loop1   
        return;
      } 
   }  else {
   //u8x8.setFlipMode(0);//กลับหัว      
  // u8x8.setCursor(0,3); 
 //u8x8.println("not.connect");
  u8x8.setInverseFont(0);//คล่มสี
  u8x8.println("config wifi");   
  u8x8.print("N="); 
  u8x8.setInverseFont(1);
  u8x8.println(ssid); 
  u8x8.setInverseFont(0);
  u8x8.print("P=");  
  u8x8.setInverseFont(1);
  u8x8.println(pass);
  u8x8.setInverseFont(0);
   
    lorabegin(BAND);
  setupAP(); 
  delay(1000);
  xTaskCreatePinnedToCore(loop2, "loop2", 4096, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
   
  }  //end else  
 } // end setup void
   



bool testWifi(void) {    //เช็คสถาณไวไฟ และการทำงานไวไฟ boolซึ่งเป็นตัวแปรทางลอจิก มีค่าเป็น True (1) หรือ False (0) ได้เท่านั้น
    int c = 20;
    Serial.println("Waiting for Wifi to connect");  
    while ( c > 1 ) { 
      u8x8.setInverseFont(0);//คล่มสี
      u8x8.println("WIFI"); 
      u8x8.println("connecting..");   
    if (WiFi.status() == WL_CONNECTED) {
      u8x8.clear(); 
      u8x8.println("WIFI"); 
      u8x8.println("CONNECTED"); 
      u8x8.setInverseFont(1);//คล่มสี
      u8x8.println(WiFi.localIP());
      u8x8.setInverseFont(0);//คล่มสี
      delay(1000); 
      return true;              //ให้ค่าเป็นจริง
    } 
    delay(500);
    Serial.print("wifi :"); 
    Serial.println(WiFi.status()); 
     
    c--;   //ลดลง
   
   Serial.print("startconfig :");   
   Serial.print( c );  
   Serial.println (" second");    

  }
  Serial.println("");
  Serial.println("");
  //ESP.restart();
  return true;                  //ให้ค่าเป็นจริง
 //return false;   //ให้ค่าาเป็นเท็จ
  
} 

void launchWeb(int webtype) {   
  Serial.println("");
  Serial.println("Start WiFi");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  
  createWebServer(webtype);//เรียกใช้ฟั่งซั่น  webserver createWebServer
  
  // Start the server
 
  server.begin();
  Serial.println("Server started"); 
}

void setupAP(void) {    //config wifi 
  WiFi.mode(WIFI_STA);   //ตั้งให้เป็น โหมด station
  WiFi.disconnect();     //ปิด wifi
  delay(100);
  int n = WiFi.scanNetworks();  //แสกนไวไฟ
  Serial.println("scan done");   //แสดง
  if (n == 0)                             // ถ้าไม่มีไวไฟ 
    Serial.println("no networks found");  //แสดง
  else
  {
    Serial.print(n);                     //แสดงจำนวนไวไฟ       
    Serial.println(" networks found");    //แสดง
    for (int i = 0; i < n; ++i)           //กำหนดให้ iเป็น0  ถ้า iมากกว่า n ให้บวก ขึ้น  
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);     //แสดง บวก1
      Serial.print(": ");      //แสดง
      Serial.print(WiFi.SSID(i));    //แสดง ชื้อ ไวไฟในตัวแปล i
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));    //แสดงค่าสัญญานที่ได้ ในตัวแปล i
      Serial.print(")");
      #ifdef ESP8266    
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");      //ตรวจสอบว่าเป็นไวไฟมีรหัสหรือไม่ ถ้ามี ให้เพิ่ม  " * "
      #else  //esp32
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");      //ตรวจสอบว่าเป็นไวไฟมีรหัสหรือไม่ ถ้ามี ให้เพิ่ม  " * "
      #endif
      delay(10);
     }
  }
  Serial.println("");  
  delay(100);
  WiFi.softAP(ssid, pass, 6);   //ตั้งไวไฟ  ชื่อ รหัสผ่าน และ  ช่องศัญญาน
  Serial.println("configwifi");     //แสดง
  launchWeb(1);                    //เรียกใช้ ฟั่งซั่น แสดงข้อมูล ไวไฟ   //เรียกใช้ ฟั่งซั่น  กำหนดตัวแปล เป็น 1

}

void createWebServer(int webtype){
  if ( webtype == 1 ) {      //ตัวแปลเป็น 1 ให้ทำงาน
    server.on("/", []() {     //ถ้าเรียก server ./.

        
        configwifi = "<!DOCTYPE html>";        
        configwifi += "<html lang='en'><head>"; 
        configwifi += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
        configwifi += "<title>FARMZ_MASTER</title>";
        
        configwifi += "<style>";
        configwifi += ".c{text-align: center;}";
        configwifi += "div,input{padding:5px;font-size:1em;}";
        configwifi += "input{background-color:hsl(0, 0%, 100%) width:95%;}";
        configwifi += "button{border:0;border-radius:0.3rem;background-color:hsl(0, 0%, 24%);color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;}";
        configwifi += "body{text-align: center;font-family:verdana;}";
        configwifi += ".l{background: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==') no-repeat left center;background-size: 1em;}";
        configwifi += ".q{float: right;width: 64px;text-align: right;}";
        
        configwifi += ".topnav {overflow: hidden;background-color: #333;}";
        configwifi += ".topnav a {float: left;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
        configwifi += ".topnav a:hover {background-color: #ddd;color: black;}";
        configwifi += ".topnav a.active {background-color: #4CAF50;color: white;}";
        configwifi += "</style>";
        
        configwifi += "<script>";
        configwifi += "function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}";
        configwifi += "</script>";
        
        configwifi += "</head>";
        configwifi += "<body style='background-color:#b4b4b4;'>";
        
        configwifi += "<div class='topnav'>";   
        configwifi += "<a class='active'href='/'>Config</a>";   
        configwifi += "<a href='/status'>Status</a>";      
        configwifi += "<a href='/reboot'>Reboot</a>"; 
        configwifi += "<a href='http://www.zhotspot.com'>About</a>"; 
        configwifi += "</div><br/>";  
          
        configwifi += "<div style='text-align:left;display:inline-block;min-width:260px;'>";      
        configwifi += "<h2>Setup</h2>";   
         
      IPAddress ip = WiFi.softAPIP();    
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);          
      int n = WiFi.scanNetworks();
    st = "<ul>";           
      for (int i = 0; i < n; ++i)       
      {      
      st += "<li>"; 
      String v = WiFi.SSID(i);
       
      st += "<div><a href='#p' onclick='c(this)'>";                 
      st += v; 
      st += "</a> ";  
      st += " (";
      st += WiFi.RSSI(i);      
       #ifdef ESP8266     
      st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
       #else
      st += ((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
       #endif
      st += ")<span class='q'></span></div>";
      st += "</li>";
       }
      st += "</ul>";   
        
        configwifi += ipStr;      
        configwifi += "<p>";  
        configwifi += st;    
        configwifi += "</p>";     
                   
        configwifi += "<form method='get' action='setting'>"; 
        
        configwifi += "<label>SSID: </label><input id='s' name='ssid' length=20   placeholder='SSID'><br/><br>";     
        configwifi += "<label>PASS: </label><input id='p' name='pass' length=20 type='password' placeholder='password'><br/>";
        
        configwifi += "<br> NAME : <input type='text'name='farmname' placeholder='farmname'><br><br>";              
        configwifi += "<label>Device1: </label><input name='device1'length=17 placeholder='API Thingspeak'><br><br>";
        configwifi += "<label>Device2: </label><input name='device2'length=17 placeholder='API Thingspeak'><br><br>";
        configwifi += "<label>Device3: </label><input name='device3'length=17 placeholder='API Thingspeak'><br><br>";
        configwifi += "<label>Device4: </label><input name='device4'length=17 placeholder='API Thingspeak'><br><br>";
        configwifi += "<label>Device5: </label><input name='device5'length=17 placeholder='API Thingspeak'><br><br>";
        configwifi += "<label>Device6: </label><input name='device6'length=17 placeholder='API Thingspeak'><br><br>";

        configwifi += "LoRa Band<br>";
        configwifi += "<br><label>BAND : </label><select name='band'> <br />";
        configwifi += "<option value='407'>407</option>";
        configwifi += "<option value='415'>415</option>";
        configwifi += "<option value='420'>420</option>";        
        configwifi += "<option value='425'>425</option>";        
        configwifi += "<option value='427'>427</option>";        
        configwifi += "<option value='433'>433</option>";
        configwifi += "<option value='439'>439</option>";
        configwifi += "<option value='443'>443</option>";
        
        configwifi += "</select><br />";
        
        configwifi += "<br/><button type='submit'>save</button></form>";   
        configwifi += "<br/><div class=\"c\"><a href=\"/\">Scan</a></div>";
        
        configwifi += "</div>";
        configwifi += "</body>";
        configwifi += "</html>";  
       server.send(200, "text/html", configwifi);  //ส่งข้อมูลไปที่ server
    });

    
    server.on("/setting", []() {      
     int sband ;
     int sdevice;
     
     String FarmzName = server.arg("farmname");
     String qsid = server.arg("ssid");
     String qpass = server.arg("pass");
     String qdevice = server.arg("device");
     String qband = server.arg("band");
     String device1 = server.arg("device1");
     String device2 = server.arg("device2");
     String device3 = server.arg("device3");
     String device4 = server.arg("device4");
     String device5 = server.arg("device5");
     String device6 = server.arg("device6");
     String cdevice= "" ;
        if (qsid.length() > 0 && qpass.length() > 0 ){   
          Serial.println("clearing eeprom");
          for (int i = 0; i < 86; ++i) { EEPROM.put(i, 0); } 
          Serial.print("qsid =");
          Serial.println(qsid);  
          Serial.print("qpass =");
          Serial.println(qpass);      
          Serial.print("qdevice =");
          Serial.println(qdevice);      
          Serial.print("qband =");
          Serial.println(qband); 
          Serial.print("device1 =");
          Serial.println(device1);
          Serial.print("device2 =");
          Serial.println(device2); 
          Serial.print("device3 =");
          Serial.println(device3); 
          Serial.print("device4 =");
          Serial.println(device4); 
          Serial.print("device5 =");
          Serial.println(device5); 
          Serial.print("device6 =");
          Serial.println(device6);  
              delay(100);
          Serial.println("writing eeprom ssid:");          
          for (int i = 0; i < qsid.length(); ++i)
            {
              EEPROM.put(i, qsid[i]);   //eeprom เขียน  ชื่อไวไฟ
              Serial.print("Wrote: ");
              Serial.println(qsid[i]); 
            }
            delay(100);
          Serial.println("writing eeprom pass:"); 
          for (int i = 0; i < qpass.length(); ++i)
            {
              EEPROM.put(24+i, qpass[i]);
              Serial.print("Wrote: ");
              Serial.println(qpass[i]); 
            }    
 //           delay(100);
 //         Serial.println("writing eeprom device:"); 
 //         for (int i = 0; i < 1 ; ++i)
 //           {
 //            EEPROM.put(42+i, qdevice[i]);
 //             Serial.print("Wrote: ");
 //             Serial.println(qdevice[i]); 
 //           }    
            delay(100);
          Serial.println("writing eeprom band:"); 
          for (int i = 0; i < 3 ; ++i)
            {
              EEPROM.put(44+i, qband[i]);
              Serial.print("Wrote: ");
              Serial.println(qband[i]); 
           } 
/*..............................................apikey..*/
          if (device6.length() > 1 ){
               cdevice = "6";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           } 
              Serial.println("writing eeprom device2:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(64+i, device2[i]);
              Serial.print("Wrote: ");
              Serial.println(device2[i]); 
           } 
              Serial.println("writing eeprom device3:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(81+i, device3[i]);
              Serial.print("Wrote: ");
              Serial.println(device3[i]); 
           }
             Serial.println("writing eeprom device4:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(98+i, device4[i]);
              Serial.print("Wrote: ");
              Serial.println(device4[i]); 
           }
             Serial.println("writing eeprom device5:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(115+i, device5[i]);
              Serial.print("Wrote: ");
              Serial.println(device5[i]); 
           }
             Serial.println("writing eeprom device6:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(132+i, device6[i]);
              Serial.print("Wrote: ");
              Serial.println(device6[i]); 
           }     
                    
          } else if (device5.length() > 1 ){
              cdevice = "5";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           } 
              Serial.println("writing eeprom device2:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(64+i, device2[i]);
              Serial.print("Wrote: ");
              Serial.println(device2[i]); 
           } 
              Serial.println("writing eeprom device3:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(81+i, device3[i]);
              Serial.print("Wrote: ");
              Serial.println(device3[i]); 
           }
             Serial.println("writing eeprom device4:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(98+i, device4[i]);
              Serial.print("Wrote: ");
              Serial.println(device4[i]); 
           }
             Serial.println("writing eeprom device5:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(115+i, device5[i]);
              Serial.print("Wrote: ");
              Serial.println(device5[i]); 
           }
           
          } else if (device4.length() > 1 ){
              cdevice = "4";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           } 
              Serial.println("writing eeprom device2:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(64+i, device2[i]);
              Serial.print("Wrote: ");
              Serial.println(device2[i]); 
           } 
              Serial.println("writing eeprom device3:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(81+i, device3[i]);
              Serial.print("Wrote: ");
              Serial.println(device3[i]); 
           }
             Serial.println("writing eeprom device4:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(98+i, device4[i]);
              Serial.print("Wrote: ");
              Serial.println(device4[i]); 
           }
          }else if (device3.length() > 1 ){
              cdevice = "3";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           } 
              Serial.println("writing eeprom device2:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(64+i, device2[i]);
              Serial.print("Wrote: ");
              Serial.println(device2[i]); 
           } 
              Serial.println("writing eeprom device3:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(81+i, device3[i]);
              Serial.print("Wrote: ");
              Serial.println(device3[i]); 
           }
          }else if (device2.length() > 1 ){
              cdevice = "2";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           } 
              Serial.println("writing eeprom device2:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(64+i, device2[i]);
              Serial.print("Wrote: ");
              Serial.println(device2[i]); 
           }
          } else if (device1.length() > 1 ){
              cdevice = "1";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           }
          }


          
          Serial.println("writing eeprom device:"); 
          for (int i = 0; i < 1 ; ++i)
            {
             EEPROM.put(42+i, cdevice[i]);
              Serial.print("Wrote: ");
              Serial.println(cdevice[i]); 
            }            
        Serial.println(cdevice); 

        Serial.println("writing eeprom Farmname:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(149+i, FarmzName[i]); //to 164 ตัวอักษร
              Serial.print("Wrote: ");
              Serial.println(FarmzName[i]);  
           } 


        
                    
      EEPROM.commit();       
      u8x8.setInverseFont(1);//คล่มสี
      u8x8.println("saved config");
      u8x8.setInverseFont(0);//คล่มสี            

          content = " <html>";
          content += " <head>";
          content += "<title>FARMZ_MASTER</title>";
          content += "<meta http-equiv= 'refresh' content='3; url=/status'/>"; 
          content += " </head>";
          content += "<body>";
          content += "reboot esp32 to save config ";
          content += "</body>";
          content += "</html>";           
        
          statusCode = 200;
        } else {
          content = "{\"Error\":\"404 not found\"}";
          statusCode = 404;
          Serial.println("Sending 404");
        }
        server.send(statusCode, "text/html", content);
        delay(10000);
 //        #ifdef ESP8266
 //      ESP.reset();
 //        #else
 //      ESP.restart();
 //        #endif         
    });
    
    server.on("/reboot",HTTP_GET, []() {          
      reboot = "<!DOCTYPE html>";
      reboot += "<html><head>"; 
      reboot += "<title>FARMZ_MASTER</title>";
      reboot += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
      reboot += "<style>";
      reboot += "body{  margin: 0; font-family: Arial, Helvetica, sans-serif;}";
      reboot += ".topnav {overflow: hidden;background-color: #333;}";
      reboot += ".topnav a {float: left;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
      reboot += ".topnav a:hover {background-color: #ddd;color: black;}";
      reboot += ".topnav a.active {background-color: #4CAF50;color: white;}";
      reboot += "</style>";
      reboot += "</head>";
      reboot += "<body>";
      reboot += "<div class='topnav'>";   
      reboot += "<a href='/'>Config</a>";   
      reboot += "<a href='/status'>Status</a>";      
      reboot += "<a class='active'href='/reboot'>Reboot</a>"; 
      reboot += "<a href='http://www.zhotspot.com'>About</a>"; 
      reboot += "</div>";    
      reboot += "<div style='padding-left:16px'>";
      reboot += "<h2>REBOOT</h2>";
      reboot += "<p>Restart system Ok!</p>";
      reboot += "<form method='POST' action='/reboot'><input type='submit'value='YES'></form>";
      reboot += "</div>";
      reboot += "</body>";
      reboot += "</html>";         
      server.sendHeader("Connection", "close"); 
      server.send(200, "text/html", reboot);  
     });

     server.on("/status",HTTP_GET, []() {          
      p_status = "<!DOCTYPE html>";
      p_status += "<html><head>"; 
      p_status += "<title>FARMZ_MASTER</title>";
      p_status += "<meta http-equiv= 'refresh' content='5; url=/status'/>"; 
      p_status += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
      p_status += "<style>";
      p_status += "body{  margin: 0; font-family: Arial, Helvetica, sans-serif;}";
      p_status += ".topnav {overflow: hidden;background-color: #333;}";
      p_status += ".topnav a {float: left;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
      p_status += ".topnav a:hover {background-color: #ddd;color: black;}";
      p_status += ".topnav a.active {background-color: #4CAF50;color: white;}";
      p_status += "</style>";
      p_status += "</head>";
      p_status += "<body>";
      p_status += "<div class='topnav'>";       
      p_status += "<a href='/'>Config</a>";   
      p_status += "<a class='active'href='/'>status</a>";      
      p_status += "<a href='/reboot'>Reboot</a>"; 
      p_status += "<a href='http://www.zhotspot.com'>About</a>"; 
      
      p_status += "</div>";    
      
      p_status += "<div style='padding-left:16px'>";
      p_status += "<h2>Status</h2>";
      p_status += "<p></p>";
      
       if (checkd1 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 1 : Online</FONT>";   //<FONT COLOR="Green">Computer</FONT> 
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 1 : Offline </FONT>";   //<FONT COLOR="Red">Computer</FONT> 
     
      
      p_status += "<p></p>"; 
      }
      if (checkd2 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 2 : Online</FONT>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 2 : Offline </FONT>";
      p_status += "<p></p>"; 
      }
      if (checkd3 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 3 : Online</FONT>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 3 : Offline </FONT>";
      p_status += "<p></p>"; 
      }
      if (checkd4 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 4 : Online</FONT>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 4 : Offline </FONT>";
      p_status += "<p></p>"; 
      }
       if (checkd5 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 5 : Online</FONT>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 5 : Offline </FONT>";
      p_status += "<p></p>"; 
      }
      if (checkd6 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 6 : Online</FONT>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 6 : Offline </FONT>";
      p_status += "<p></p>"; 
      }
      

      if (check_code == 99){
        p_status += "<p></p>";
        p_status += "ready to update band";
      }else {
        p_status += "<p></p>";
        p_status += "!ready to update band";
      }
        
      p_status += "<p></p>";
    
//      p_status += "<p>reboot</p>";     
      p_status += "<form method='POST' action='/reboot'><input type='submit'value='Reboot'></form>";
      p_status += "</div>";
      p_status += "</body>";
      p_status += "</html>";       
      server.sendHeader("Connection", "close"); 
      server.send(200, "text/html", p_status);  
     });
     
     server.on("/reboot",HTTP_POST, []() {      
      reboot = " <html>";
      reboot += " <head>";
      reboot += " <title>reboot</title>";
      reboot += "<meta http-equiv= 'refresh' content='3; url=/'/>"; 
      reboot += " </head>";
      reboot += "<body>";
      reboot += "reboot in 5 s";
      reboot += "</body>";
      reboot += "</html>"; 
      
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", reboot);        
       delay(1000);  
         #ifdef ESP8266
       ESP.reset();
         #else
       ESP.restart();
         #endif
    });
    /*.................................................................server on in locla ip...*/
  } else if (webtype == 0) {        
      server.on("/",HTTP_GET, []() { 
        if(!server.authenticate(www_username, www_password))
        return server.requestAuthentication();         
      p_status = "<!DOCTYPE html>";
      p_status += "<html><head>"; 
      p_status += "<title>FARMZ_MASTER</title>";
      p_status += "<meta http-equiv= 'refresh' content='5; url=/'/>"; 
      p_status += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
      p_status += "<style> table, th, td { border: 1px solid black;}";     
      p_status += "body{  margin: 0; font-family: Arial, Helvetica, sans-serif;}";
      p_status += ".topnav {overflow: hidden;background-color: #333;}";
      p_status += ".topnav a {float: left;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
      p_status += ".topnav a:hover {background-color: #ddd;color: black;}";
      p_status += ".topnav a.active {background-color: #4CAF50;color: white;}";
      p_status += "</style>";
      p_status += "</head>";
      p_status += "<body>";
      p_status += "<div class='topnav'>";       
      p_status += "<a class='active'href='/'>Home</a>"; 
      p_status += "<a href='/config'>Config</a>"; 
      p_status += "<a href='/update'>Update</a>"; 
      p_status += "<a href='/reboot'>Reboot</a>"; 
      p_status += "<a href='/reset'>Reset Config</a>"; 
      p_status += "<a href='http://www.zhotspot.com'>About</a>"; 
      p_status += "</div>";    
      
      p_status += "<div style='padding-left:16px'>";
      p_status += "<h2>Status</h2>";
      p_status += "<B>Band :</B>";
      p_status += eband;
      p_status += "<p></p>";
       if (checkd1 == 1){
      p_status += "Sensor 1 :<span style='background-color:#3cb371'>Online</span>";   //<FONT COLOR="Green">Computer</FONT> 
      p_status += "<p></p>"; 
      
      }else {
      p_status += "Sensor 1 :<span style='background-color:#ff0000'>Offline </span>";   //<span style="background-color:#3cb371"> Online</span>
      p_status += "&nbsp;";
      p_status += hours; 
      p_status +=  "h "; 
      p_status += mins;
      p_status +=  "m "; 
      p_status += mins;
      p_status +=  "s ";
      p_status += "&nbsp;";
      p_status += "last time..";
      p_status += "<p></p>"; 
      }
      if (checkd2 == 1){
      p_status += "Sensor 2 :<span style='background-color:#3cb371'>Online</span>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "Sensor 2 :<span style='background-color:#ff0000'>Offline </span>";
      p_status += "&nbsp;";
      p_status += hours; 
      p_status +=  "h "; 
      p_status += mins;
      p_status +=  "m "; 
      p_status += mins;
      p_status +=  "s ";
      p_status += "&nbsp;";
      p_status += "last time..";
      p_status += "<p></p>"; 
      
      }
      if (checkd3 == 1){
      p_status += "Sensor 3 :<span style='background-color:#3cb371'>Online</span>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "Sensor 3 :<span style='background-color:#ff0000'>Offline </span>";
      p_status += "&nbsp;";
      p_status += hours; 
      p_status +=  "h "; 
      p_status += mins;
      p_status +=  "m "; 
      p_status += mins;
      p_status +=  "s ";
      p_status += "&nbsp;";
      p_status += "last time..";
      p_status += "<p></p>"; 
      
      }
      if (checkd4 == 1){
      p_status += "Sensor 4 :<span style='background-color:#3cb371'>Online</span>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "Sensor 4 :<span style='background-color:#ff0000'>Offline </span>";
      p_status += "&nbsp;";
      p_status += hours; 
      p_status +=  "h "; 
      p_status += mins;
      p_status +=  "m "; 
      p_status += mins;
      p_status +=  "s ";
      p_status += "&nbsp;";
      p_status += "last time..";
      p_status += "<p></p>";  
      }
       if (checkd5 == 1){
      p_status += "Sensor 5 :<span style='background-color:#3cb371'>Online</span>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "Sensor 5 :<span style='background-color:#ff0000'>Offline </span>";
      p_status += "&nbsp;";
      p_status += hours; 
      p_status +=  "h "; 
      p_status += mins;
      p_status +=  "m "; 
      p_status += mins;
      p_status +=  "s ";
      p_status += "&nbsp;";
      p_status += "last time..";
      p_status += "<p></p>"; 
      }
      if (checkd6 == 1){
      p_status += "Sensor 6 :<span style='background-color:#3cb371'>Online</span>";
      p_status += "<p></p><p><p><p>"; 
      
      }else {
      p_status += "Sensor 6 :<span style='background-color:#ff0000'>Offline </span>";
      p_status += "&nbsp;";
      p_status += hours; 
      p_status +=  "h "; 
      p_status += mins;
      p_status +=  "m "; 
      p_status += mins;
      p_status +=  "s ";
      p_status += "&nbsp;";
      p_status += "last time..";     
      p_status += "<p></p><p><p><p>"; 
      }  
        p_status +=  "<B>TEST_Check_Data</B><p>";  
      p_status += "<table> <colgroup><col span='1' style='background-color:red'>";
      p_status += "<col span='4'style='background-color:yellow'></colgroup>";      
      p_status += "<tr><th>Device..</th><th>Data1</th><th>Data2</th><th>Data3</th><th>Data4</th></tr>";
          
      p_status += "<tr>";
      p_status += "<td>";
      p_status += sender;
      p_status += "</td>";      
      p_status += "<td>";
      p_status += data1;
      p_status += "</td>";
      p_status += "<td>";
      p_status += data2;
      p_status += "</td>";
      p_status += "<td>";
      p_status += data3;
      p_status += "</td>";
      p_status += "<td>";
      p_status += data4;
      p_status += "</td>";      
      p_status += "</tr></table>";  
       
      p_status += "<p><p>";
      p_status += "<B>Test offline</B>"; 
      p_status += "<p>Device1 offile =";
      p_status += checko1;      
      p_status += "<p>Device2 offile ="; 
      p_status += checko2; 
      p_status += "<p>Device3 offile =";
      p_status +=checko3;  
      p_status += "<p>Device4 offile =";
      p_status +=checko4;
      p_status += "<p>Device5 offile =";
      p_status +=checko5;
      p_status += "<p>Device6 offile ="; 
      p_status +=checko6;
      p_status += "<p><p><form method='POST' action='/reboot'><input type='submit'value='Reboot'></form>";  
      p_status += "<p><p><form method='POST' action='/scan'><input type='submit'value='scan'></form>";    
      p_status += "</div>";
      p_status += "</body>";
      p_status += "</html>";            
      server.sendHeader("Connection", "close"); 
      server.send(200, "text/html", p_status);  
     });
    
      server.on("/scan",HTTP_POST, []() {        
        b_status = "<!DOCTYPE html>";        
        b_status += "<html lang='en'><head>"; 
        b_status += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
        b_status += "<title>FARMZ_MASTER</title>";        
        b_status += "<style>";
        b_status += ".c{text-align: center;}";
        b_status += "div,input{padding:5px;font-size:1em;}";
        b_status += "input{background-color:hsl(0, 0%, 100%) width:95%;}";
        b_status += "button{border:0;border-radius:0.3rem;background-color:hsl(0, 0%, 24%);color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;}";
        b_status += "body{text-align: center;font-family:verdana;}";
        b_status += ".q{float: right;width: 64px;text-align: right;}";        
        b_status += ".topnav {overflow: hidden;background-color: #333;}";
        b_status += ".topnav a {float: left;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
        b_status += ".topnav a:hover {background-color: #ddd;color: black;}";
        b_status += ".topnav a.active {background-color: #4CAF50;color: white;}";
        b_status += "</style>";                
        b_status += "</head>";
        b_status += "<body style='background-color:#b4b4b4;'>";        
        b_status += "<div class='topnav'>";   
        b_status += "<a href='/'>Config</a>";   
        b_status += "<a class='active'href='/status'>Status</a>";      
        b_status += "<a href='/reboot'>Reboot</a>"; 
        b_status += "<a href='http://www.zhotspot.com'>About</a>"; 
        b_status += "</div><br/>";  
          
        b_status += "<div style='text-align:left;display:inline-block;min-width:260px;'>";      
        b_status += "<h2>Setup</h2>"; 
        b_status += "<B>Band :</B>";
        b_status += eband;
        b_status += "<p></p>";  
        b_status += "<textarea name='text' cols='25' rows='5'>";
       // b_status +=  scanbond;
        b_status += "</textarea>";
         
        b_status += "</div>";
        b_status += "</body>";
        b_status += "</html>";      
                    
    server.sendHeader("Connection", "close"); 
    server.send(200, "text/html", b_status);
    }); 

    
     server.on("/config",HTTP_GET, []() {
            
        configwifi = "<!DOCTYPE html>";        
        configwifi += "<html lang='en'><head>"; 
        configwifi += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
        configwifi += "<title>FARMZ_MASTER</title>";
        
        configwifi += "<style>";
        configwifi += ".c{text-align: center;}";
        configwifi += "div,input{padding:5px;font-size:1em;}";
        configwifi += "input{background-color:hsl(0, 0%, 100%) width:95%;}";
        configwifi += "button{border:0;border-radius:0.3rem;background-color:hsl(0, 0%, 24%);color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;}";
        configwifi += "body{text-align: center;font-family:verdana;}";
        configwifi += ".l{background: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==') no-repeat left center;background-size: 1em;}";
        configwifi += ".q{float: right;width: 64px;text-align: right;}";
        
        configwifi += ".topnav {overflow: hidden;background-color: #333;}";
        configwifi += ".topnav a {float: left;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
        configwifi += ".topnav a:hover {background-color: #ddd;color: black;}";
        configwifi += ".topnav a.active {background-color: #4CAF50;color: white;}";
        configwifi += "</style>";
        
        configwifi += "<script>";
        configwifi += "function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}";
        configwifi += "</script>";
        
        configwifi += "</head>";
        configwifi += "<body style='background-color:#b4b4b4;'>";
        
        configwifi += "<div class='topnav'>";   
        configwifi += "<a href='/'>Home</a>"; 
        configwifi += "<a class='active' href='/config'>Config</a>"; 
        configwifi += "<a href='/update'>Update</a>"; 
        configwifi += "<a href='/reboot'>Reboot</a>"; 
        configwifi += "<a href='/reset'>Reset Config</a>"; 
        configwifi += "<a href='http://www.zhotspot.com'>About</a>"; 
        configwifi += "</div><br>";
          
        configwifi += "<div style='text-align:left;display:inline-block;min-width:260px;'>";      
        configwifi += "<h2>Setup</h2>";           
        configwifi += "</p>";                       
        configwifi += "<form method='get' action='setting'>";         
        configwifi += "<label>SSID: </label><input id='s' name='ssid' length=20   placeholder='SSID'><br/><br>";     
        configwifi += "<label>PASS: </label><input id='p' name='pass' length=20 type='password' placeholder='password'><br/>";
        
        configwifi += "<br> NAME : <input type='text'name='farmname' placeholder='farmname'><br><br>";              
        configwifi += "<label>Device1: </label><input name='device1'length=17 placeholder='API Thingspeak'><br><br>";
        configwifi += "<label>Device2: </label><input name='device2'length=17 placeholder='API Thingspeak'><br><br>";
        configwifi += "<label>Device3: </label><input name='device3'length=17 placeholder='API Thingspeak'><br><br>";
        configwifi += "<label>Device4: </label><input name='device4'length=17 placeholder='API Thingspeak'><br><br>";
        configwifi += "<label>Device5: </label><input name='device5'length=17 placeholder='API Thingspeak'><br><br>";
        configwifi += "<label>Device6: </label><input name='device6'length=17 placeholder='API Thingspeak'><br><br>";

        configwifi += "LoRa Band<br>";
        configwifi += "<br><label>BAND : </label><select name='band'> <br />";
        configwifi += "<option value='407'>407</option>";
        configwifi += "<option value='415'>415</option>";
        configwifi += "<option value='420'>420</option>";        
        configwifi += "<option value='425'>425</option>";        
        configwifi += "<option value='427'>427</option>";        
        configwifi += "<option value='433'>433</option>";
        configwifi += "<option value='439'>439</option>";
        configwifi += "<option value='443'>443</option>";
        
        configwifi += "</select><br />";
        
        configwifi += "<br/><button type='submit'>save</button></form>";   
        configwifi += "<br/><div class=\"c\"><a href=\"/\">Scan</a></div>";
        
        configwifi += "</div>";
        configwifi += "</body>";
        configwifi += "</html>";  
     
      server.sendHeader("Connection", "close");
      server.send(200, "text/html",configwifi);      

    });
//setting?ssid=....&pass=....&device=...&device1=...&device2=...&device3=...&device4...=&device5...=&device6...=&band=415 
    server.on("/setting", []() {    
     delay1= 700;
     delay2= 700;
     
     String FarmzName = server.arg("farmname");
     String qsid = server.arg("ssid");
     String qpass = server.arg("pass");
     String qdevice = server.arg("device");
     String qband = server.arg("band");
     String device1 = server.arg("device1");
     String device2 = server.arg("device2");
     String device3 = server.arg("device3");
     String device4 = server.arg("device4");
     String device5 = server.arg("device5");
     String device6 = server.arg("device6");
     String cdevice= "" ;
        if (qsid.length() > 0 && qpass.length() > 0 ){   
          Serial.println("clearing eeprom");
          for (int i = 0; i < 86; ++i) { EEPROM.put(i, 0); } 
          Serial.print("qsid =");
          Serial.println(qsid);  
          Serial.print("qpass =");
          Serial.println(qpass);      
          Serial.print("qdevice =");
          Serial.println(qdevice);      
          Serial.print("qband =");
          Serial.println(qband); 
          Serial.print("device1 =");
          Serial.println(device1);
          Serial.print("device2 =");
          Serial.println(device2); 
          Serial.print("device3 =");
          Serial.println(device3); 
          Serial.print("device4 =");
          Serial.println(device4); 
          Serial.print("device5 =");
          Serial.println(device5); 
          Serial.print("device6 =");
          Serial.println(device6);  
              delay(100);
          Serial.println("writing eeprom ssid:");          
          for (int i = 0; i < qsid.length(); ++i)
            {
              EEPROM.put(i, qsid[i]);   //eeprom เขียน  ชื่อไวไฟ
              Serial.print("Wrote: ");
              Serial.println(qsid[i]); 
            }
            delay(100);
          Serial.println("writing eeprom pass:"); 
          for (int i = 0; i < qpass.length(); ++i)
            {
              EEPROM.put(24+i, qpass[i]);
              Serial.print("Wrote: ");
              Serial.println(qpass[i]); 
            }    
 //           delay(100);
 //         Serial.println("writing eeprom device:"); 
 //         for (int i = 0; i < 1 ; ++i)
 //           {
 //            EEPROM.put(42+i, qdevice[i]);
 //             Serial.print("Wrote: ");
 //             Serial.println(qdevice[i]); 
 //           }    
            delay(100);
          Serial.println("writing eeprom band:"); 
          for (int i = 0; i < 3 ; ++i)
            {
              EEPROM.put(44+i, qband[i]);
              Serial.print("Wrote: ");
              Serial.println(qband[i]); 
           } 
/*..............................................apikey..*/
          if (device6.length() > 1 ){
               cdevice = "6";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           } 
              Serial.println("writing eeprom device2:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(64+i, device2[i]);
              Serial.print("Wrote: ");
              Serial.println(device2[i]); 
           } 
              Serial.println("writing eeprom device3:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(81+i, device3[i]);
              Serial.print("Wrote: ");
              Serial.println(device3[i]); 
           }
             Serial.println("writing eeprom device4:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(98+i, device4[i]);
              Serial.print("Wrote: ");
              Serial.println(device4[i]); 
           }
             Serial.println("writing eeprom device5:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(115+i, device5[i]);
              Serial.print("Wrote: ");
              Serial.println(device5[i]); 
           }
             Serial.println("writing eeprom device6:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(132+i, device6[i]);
              Serial.print("Wrote: ");
              Serial.println(device6[i]); 
           }     
                    
          } else if (device5.length() > 1 ){
              cdevice = "5";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           } 
              Serial.println("writing eeprom device2:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(64+i, device2[i]);
              Serial.print("Wrote: ");
              Serial.println(device2[i]); 
           } 
              Serial.println("writing eeprom device3:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(81+i, device3[i]);
              Serial.print("Wrote: ");
              Serial.println(device3[i]); 
           }
             Serial.println("writing eeprom device4:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(98+i, device4[i]);
              Serial.print("Wrote: ");
              Serial.println(device4[i]); 
           }
             Serial.println("writing eeprom device5:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(115+i, device5[i]);
              Serial.print("Wrote: ");
              Serial.println(device5[i]); 
           }
           
          } else if (device4.length() > 1 ){
              cdevice = "4";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           } 
              Serial.println("writing eeprom device2:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(64+i, device2[i]);
              Serial.print("Wrote: ");
              Serial.println(device2[i]); 
           } 
              Serial.println("writing eeprom device3:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(81+i, device3[i]);
              Serial.print("Wrote: ");
              Serial.println(device3[i]); 
           }
             Serial.println("writing eeprom device4:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(98+i, device4[i]);
              Serial.print("Wrote: ");
              Serial.println(device4[i]); 
           }
          }else if (device3.length() > 1 ){
              cdevice = "3";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           } 
              Serial.println("writing eeprom device2:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(64+i, device2[i]);
              Serial.print("Wrote: ");
              Serial.println(device2[i]); 
           } 
              Serial.println("writing eeprom device3:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(81+i, device3[i]);
              Serial.print("Wrote: ");
              Serial.println(device3[i]); 
           }
          }else if (device2.length() > 1 ){
              cdevice = "2";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           } 
              Serial.println("writing eeprom device2:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(64+i, device2[i]);
              Serial.print("Wrote: ");
              Serial.println(device2[i]); 
           }
          } else if (device1.length() > 1 ){
              cdevice = "1";
              Serial.println("writing eeprom device1:"); 
            for (int i = 0; i < 16 ; ++i)
            {
              EEPROM.put(47+i, device1[i]);
              Serial.print("Wrote: ");
              Serial.println(device1[i]); 
           }
          }


          
          Serial.println("writing eeprom device:"); 
          for (int i = 0; i < 1 ; ++i)
            {
             EEPROM.put(42+i, cdevice[i]);
              Serial.print("Wrote: ");
              Serial.println(cdevice[i]); 
            }            
        Serial.println(cdevice); 

        Serial.println("writing eeprom Farmname:"); 
            for (int i = 0; i < 16; ++i)
            {
              EEPROM.put(149+i, FarmzName[i]); //to 164 ตัวอักษร
              Serial.print("Wrote: ");
              Serial.println(FarmzName[i]);  
           }    
                       
      EEPROM.commit();      
      u8x8.setInverseFont(1);//คล่มสี
      u8x8.println("saved config");
      u8x8.setInverseFont(0);//คล่มสี

          content = " <html>";
          content += " <head>";
          content += " <title>ZNAsolution</title>";
          content += "<meta http-equiv= 'refresh' content='3; url=/reboot'/>"; 
          content += " </head>";
          content += "<body>";
          content += "reboot esp32 to save config ";
          content += "</body>";
          content += "</html>";           
          statusCode = 200;
        } else {
          content = "{\"Error\":\"404 not found\"}";
          statusCode = 404;
          Serial.println("Sending 404");
        }
        server.send(statusCode, "text/html", content);
        delay(10000);
    });

    
     server.on("/reboot",HTTP_GET, []() {          
      reboot = "<!DOCTYPE html>";
      reboot += "<html><head>"; 
      reboot += "<title>FARMZ_MASTER</title>";
      reboot += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
      reboot += "<style>";
      reboot += "body{  margin: 0; font-family: Arial, Helvetica, sans-serif;}";
      reboot += ".topnav {overflow: hidden;background-color: #333;}";
      reboot += ".topnav a {float: left;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
      reboot += ".topnav a:hover {background-color: #ddd;color: black;}";
      reboot += ".topnav a.active {background-color: #4CAF50;color: white;}";
      reboot += "</style>";
      reboot += "</head>";
      reboot += "<body>";
      reboot += "<div class='topnav'>";   
      reboot += "<a href='/'>Home</a>"; 
      reboot += "<a href='/config'>Config</a>"; 
      reboot += "<a href='/update'>Update</a>"; 
      reboot += "<a class='active' href='/reboot'>Reboot</a>"; 
      reboot += "<a href='/reset'>Reset Config</a>"; 
      reboot += "<a href='http://facebook.com/noonpalakorn'>About</a>"; 
      reboot += "</div>";    
      reboot += "<div style='padding-left:16px'>";
      reboot += "<h2>REBOOT</h2>";
      reboot += "<p>Restart system Ok!</p>";
      reboot += "<form method='POST' action='/reboot'><input type='submit'value='YES'></form>";
      reboot += "</div>";
      reboot += "</body>";
      reboot += "</html>";         
      server.sendHeader("Connection", "close"); 
      server.send(200, "text/html", reboot);        
     });
     
     server.on("/reboot",HTTP_POST, []() {      
      reboot = " <html>";
      reboot += " <head>";
      reboot += "<title>FARMZ_MASTER</title>";
      reboot += "<meta http-equiv= 'refresh' content='3; url=/'/>"; 
      reboot += " </head>";
      reboot += "<body>";
      reboot += "reboot in 5 s";
      reboot += "</body>";
      reboot += "</html>"; 
      
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", reboot);   
      String miniproreboot = "reboot";     
       delay(1000);  
         #ifdef ESP8266
       ESP.reset();
         #else
       ESP.restart();
         #endif
    });

     server.on("/reset",HTTP_GET, []() {
      reconfig  = "<!DOCTYPE html>";
      reconfig += "<html><head>"; 
      reconfig += "<title>FARMZ_MASTER</title>";
      reconfig += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
      reconfig += "<style>";
      reconfig += "body{  margin: 0; font-family: Arial, Helvetica, sans-serif;}";
      reconfig += ".topnav {overflow: hidden;background-color: #333;}";
      reconfig += ".topnav a {float: left;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
      reconfig += ".topnav a:hover {background-color: #ddd;color: black;}";
      reconfig += ".topnav a.active {background-color: #4CAF50;color: white;}";
      reconfig += "</style>";
      reconfig += "</head>";
      reconfig += "<body>";
      reconfig += "<div class='topnav'>";   
      reconfig += "<a href='/'>Home</a>"; 
      reconfig += "<a href='/config'>Config</a>";
      reconfig += "<a href='/update'>Update</a>"; 
      reconfig += "<a href='/reboot'>Reboot</a>"; 
      reconfig += "<a class='active'href='/reset'>Reset Config</a>"; 
      reconfig += "<a href='http://facebook.com/noonpalakorn'>About</a>"; 
      reconfig += "</div>";    
      reconfig += "<div style='padding-left:16px'>";
      reconfig += "<h2>RESET</h2>";
      reconfig += "<p>Reset configuration ok! </p>";
      reconfig += "<br><form method='POST' action='/reset'><input type='submit'value='YES'></form>";
      reconfig += "</div>";
      reconfig += "</body>";
      reconfig += "</html>";       
      server.sendHeader("Connection", "close"); 
      server.send(200, "text/html", reconfig);      
    
    });
    
     server.on("/reset", HTTP_POST,[]() {  
    
      reconfig = " <html>";
      reconfig += " <head>";
      reconfig += "<title>FARMZ_MASTER</title>";
      reconfig += "<meta http-equiv= 'refresh' content='3; url=/'/>";   
      reconfig += " </head>";
      reconfig += "<body>";
      reconfig += "reset in 5 s";
      reconfig += "</body>";
      reconfig += "</html>"; 
      server.sendHeader("Connection", "close"); 
      server.send(200, "text/html", reconfig); 
      
      delay1= 1000; 
       
    destination1 = 0x63;     
    destination2 = 0x63;      
    destination3 = 0x63;  
    destination4 = 0x63;     
    destination5 = 0x63;  
    destination6 = 0x63;
    
    delay(20000); 
      Serial.println("clearing eeprom");          
      for (int i = 0; i < 150; ++i) { EEPROM.write(i, 0); }     
      EEPROM.commit();
      delay(1000);
      //ESP.eraseConfig();
      delay(1000);
         #ifdef ESP8266
       ESP.reset();
         #else
       ESP.restart();
         #endif
    });
        
     server.on("/update",HTTP_GET, []() {
      update  = "<!DOCTYPE html>";
      update += "<html><head>"; 
      update += "<title>FARMZ_MASTER</title>";
      update += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
      update += "<style>";
      update += "body{  margin: 0; font-family: Arial, Helvetica, sans-serif;}";
      update += ".topnav {overflow: hidden;background-color: #333;}";
      update += ".topnav a {float: left;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
      update += ".topnav a:hover {background-color: #ddd;color: black;}";
      update += ".topnav a.active {background-color: #4CAF50;color: white;}";
      update += "</style>";
      update += "</head>";
      update += "<body>";
      update += "<div class='topnav'>";   
      update += "<a href='/'>Home</a>"; 
      update += "<a href='/config'>Config</a>";
      update += "<a class='active'href='/update'>Update</a>"; 
      update += "<a href='/reboot'>Reboot</a>"; 
      update += "<a href='/reset'>Reset Config</a>"; 
      update += "<a href='http://www.zhotspot.com'>About</a>"; 
      update += "</div>";    
      update += "<div style='padding-left:16px'>";
      update += "<h2>Uupdate firmware</h2>";
      update  += "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'>"; 
      update  += "<input type='submit' value='Update'>";     
      update += "</div>";
      update += "</body>";
      update += "</html>";                    
    server.sendHeader("Connection", "close"); 
    server.send(200, "text/html", update);
    });                                             
    
    server.on("/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");      
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, 
    []() 
    {
      
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin()) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
    }); 

     
    
    
  }
}

void sendMessage1(byte destination) {  
  String incoming = "";  
  SerialNum = ""; 
   data1 = ""; 
   data2 = "";  
   data3 = "";  
   data4 = "";
   lora_rssi = 0; 
   sender = 0;    
  Serial.println("Send REQUEST");
  LoRa.beginPacket();                  
  LoRa.write(destination);             
  delay(10);
  LoRa.write(destination);
  LoRa.print("🤘🤘🤘🤘🤘🤘🤘");                 
  LoRa.endPacket();                   
  
  // end send request

  //start wait data
  long interval = 0; // รอสิบวิ
  while (LoRa.parsePacket() == 0) {
    if (interval > 100000) {
      Serial.println("Wait Data Timeout");      
      u8x8.setCursor(4,4);
      u8x8.println("Timeout");
      u8x8.setCursor(0,5);
      u8x8.println("please check !");
      u8x8.setCursor(4,6);
      u8x8.println("sensor.");
      return;
    }
    interval++;
  }  
    
//  rssi = LoRa.packetRssi();
   sender = LoRa.read();            // sender address  
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
  
  if (sender == destination) { //เช็คว่าไอดีของ sensor ที่ส่งกลับมาตรงกับไอดีที่ส่งไปไหม ถ้าตรง อ่านค่าได้
      Serial.print("Has Data: ");
      Serial.println(destination);
      Serial.println(incoming);      
    //Substring Data Process               
        // SERIAL NUMBER
        String L1 = incoming.substring(incoming.indexOf("L=") +2);
        SerialNum = L1.substring(0,L1.indexOf(","));
        Serial.print(SerialNum);
        Serial.println();
        // CURRENT
        String L2 = incoming.substring(incoming.indexOf("nt1=") +4);
        data1 = L2.substring(0,L2.indexOf(","));
        Serial.print(data1);
        Serial.println();
        // AMMONIA
        String L3 = incoming.substring(incoming.indexOf("nt2=") +4);
        data2 = L3.substring(0,L3.indexOf(","));
        Serial.print(data2); 
        Serial.println();        
        // HUMIDITY
        String L4 = incoming.substring(incoming.indexOf("nt3=") +4);
        data3 = L4.substring(0,L4.indexOf(","));
        Serial.print(data3);
        Serial.println();
        // ++++      
        String L5 = incoming.substring(incoming.indexOf("nt4=") +4);
        data4 = L5.substring(0,L5.indexOf(","));
        Serial.print(data4);
        Serial.println();
       
        //OLED Status
   //u8x8.setFont(u8x8_font_chroma48medium8_r);
   //u8x8.clear();
   u8x8.setFlipMode(0);//กลับหัว      
   u8x8.setCursor(5,2);
   u8x8.println(data1);   
   u8x8.setCursor(5,3);
   u8x8.println(data2); 
   u8x8.setCursor(5,4);   
   u8x8.println(data3);
   u8x8.setCursor(5,5);   
   u8x8.println(data4);
   u8x8.setInverseFont(1);
   u8x8.print("IP ");
   u8x8.println(WiFi.localIP());
   u8x8.setInverseFont(0);
   lora_rssi = LoRa.packetRssi();
   u8x8.setCursor(0,7);
   u8x8.print("with RSSI = ");
   u8x8.print(LoRa.packetRssi());    
  
  }
  
 }

void loop() {   
  server.handleClient();  
  digitalWrite(23, HIGH);   
   buttonState = digitalRead(22);     
  if (buttonState == 1){     
    Serial.print(i); 
   u8x8.clear();
   u8x8.setCursor(5,4);
   u8x8.print("reset= "); 
   u8x8.print(i);
  digitalWrite(LED, 1);
  delay(1000);
  digitalWrite(LED, 0);          
    i--;    
  if (i < 1){
    fillDisplay();
    Serial.print("SYS reset");
    u8x8.println("SYS reset ");
  for (int i = 0; i < 150; ++i) { EEPROM.put(i, 0); }    
    EEPROM.commit();
    delay(1000);
    ESP.restart();
     }
     u8x8.clear();
     u8x8.print("configwifi");        
    }else {i = 4; 
   }   
  delay(10);
}

void loop1(void *pvParameters) {
  while (1) {   
    
  if (edevice.toInt()== 1){    
  u8x8.clear();  
  u8x8.setCursor(0,0);
  u8x8.setInverseFont(1);
  Serial.println("Device= 1");
  u8x8.print("device=");
  u8x8.println(destination1);   
  Serial.println(lora_rssi);         
  u8x8.setInverseFont(0);
  sendMessage1(destination1);
      if (sender == 1){
        checkd1 = 1;
      }else {
        checkd1 = 0;
        checko1 ++;
      }
      
      Serial.print ("checkd1 :");
      Serial.println (checkd1);
      Serial.print ("sender :");
      Serial.println (sender);
      
         uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey1,destination1);
  // Serial.println("upload success");    
       
  delay(delay1); 
  clearLineRT();
  clearLineLT(); 
  }
  
 if (edevice.toInt()== 2){    
  u8x8.clear();  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 1");
  u8x8.print("device=");
  u8x8.println(destination1);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination1);
    if (sender == 1){
        checkd1 = 1;
      }else {
        checkd1 = 0;
        checko1 ++;
      }
      Serial.println ("checkd1 :");
      Serial.println (checkd1);
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey1,destination1);
  
  delay(delay1); // รอ 10 วิ
  u8x8.clear(); 
  
     
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 2");
  u8x8.print("device=");
  u8x8.println(destination2);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination2);
  
      if (sender == 2){         
        checkd2 = 1;
      }else {
        checkd2 = 0;
        checko2 ++;
      }         
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey2,destination2);
       
  delay(delay1);
  clearLineRT();
  clearLineLT();   
  }

 if (edevice.toInt()== 3){  
  u8x8.clear();  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 1");
  u8x8.print("device=");
  u8x8.println(destination1);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination1);
  if (sender == 1){
        checkd1 = 1;
      }else {
        checkd1 = 0;
        checko1 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey1,destination1);
        Serial.println("upload success"); 
  delay(delay1); // รอ 10 วิ
  u8x8.clear(); 
  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 2");
  u8x8.print("device=");
  u8x8.println(destination2);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination2);
  
      if (sender == 2){         
        checkd2 = 1;
      }else {
        checkd2 = 0;
        checko2 ++;
      }         
  
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey2,destination2);
        Serial.println("upload success"); 
  delay(delay1);
  u8x8.clear(); 
  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 3");
  u8x8.print("device=");
  u8x8.println(destination3);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination3);
   if (sender == 3){         
        checkd3 = 1;
       }else {
        checkd3 = 0;
        checko3 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey3,destination3);
        Serial.println("upload success"); 
  delay(delay1);
  clearLineRT();
  clearLineLT(); 
 }

 if (edevice.toInt()== 4){  
  u8x8.clear();  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 1");
  u8x8.print("device=");
  u8x8.println(destination1);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination1);
     if (sender == 1){         
        checkd1 = 1;
      }else {
        checkd1 = 0;
        checko1 ++;
      } 
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey1,destination1);
        Serial.println("upload success"); 
  delay(delay1); // รอ 10 วิ
  u8x8.clear(); 
  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 2");
  u8x8.print("device=");
  u8x8.println(destination2);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination2);
  
      if (sender == 2){         
        checkd2 = 1;
      }else {
        checkd2 = 0;
        checko2 ++;
      }         
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey2,destination2);
        Serial.println("upload success"); 
  delay(delay1);
  u8x8.clear(); 
  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 3");
  u8x8.print("device=");
  u8x8.println(destination3);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination3);
   if (sender == 3){         
        checkd3 = 1;
       }else {
        checkd3 = 0;
        checko3 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey3,destination3);
        Serial.println("upload success"); 
  delay(delay1);
  u8x8.clear(); 

  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 4");
  u8x8.print("device=");
  u8x8.println(destination4);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination4);
   if (sender == 4){        
        checkd4 = 1;
       }else {
        checkd4 = 0;
        checko4 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey4,destination4);
        Serial.println("upload success"); 
  delay(delay1);
  clearLineRT();
  clearLineLT(); 
 }
 
 if (edevice.toInt()== 5){    
  u8x8.clear();  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 1");
  u8x8.print("device=");
  u8x8.println(destination1);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination1);
  if (sender == 1){
        checkd1 = 1;
      }else {
        checkd1 = 0;
        checko1 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey1,destination1);
        Serial.println("upload success"); 
  delay(delay1); // รอ 10 วิ
  u8x8.clear(); 
  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 2");
  u8x8.print("device=");
  u8x8.println(destination2);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination2);
  
      if (sender == 2){         
        checkd2 = 1;
      }else {
        checkd2 = 0;
        checko2 ++;
      }         
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey2,destination2);
        Serial.println("upload success"); 
  delay(delay1);
  u8x8.clear(); 
  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 3");
  u8x8.print("device=");
  u8x8.println(destination3);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination3);
   if (sender == 3){         
        checkd3 = 1;
       }else {
        checkd3 = 0;
        checko3 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey3,destination3);
        Serial.println("upload success"); 
  delay(delay1);
  u8x8.clear(); 

  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 4");
  u8x8.print("device=");
  u8x8.println(destination4);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination4);
   if (sender == 4){        
        checkd4 = 1;
       }else {
        checkd4 = 0;
        checko4 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey4,destination4);
        Serial.println("upload success"); 
  delay(delay1);
  u8x8.clear(); 

  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 5");
  u8x8.print("device=");
  u8x8.println(destination5);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination5);
   if (sender == 5){         
        checkd5 = 1;
      }else {
        checkd5 = 0;
        checko5 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey5,destination5);
        Serial.println("upload success"); 
  delay(delay1);
  clearLineRT();
  clearLineLT();
  } //device

  if (edevice.toInt()== 6){    
  u8x8.clear();  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 1");
  u8x8.print("device=");
  u8x8.println(destination1);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination1);
  if (sender == 1){
        checkd1 = 1;
      }else {
        checkd1 = 0;
        checko1 ++;
      }

      Serial.print("print checkd#o=");
      Serial.print(sender);
      Serial.print(" ");
      Serial.println(checko1);
      
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey1,destination1);
        Serial.println("upload success"); 
  delay(delay1); // รอ 10 วิ
  u8x8.clear(); 
  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 2");
  u8x8.print("device=");
  u8x8.println(destination2);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination2);
  
      if (sender == 2){         
        checkd2 = 1;
      }else {
        checkd2 = 0;
        checko2 ++;
      }         
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey2,destination2);
        Serial.println("upload success"); 
  delay(delay1);
  u8x8.clear(); 
  
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 3");
  u8x8.print("device=");
  u8x8.println(destination3);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination3);
   if (sender == 3){         
        checkd3 = 1;
       }else {
        checkd3 = 0;
        checko3 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey3,destination3);
        Serial.println("upload success"); 
  delay(delay1);
  u8x8.clear(); 

  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 4");
  u8x8.print("device=");
  u8x8.println(destination4);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination4);
   if (sender == 4){        
        checkd4 = 1;
       }else {
        checkd4 = 0;
        checko4 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey4,destination4);
        Serial.println("upload success"); 
  delay(delay1);
  u8x8.clear(); 

  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 5");
  u8x8.print("device=");
  u8x8.println(destination5);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination5);
   if (sender == 5){         
        checkd5 = 1;
      }else {
        checkd5 = 0;
        checko5 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey5,destination5);
        Serial.println("upload success"); 
  delay(delay1);
  u8x8.clear(); 
  u8x8.setCursor(3,0);
  u8x8.setInverseFont(1);//คล่มสี
  Serial.println("Device= 6");
  u8x8.print("device=");
  u8x8.println(destination6);           
  u8x8.setInverseFont(0);//คล่มสี
  sendMessage1(destination6);
  if (sender == 6){        
       checkd6 = 1;
      }else {
       checkd6 = 0;
       checko6 ++;
      }
          uploadThingsSpeak(data1.toFloat(),data2.toFloat(),data3.toFloat(),data4.toFloat(),eapikey6,destination6);
        Serial.println("upload success"); 
  delay(delay1);
  clearLineRT();
  clearLineLT();
  } //device 
  clearLineRT();
    u8x8.print("not-config"); 
      digitalWrite(23, HIGH);   
   buttonState = digitalRead(22);     
  if (buttonState == 1){     
    Serial.print(i); 
   u8x8.clear();
   u8x8.setCursor(5,4);
   u8x8.print("reset= "); 
   u8x8.print(i);
  digitalWrite(LED, 1);
  delay(1000);
  digitalWrite(LED, 0);          
    i--;    
  if (i < 1){
    fillDisplay();
    Serial.print("SYS reset");
    u8x8.println("SYS reset ");
  for (int i = 0; i < 150; ++i) { EEPROM.put(i, 0); }    
    EEPROM.commit();
    delay(1000);
    ESP.restart();
     }
     u8x8.clear();
     u8x8.print("configwifi");        
    }else {i = 4; 
   }   
  delay(10);
    Serial.println("not-config"); 
  clearLineLT();
 } //while
} //loop


void loop2(void *pvParameters) {
  while (1) {   
    int delay3 = 700;
    delay(delay3);
    Serial.println("Device= 1");
    update_band(destination1,code_update); 
    if (check_code2 == 1){
        checkd1 = 1;
      }else {
        checkd1 = 0;
      }
       Serial.print("check_device =");
       Serial.println(check_code2);
       Serial.print("check_device1 =");
       Serial.println(checkd1);
    delay(delay3); 
    Serial.println("Device= 2"); 
    update_band(destination2,code_update);
      if (check_code2 == 2){         
        checkd2 = 1;
      }else {
        checkd2 = 0;
      } 
    delay(delay3); 
    Serial.println("Device= 3"); 
    update_band(destination3,code_update); 
    if (check_code2 == 3){         
        checkd3 = 1;
       }else {
        checkd3 = 0;
      }      
    delay(delay3);
    Serial.println("Device= 4");  
    update_band(destination4,code_update); 
     if (check_code2 == 4){        
        checkd4 = 1;
       }else {
        checkd4 = 0;
      }
    delay(delay3); 
    Serial.println("Device= 5"); 
    update_band(destination5,code_update);
     if (check_code2 == 5){         
        checkd5 = 1;
      }else {
        checkd5 = 0;
      }  
    delay(delay3); 
    Serial.println("Device= 6"); 
    update_band(destination6,code_update);
     if (check_code2 == 6){        
       checkd6 = 1;
      }else {
       checkd6 = 0;
      }    
     delay(delay3);   
    } //while
 } //loop

/*..................................................lorasend_bandupdate.....*/
 void update_band(byte Address ,byte code_update) {  
  check_code2 = 0;
   
  LoRa.beginPacket(); 
  LoRa.write(Address);  
  LoRa.print("SN=");  
  LoRa.print(SerialNumber);   
  LoRa.print(",");           
  LoRa.endPacket();  
   
  long interval = 0;    
  while (LoRa.parsePacket() == 0) {
     if (interval > 100000) {  
      Serial.println(" ....Not_sync");  
      Serial.println("Re_sender");    
      return;
    }
    interval++;
  }  
  int updatband_delay = 0;
  check_code = LoRa.read(); 
  check_code2 = LoRa.peek();
  String data1 = "";      
 
  while (LoRa.available()) {
    data1 += (char)LoRa.read();        
  }  


  Serial.print("check_code2 = "); 
  Serial.println(check_code2);
  if (check_code == code_update) { 
    while  (updatband_delay < 2) {
     Serial.println("");
     Serial.print("check_code=");  
     Serial.println(check_code);
      sendband(1,Address);
     updatband_delay++;
    }    
  }   
   sendband(0,Address);    
}


void sendband(int code,byte Address) {
  if (code == 1) {
  String sband = ""; 
  Serial.println("Reading EEPROM Band");
  for (int i = 44; i < 47; ++i)
    {
      sband += char(EEPROM.read(i));
    }
    delay(100);
   Serial.print("sender:band");
   Serial.println(sband);
       LoRa.beginPacket();                  
       LoRa.write(code_update);                            
       LoRa.print(sband);                
       LoRa.endPacket(); 
     
      long interval = 0;    
     while (LoRa.parsePacket() == 0) {
     if (interval > 50000) {
      //Serial.println("Wait Data Timeout loop2");  
      return;
    }
    interval++;
  }
    int updatband_delay = 0;
     check_code = LoRa.read(); 
     address = LoRa.peek();
    String data1 = ""; 
 
  while (LoRa.available()) {
    data1 += (char)LoRa.read();        
  }
  if ( address == code_confirm ){    
       Serial.println("reday to update");             
   }
   
  } else if (code == 0) {    
    Serial.println("Re_sender");  
   } 
 }
 
 void reset_band(byte code_update) {   
  LoRa.beginPacket(); 
  LoRa.write(code_update);                
  LoRa.endPacket();  
   
  long interval = 0;    
  while (LoRa.parsePacket() == 0) {
     if (interval > 100000) {  
      Serial.println(" ....Not_sync");  
      Serial.println("Re_sender");    
      return;
    }
    interval++;
  }
  
  int updatband_delay = 0;
  byte check_code = LoRa.read();  
  String data1 = "";  
  
  while (LoRa.available()) {
    data1 += (char)LoRa.read();        
  }  
   
  if (check_code == code_update) { 
   Serial.println(data1);      
  }     
}
/*.......................    String esid ;String epass ;..............uploadthing.....*/

void uploadThingsSpeak(float data1, float data2, float data3,float data4, String apikey, byte idboard) {
  delay(delay2);   
  IPAddress ip = WiFi.localIP();    
  String ip_status = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);   
 
   if (data1 > 0){ 
  myStatus = "Sensor Online " ; 
   
  ThingSpeak.setField(1, data1);
  ThingSpeak.setField(2, data2);
  ThingSpeak.setField(3, data3);
  ThingSpeak.setField(4, data4);   
  ThingSpeak.setField(5, lora_rssi);
    
  }else {
     myStatus = "Sensor Offline " ;       
    }      
 if (sendsta == 0 ){myStatus += eFarmzName;}else {myStatus += ip_status;}  
  
  apikey.toCharArray(B,17);    
  delay(50);   
  Serial.print("upload apikey : ");
  Serial.println(B);  
  Serial.println("lora_rssi :");
  Serial.println(lora_rssi);
  Serial.println(myStatus);
    
  ThingSpeak.setStatus(myStatus);   
  int x = ThingSpeak.writeFields(myChannelNumber,B);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }   
  if (x == -301){    
      ESP.restart();      
  }  
 delay(delay2);   
 sendsta -- ; 
 if (sendsta < 0){
  sendsta = 2;
  } 
  myStatus = "";
 }

void clearLineRT(){    
  for( c = u8x8.getCols()-1; c > 0; c--)
  {
    draw_bar(c-1, 1);
    draw_bar(c, 0);
    delay(50);
  }
   draw_bar(0, 0);
}

void clearLineLT(){  
  for( c = 1; c < u8x8.getCols(); c++ )
  {
    draw_bar(c, 1);
    draw_bar(c-1, 0);
    delay(50);
  }
  draw_bar(u8x8.getCols()-1, 0);
}

void fillDisplay(){
  u8x8.fillDisplay();
  for( r = 0; r < u8x8.getRows(); r++ )
  {
    u8x8.clearLine(r);
    delay(100);
  }
}

void draw_bar(uint8_t c, uint8_t is_inverse){  
  uint8_t r;
  u8x8.setInverseFont(is_inverse);
  for( r = 0; r < u8x8.getRows(); r++ )
  {
    u8x8.setCursor(c, r);
    u8x8.print(" ");
  }
}

void draw_ascii_row(uint8_t r, int start){
  int a;
  uint8_t c;
  for( c = 0; c < u8x8.getCols(); c++ )
  {
    u8x8.setCursor(c,r);
    a = start + c;
    if ( a <= 255 )
      u8x8.write(a);
  }
}
void device_timeup (int uptime){
  if(uptime == 1){  
   currentmillis=millis();
   secs = currentmillis/1000;
   mins=secs/60;
   hours=mins/60;
   days=hours/24;
   secs=secs-(mins*60);
   mins=mins-(hours*60);
   hours=hours-(days*24);
  }
  if(uptime == 0){
    Serial.print ("device online");
  }
} 


