    #include <PubSubClient.h>       
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
  
      
    #define LED 2     
    #define SS      18
    #define RST     14
    #define DI0     26
    
  IPAddress local_ip = {192,168,1,1};       // สำหรับ AP Mode         
  IPAddress gateway = {192,168,1,1};
  IPAddress subnet = {255,255,255,0};
  
    String BAND = "433"; 
    String miniproreboot = "reboot"; 
    const char* host = "esp32-webupdate";
    const char* ssid = "FARMZ";
    const char* pass = "admin123";
    const char* update_path = "/update";
    const char* www_username = "root";
    const char* www_password = "password";

    const char* mqttServer = "18.138.250.120";
    const int   mqttPort = 1883;
    const char* mqttUser = "mqtt";
    const char* mqttPassword = "password";
    const char* topic = "Farmzmaster/sensor";
    char B[16];
    //ตัวแปลไว้เช็คการเชื่อมต่อข้อง Client lora
    bool Lora_Client ;
    String c_rssi; 
    int Status_Deviec1 = 0 ;
    int Status_Deviec2 = 0 ;
    int Status_Deviec3 = 0 ;
    int Status_Deviec4 = 0 ;
    int Status_Deviec5 = 0 ;
    int Status_Deviec6 = 0 ;
    
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
    
    
   String SerialNumber1 ;
   String SerialNumber2 ;
   String SerialNumber3 ;
   String SerialNumber4 ;
   String SerialNumber5 ;
   String SerialNumber6 ;
   
   String myStatus = "";
   String eFarmzName ="";
   String statusip = ""; 
   unsigned long myChannelNumber = 1; 
    
  WiFiClient espClient;
  PubSubClient mqtt(espClient);
  WebServer server(80);

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);   
#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 8
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8X8LOG u8x8log;


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
  
void oled_ip(){
  IPAddress ip_s = WiFi.softAPIP();    
  String ipStr_s = String(ip_s[0]) + '.' + String(ip_s[1]) + '.' + String(ip_s[2]) + '.' + String(ip_s[3]);   
  terminal("IP :");
  terminal_n(ipStr_s); 
}
  
 void callback(char* topic, byte* payload, unsigned int length) { 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  } 
  Serial.println();
  Serial.println("-----------------------"); 
}

//oled println
void oled_print(uint8_t x ,uint8_t y ,String mes){ 
  u8x8.setCursor(x,y);
  u8x8.print(mes);  
}
//OLED แถบหัว
void oled_pre(uint8_t x ,uint8_t y ,String mes){
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);    
  //u8x8.clear();
  u8x8.setCursor(x,y);
  u8x8.inverse();
  u8x8.print(mes);
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_u );  
  u8x8.noInverse();
}

void draw_1x2(uint8_t x ,uint8_t y ,const char* mes){
 u8x8.setFont(u8x8_font_chroma48medium8_r );
 u8x8.draw1x2String(x, y, mes); 
 delay(1000);
}
void terminal(String mes){
  u8x8.setFont(u8x8_font_chroma48medium8_r );
  u8x8log.print(mes);  
}
void terminal_n(String mes){
  u8x8.setFont(u8x8_font_chroma48medium8_r );
  u8x8log.print(mes);
  u8x8log.print("\n");
}
void terminal_f(String mes){
  u8x8.setFont(u8x8_font_chroma48medium8_r );
  u8x8log.print(mes);
  u8x8log.print("\f");
}
void fillDisplay(){
  for(int r = 0; r < u8x8.getRows(); r++ )
  {
    u8x8.clearLine(r);
    delay(100);
  }
}
void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);   
  u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8x8log.setRedrawMode(0);    // 0: Update screen with newline, 1: Update screen for every char
  
  MDNS.addService("http", "tcp", 80);  
  pinMode(22,INPUT);
  pinMode(23,OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(22, LOW);
  digitalWrite(23, HIGH);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  //.................................................//OLED/config
  u8x8.begin();
  
  oled_pre(3,1,"STARTDEVICE");  
  draw_1x2(3,3,"FARMZMASTER");    
  terminal_n("Running....");
  terminal_n("config");
  Serial.println("Running....");
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
  
  terminal_n("ReadingEEPROM...");
  terminal_f(" ");
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
   /*.................................................apikey*/
  Serial.println("Reading EEPROM SerialNumber1");
  for (int i = 47; i < 64; ++i)
    {
      SerialNumber1 += char(EEPROM.read(i));
    }
   Serial.print("SerialNumber1 : ");
   Serial.println(SerialNumber1);
   
  Serial.println("Reading EEPROM SerialNumber2");
  for (int i = 64; i < 81; ++i)
    {
      SerialNumber2 += char(EEPROM.read(i));
    }
   Serial.print("SerialNumber2 : ");
   Serial.println(SerialNumber2);
   
  Serial.println("Reading EEPROM SerialNumber3");
  for (int i = 81; i < 98; ++i)
    {
      SerialNumber3 += char(EEPROM.read(i));
    }
   Serial.print("SerialNumber3 : ");
   Serial.println(SerialNumber3); 
   
  Serial.println("Reading EEPROM SerialNumber4");
  for (int i = 98; i < 115; ++i)
    {
      SerialNumber4 += char(EEPROM.read(i));
    }
   Serial.print("SerialNumber4 : ");
   Serial.println(SerialNumber4);  
   
  Serial.println("Reading EEPROM SerialNumber5");
  for (int i = 115; i < 132; ++i)
    {
      SerialNumber5 += char(EEPROM.read(i));
    }
   Serial.print("SerialNumber5 : ");
   Serial.println(SerialNumber5);
   
  Serial.println("Reading EEPROM SerialNumber6");
  for (int i = 132; i < 149; ++i)
    {
      SerialNumber6 += char(EEPROM.read(i));
    }
   Serial.print("SerialNumber6 : ");
   Serial.println(SerialNumber6);


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
      if (testWifi()) {
        launchWeb(0);       //เรียกใช้ ฟั่งซั่น  กำหนดตัวแปล เป็น 0         
        lorabegin(BAND);
//...................................................//lora connect 
        terminal_n("connected");
        delay(100); 
         xTaskCreate(&loop1, "loop1",  4000, NULL, 10, NULL);    
        // xTaskCreate(&loop3, "loop3",  2000, NULL, 9, NULL);        
        return;
      } 
   }  else {   
    lorabegin(BAND);
  setupAP(); 
  delay(1000);
        xTaskCreate(&loop2, "loop2",  2000, NULL, 5, NULL);
   
  }  //end else  
 } // end setup void
   


//เช็คสถาณไวไฟ และการทำงานไวไฟ boolซึ่งเป็นตัวแปรทางลอจิก มีค่าเป็น True (1) หรือ False (0) ได้เท่านั้น
bool testWifi(void) {    
    int c = 20;
    WiFi.begin(esid.c_str(), epass.c_str());
    Serial.println("Waiting for Wifi to connect");  
    terminal_n("Waiting.connect");
    while ( c > 1 ) {    
    if (WiFi.status() == WL_CONNECTED) {
        delay(1000); 
      return true;              //ให้ค่าเป็นจริง
    } 
    delay(500);
    Serial.print("wifi :"); 
    Serial.println(WiFi.status());      
    c--;   //ลดลง  
   String Text_String;
   String Print_String = Text_String+String(c) ;
   u8x8log.print("connecting - ");  
   terminal_n(Print_String);
   Serial.print("startconfig :");   
   Serial.print( c );  
   Serial.println (" second");    
   } 
  return true;                  //ให้ค่าเป็นจริง 
} 

void launchWeb(int webtype) {   
  Serial.println("");
  Serial.println("Start WiFi");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  oled_ip();
  createWebServer(webtype);//เรียกใช้ฟั่งซั่น  webserver createWebServer
  
  // Start the server
 
  server.begin();
  Serial.println("Server started"); 
}

void setupAP(void) {    //config wifi 
  WiFi.mode(WIFI_STA);   //ตั้งให้เป็น โหมด station
  oled_pre(1,1,"Sutup Device");
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
  WiFi.softAPConfig(local_ip,gateway,subnet);
  WiFi.softAP(ssid, pass, 6);   //ตั้งไวไฟ  ชื่อ รหัสผ่าน และ  ช่องศัญญาน
  terminal_n(ssid); 
  terminal_n(pass);
  Serial.println("Sutup Device");     //แสดง
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
        configwifi += "<a href='https://www.facebook.com/farmzmaster'>About</a>"; 
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
        configwifi += "<label>Device1: </label><input name='device1'length=17 placeholder='SerialNumber'><br><br>";
        configwifi += "<label>Device2: </label><input name='device2'length=17 placeholder='SerialNumber'><br><br>";
        configwifi += "<label>Device3: </label><input name='device3'length=17 placeholder='SerialNumber'><br><br>";
        configwifi += "<label>Device4: </label><input name='device4'length=17 placeholder='SerialNumber'><br><br>";
        configwifi += "<label>Device5: </label><input name='device5'length=17 placeholder='SerialNumber'><br><br>";
        configwifi += "<label>Device6: </label><input name='device6'length=17 placeholder='SerialNumber'><br><br>";
        
        
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
            delay(100);
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
   
          content = " <html>";
          content += " <head>";
          content += "<title>FARMZ_MASTER</title>";
          content += "<meta http-equiv= 'refresh' content='3; url=/reboot'/>"; 
          content += " </head>";
          content += "<body>";
          content += "reboot to save config ";
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
      reboot += "<a href='/'>Config</a>";   
      reboot += "<a href='/status'>Status</a>";      
      reboot += "<a class='active'href='/reboot'>Reboot</a>"; 
      reboot += "<a href='https://www.facebook.com/farmzmaster'>About</a>"; 
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
      p_status += "<a href='https://www.facebook.com/farmzmaster'>About</a>"; 
      
      p_status += "</div>";    
      
      p_status += "<div style='padding-left:16px'>";
      p_status += "<h2>Status</h2>";
      p_status += "<p></p>";
      
       if (Status_Deviec1 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 1 : Online</FONT>";   //<FONT COLOR="Green">Computer</FONT> 
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 1 : Offline </FONT>";   //<FONT COLOR="Red">Computer</FONT> 
     
      
      p_status += "<p></p>"; 
      }
      if (Status_Deviec2 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 2 : Online</FONT>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 2 : Offline </FONT>";
      p_status += "<p></p>"; 
      }
      if (Status_Deviec3 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 3 : Online</FONT>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 3 : Offline </FONT>";
      p_status += "<p></p>"; 
      }
      if (Status_Deviec4 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 4 : Online</FONT>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 4 : Offline </FONT>";
      p_status += "<p></p>"; 
      }
       if (Status_Deviec5 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 5 : Online</FONT>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 5 : Offline </FONT>";
      p_status += "<p></p>"; 
      }
      if (Status_Deviec6 == 1){
      p_status += "<FONT COLOR='Green'>Sensor 6 : Online</FONT>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "<FONT COLOR='Red'>Sensor 6 : Offline </FONT>";
      p_status += "<p></p>"; 
      } 
        
    
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
      p_status += "<a href='https://www.facebook.com/farmzmaster'>About</a>"; 
      p_status += "</div>";    
      
      p_status += "<div style='padding-left:16px'>";
      p_status += "<h2>Status</h2>";
      p_status += "<B>Frequency</B>";
      p_status += eband;
      p_status += "<p></p>";
       if (Status_Deviec1 == 1){
      p_status += "Sensor 1 :<span style='background-color:#3cb371'>Online</span>";   
      p_status += "<p></p>"; 
      
      }else {
      p_status += "Sensor 1 :<span style='background-color:#ff0000'>Offline </span>";   
      p_status += "<p></p>"; 
      }
      if (Status_Deviec2 == 1){
      p_status += "Sensor 2 :<span style='background-color:#3cb371'>Online</span>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "Sensor 2 :<span style='background-color:#ff0000'>Offline </span>";     
      p_status += "<p></p>"; 
      
      }
      if (Status_Deviec3 == 1){
      p_status += "Sensor 3 :<span style='background-color:#3cb371'>Online</span>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "Sensor 3 :<span style='background-color:#ff0000'>Offline </span>";      
      p_status += "<p></p>"; 
      
      }
      if (Status_Deviec4 == 1){
      p_status += "Sensor 4 :<span style='background-color:#3cb371'>Online</span>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "Sensor 4 :<span style='background-color:#ff0000'>Offline </span>";      
      p_status += "<p></p>";  
      }
       if (Status_Deviec5 == 1){
      p_status += "Sensor 5 :<span style='background-color:#3cb371'>Online</span>";
      p_status += "<p></p>"; 
      
      }else {
      p_status += "Sensor 5 :<span style='background-color:#ff0000'>Offline </span>";
      p_status += "<p></p>"; 
      }
      if (Status_Deviec6 == 1){
      p_status += "Sensor 6 :<span style='background-color:#3cb371'>Online</span>";
      p_status += "<p></p><p><p><p>"; 
      
      }else {
      p_status += "Sensor 6 :<span style='background-color:#ff0000'>Offline </span>";    
      p_status += "<p></p><p><p><p>"; 
      }  
        p_status +=  "<B>TEST_Check_Data</B><p>";  
      p_status += "<table> <colgroup><col span='1' style='background-color:red'>";
      p_status += "<col span='4'style='background-color:yellow'></colgroup>";      
      p_status += "<tr><th>Device..</th><th>Data1</th><th>Data2</th><th>Data3</th><th>Data4</th></tr>";
          
      p_status += "<tr>";
      p_status += "<td>";
      p_status += SerialNum;
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
      p_status += "<p><p><form method='POST' action='/reboot'><input type='submit'value='Reboot'></form>";         
      p_status += "</div>";
      p_status += "</body>";
      p_status += "</html>";            
      server.sendHeader("Connection", "close"); 
      server.send(200, "text/html", p_status);       });
    

    
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
        configwifi += "<a href='https://www.facebook.com/farmzmaster'>About</a>"; 
        configwifi += "</div><br>";
          
        configwifi += "<div style='text-align:left;display:inline-block;min-width:260px;'>";      
        configwifi += "<h2>Setup</h2>";           
        configwifi += "</p>";                       
        configwifi += "<form method='get' action='setting'>";         
        configwifi += "<label>SSID: </label><input id='s' name='ssid' length=20   placeholder='SSID'><br/><br>";     
        configwifi += "<label>PASS: </label><input id='p' name='pass' length=20 type='password' placeholder='password'><br/>";
        
        configwifi += "<br> NAME : <input type='text'name='farmname' placeholder='farmname'><br><br>";                  
        configwifi += "<label>Device1: </label><input name='device1'length=17 placeholder='SerialNumber'><br><br>";
        configwifi += "<label>Device2: </label><input name='device2'length=17 placeholder='SerialNumber'><br><br>";
        configwifi += "<label>Device3: </label><input name='device3'length=17 placeholder='SerialNumber'><br><br>";
        configwifi += "<label>Device4: </label><input name='device4'length=17 placeholder='SerialNumber'><br><br>";
        configwifi += "<label>Device5: </label><input name='device5'length=17 placeholder='SerialNumber'><br><br>";
        configwifi += "<label>Device6: </label><input name='device6'length=17 placeholder='SerialNumber'><br><br>";      
  
        
        configwifi += "<br/><button type='submit'>save</button></form>";   
        configwifi += "<br/><div class=\"c\"><a href=\"/\">Scan</a></div>";
        
        configwifi += "</div>";
        configwifi += "</body>";
        configwifi += "</html>";  
     
      server.sendHeader("Connection", "close");
      server.send(200, "text/html",configwifi);      

    });

    server.on("/setting", []() {    
     delay1= 700;
     delay2= 700;
     
     String FarmzName = server.arg("farmname");
     String qsid = server.arg("ssid");
     String qpass = server.arg("pass");
     String qdevice = server.arg("device"); 
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
          terminal_n("Save_Config");
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
            delay(100);
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
   

          content = " <html>";
          content += " <head>";
          content += " <title>Farmzmaster</title>";
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
      reboot += "<a href='https://www.facebook.com/farmzmaster'>About</a>"; 
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
      reconfig += "<a href='https://www.facebook.com/farmzmaster'>About</a>"; 
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
      update += "<a href='https://www.facebook.com/farmzmaster'>About</a>"; 
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



void Lora_Start1(String upLoad_Serial) {  
  String incoming ;  
  String incoming_serial ;
  String s_rssi ; 
  int rssi = 0;
  c_rssi; 
  SerialNum = "";
  data1 = ""; 
  data2 = "";  
  data3 = "";  
  data4 = "";   
  sender = 0;  
  Lora_Client =  true;   
  Serial.println("Send REQUEST");
  LoRa.beginPacket();  
  LoRa.print("SN="); 
  LoRa.print(upLoad_Serial); 
  LoRa.print(",");   
  LoRa.endPacket();   
  // end send request
  //start wait data
  long interval = 0; // รอสิบวิ
  while (LoRa.parsePacket() == 0) {
    if (interval > 10000) {
      Serial.println("Wait Data Timeout");  
      Lora_Client =  false;
      return;
    }
    interval++;
  }  
    
   rssi = LoRa.packetRssi();
   sender = LoRa.read();            // sender address  
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  String L1 = incoming.substring(incoming.indexOf("AL=") +3);
        incoming_serial = L1.substring(0,L1.indexOf(","));
        Serial.print("incoming_serial = ");
        Serial.print(incoming_serial);
        Serial.print(" : upLoad_Serial = ");
        Serial.println(upLoad_Serial);     
        delay(100);
  if (upLoad_Serial == incoming_serial) {            
    //Substring Data Process               
        // SERIAL NUMBER
        String L1 = incoming.substring(incoming.indexOf("AL=") +3);
        SerialNum = L1.substring(0,L1.indexOf(","));
        Serial.print(SerialNum);
        Serial.println();
        // CURRENT
        String L2 = incoming.substring(incoming.indexOf("le1=") +4);
        data1 = L2.substring(0,L2.indexOf(","));
      
        // AMMONIA
        String L3 = incoming.substring(incoming.indexOf("le2=") +4);
        data2 = L3.substring(0,L3.indexOf(","));
               
        // HUMIDITY
        String L4 = incoming.substring(incoming.indexOf("le3=") +4);
        data3 = L4.substring(0,L4.indexOf(","));
     
        // ++++      
        String L5 = incoming.substring(incoming.indexOf("le4=") +4);
        data4 = L5.substring(0,L5.indexOf(","));

        
       c_rssi = s_rssi + String(rssi);        
       oled_print(0,0,"SN");
       oled_print(0,2,"H");
       oled_print(0,3,"T");
       oled_print(0,4,"M"); 
       oled_print(0,6,"RSSI"); 
             
       oled_print(3,0,":");
       oled_print(3,2,":");
       oled_print(3,3,":");
       oled_print(3,4,":");
       oled_print(6,6,":");
       
       oled_print(4,0,SerialNum);
       oled_print(4,2,data1);
       oled_print(4,3,data2);
       oled_print(4,4,data3);
       oled_print(8,6,c_rssi);
       
       //oled_print(15,1,"");
       oled_print(10,2,"H'");
       oled_print(10,3,"C'");
       oled_print(10,4,"PPM");
       
     
  }else{
    return; 
  }
 }
 
/*ฟังชั่นอัพโหลดข้อมูลขึ้น server
 * เอาค่า string จาก lora มาแปลงเป็น const char 
 * publish ผู้ส่ง 
 * subscribe ผู้รับ 
 */


void upload_to_server(String serial_upload,String humidity,String temperature,String mq135,String airspeed,String rssi){
  String Message ;
  Message = serial_upload.c_str();
  Message += ",";
  Message += humidity.c_str();
  Message += ",";
  Message += temperature.c_str();
  Message += ",";
  Message += mq135.c_str();
  Message += ",";
  Message += airspeed.c_str();
  Message += ",";
  Message += rssi.c_str();
  if (!mqtt.connected()) {
   reconnect();
  }
  const char* c_message = Message.c_str();
        mqtt.publish(topic ,c_message); 
        Serial.println (Message);
        Serial.println("Mqtt : update");
  
  mqtt.loop(); 
 }

/*ฟังชั่นรีเซตการเชื่อมต่อ mqtt 
 * ถ้า mqtt ไม่ได้เชื่อม ให้ทำการเชื่อมต่อใหม่ } 
 */
void reconnect() {
  // Loop until we're reconnected
    mqtt.setServer(mqttServer, mqttPort);
    mqtt.setCallback(callback);
    
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
     while (WiFi.status() != WL_CONNECTED){
      testWifi();
     }
    // Attempt to connect
    if (mqtt.connect("ESPClient", mqttUser, mqttPassword )) {
      Serial.println("connected");
      // Subscribe
      mqtt.subscribe("Farmzmaster/sensor");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {   
  server.handleClient();  
  
  digitalWrite(23, HIGH);   
   buttonState = digitalRead(22);     
  if (buttonState == 1){     
  Serial.print(i);  
  digitalWrite(LED, 1);
  delay(1000);
  digitalWrite(LED, 0);  
  fillDisplay();
         
    i--;    
  if (i < 1){    
    Serial.print("SYS reset");  
  for (int i = 0; i < 150; ++i) { EEPROM.put(i, 0); }    
    EEPROM.commit();
    delay(1000);
    ESP.restart();
     }        
    }else {i = 4; 
   }   
  delay(10);
}
/*Task Loop1
 * ขั้นตอนการทำงาน ตามลับดับคือ
 * 1. เช็คถ้าตั้งค่าEdevice == 1 คือมี 1 อุปกรณ์ในเกตเวย์ตัวนี้
 * 2. เข้าฟังชั่น Lora_start คือส่งซีเรียลไปถามว่าอุปกรณ์ซีเรียลนี้ออนไลน์อยู่ไหมถ้าถ้ามีให้ส่งข้อมูลกลับมา 
 * 3. อัพโหลดขึ้น server mqtt ข้อมูลที่จะต้องอัพโหลด มี
 *    serial - ข้ออุปกรณ์ที่ติดตั้ง
 *    data1  - ค่าความชื้น
 *    data2  - ค่าอุณหภูมิ
 *    data3  - ค่าแอมโมเนีย
 *    data4  - ค่าแรงลม
 */
void loop1(void *p) {
  while (1) {   
    
  IPAddress ip_l = WiFi.localIP();    
  String ipStr_l = String(ip_l[0]) + '.' + String(ip_l[1]) + '.' + String(ip_l[2]) + '.' + String(ip_l[3]); 
  u8x8.clear();
  draw_1x2(3,0,"FARMZMASTER");
  oled_pre(1,3,"Local IP:");
  oled_pre(1,4,ipStr_l);
  delay(1000);   
  fillDisplay();        
    //ถ้า edevice คืออุปกรณ์ทั้งหมด = 1ตัวให้ถามอุปกรณ์แค่ตัวเดียวไป
  if (edevice.toInt()== 1){    
  Lora_Start1(SerialNumber1.c_str());
   //อัพเดทขึ้น server 
   if (Lora_Client == true){
         upload_to_server(SerialNumber1,data1,data2,data3,data4,c_rssi);            
         Status_Deviec1 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 }
  
 if (edevice.toInt()== 2){ 
   Lora_Start1(SerialNumber1.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber1,data1,data2,data3,data4,c_rssi);           
         Status_Deviec1 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }

   Lora_Start1(SerialNumber2.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber2,data1,data2,data3,data4,c_rssi);  
         Status_Deviec2 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 }
 if (edevice.toInt()== 3){  
 
    Lora_Start1(SerialNumber1.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber1,data1,data2,data3,data4,c_rssi);  
         Status_Deviec1 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }

   Lora_Start1(SerialNumber2.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber2,data1,data2,data3,data4,c_rssi);  
         Status_Deviec2 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 
   Lora_Start1(SerialNumber3.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber3,data1,data2,data3,data4,c_rssi);  
         Status_Deviec3 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 }
 
 if (edevice.toInt()== 4){  
 
    Lora_Start1(SerialNumber1.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber1,data1,data2,data3,data4,c_rssi);  
         Status_Deviec1 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }

   Lora_Start1(SerialNumber2.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber2,data1,data2,data3,data4,c_rssi);  
         Status_Deviec2 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 
   Lora_Start1(SerialNumber3.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber3,data1,data2,data3,data4,c_rssi);  
         Status_Deviec3 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 
   Lora_Start1(SerialNumber4.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber4,data1,data2,data3,data4,c_rssi);  
         Status_Deviec4 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  } 
 }
 
 if (edevice.toInt()== 5){    
    Lora_Start1(SerialNumber1.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber1,data1,data2,data3,data4,c_rssi);  
         Status_Deviec1 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }

   Lora_Start1(SerialNumber2.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber2,data1,data2,data3,data4,c_rssi);  
         Status_Deviec2 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 
   Lora_Start1(SerialNumber3.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber3,data1,data2,data3,data4,c_rssi);  
         Status_Deviec3 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 
   Lora_Start1(SerialNumber4.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber4,data1,data2,data3,data4,c_rssi);  
         Status_Deviec4 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 
 Lora_Start1(SerialNumber5.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber5,data1,data2,data3,data4,c_rssi);  
         Status_Deviec5 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }  
 } //device

  if (edevice.toInt()== 6){   
     Lora_Start1(SerialNumber1.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber1,data1,data2,data3,data4,c_rssi);  
         Status_Deviec1 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }

   Lora_Start1(SerialNumber2.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber2,data1,data2,data3,data4,c_rssi);  
         Status_Deviec2 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 
   Lora_Start1(SerialNumber3.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber3,data1,data2,data3,data4,c_rssi);  
         Status_Deviec3 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 
   Lora_Start1(SerialNumber4.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber4,data1,data2,data3,data4,c_rssi);  
         Status_Deviec4 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
 
 Lora_Start1(SerialNumber5.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber5,data1,data2,data3,data4,c_rssi);  
         Status_Deviec5 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }
  
 Lora_Start1(SerialNumber6.c_str());
   //อัพเดทขึ้น server    
   if (Lora_Client == true){
         upload_to_server(SerialNumber6,data1,data2,data3,data4,c_rssi);  
         Status_Deviec6 = 1;         
   delay(delay1); 
   }else{
   Serial.println ("client_offline");
  }

  } //device 
  vTaskDelay(1000 / portTICK_PERIOD_MS);  
 } //while
} //loop



void loop2(void *p) {
  while (1) {   
  vTaskDelay(50000 / portTICK_PERIOD_MS);
    } //while
 } //loop

 
/*Taxk testwifi
 * เชื่อมต่อไวไฟแลัวเข้า loop เช็คไวไฟ
 * ถ้าไวไฟไม่ได้เชื่อมต่อให้ทำการเชื่อมต่อใหม่
 * ถ้าไม่ได้เชื่อมต่อไวไฟให้ ไฟในบอร์ด GPIO2 ดับหรือไม่ให้ไฟติด
 * 
 * เชื่อมต่อ mqtt 
 */
void loop3(void *p) {          
   while(1) {     
    if(WiFi.status() != WL_CONNECTED) {
        digitalWrite(2, LOW);
        Serial.println("WiFi Disconnected.");
        WiFi.begin(esid.c_str(), epass.c_str()); 
      } else {
        digitalWrite(2, HIGH);
      } 
    vTaskDelay(5000 / portTICK_PERIOD_MS);             
  } //while
 }


