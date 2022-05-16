
#include <WiFiClient.h>
#include <HTTPClient.h>

//-------------------------------------------------------------------

#define TempStatusLED 22         //GREEN
#define LevelStatusLED 23        //GREEN

#define START_BYTE 0X7e
#define NODE1 0x01              //Temperature sensor node 
#define NODE2 0x02              // Level sensor node
#define NODE3 0X03              // Pressure sensor node 

// DEFINE CONSTANTS 
#define  RX1 16                  //Xbee to ESP32 RX 
#define  TX1 17                  //Xbee to ESP32 TX


byte buf[3] = {0,0,0};                 //Variable to hold incoming byte of data which includes start byte,node address and sensor data
uint8_t TEMP = 0;                      //Variable to hold temperature sensor data
uint8_t LEVEL = 0;                     //Variable to hold LEVEL sensor data
uint8_t PRESSURE = 0;                  //Variable to hold Pressure sensor data 
uint8_t Tempp = 0;

const int TempRELAY =  27;                   //Relay input to ESP32 for temperature 
const int LevelRELAY = 26;                  //Relay input to ESP32 for level 
const int PressRELAY = 25;                  //Relay input to ESP32 for pressure  (2-channel relay)
 
//--------------------------------------------------------------------

const char* ssid     = " ";                     // Wifi network name 
const char* password = "  ";                    // Wifi password

//-------------------------------------------------------------------

const char* SERVER_NAME1 = "http://finalcapstone.atwebpages.com/tempdata.php";
const char* SERVER_NAME2 = "http://finalcapstone.atwebpages.com/leveldata.php";
const char* SERVER_NAME3 = "http://finalcapstone.atwebpages.com/pressuredata.php";

String PROJECT_API_KEY = "W1lls@22";
//-------------------------------------------------------------------
//Send an HTTP POST request every 1 seconds
unsigned long lastMillis = 0;
long interval = 1000;
//-------------------------------------------------------------------

void setup() {
  
  //-----------------------------------------------------------------
  Serial.begin(115200);
  Serial2.begin(9600,SERIAL_8N1,RX1,TX1); 
  Serial.println("esp32 serial initialize");
  //-----------------------------------------------------------------
  pinMode(TempStatusLED, OUTPUT);
  pinMode(LevelStatusLED,OUTPUT);

  pinMode(TempRELAY,  OUTPUT);
  pinMode(LevelRELAY, OUTPUT);
  pinMode(PressRELAY, OUTPUT);

  digitalWrite(TempStatusLED, LOW);
  digitalWrite(LevelStatusLED, LOW);

  digitalWrite(TempRELAY, LOW);
  digitalWrite(LevelRELAY, LOW);
  digitalWrite(PressRELAY, HIGH);
  //-----------------------------------------------------------------
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
}


void loop() {
  
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    if(millis() - lastMillis > interval) {
       //Send an HTTP POST request every  second
       upload_data();
       lastMillis = millis();
    }
  }
  //-----------------------------------------------------------------
  else {
    Serial.println("WiFi Disconnected");
  }
  //-----------------------------------------------------------------

       
}



void upload_data()
{
  
  while(Serial2.available()>=3) {

  //Read the next available byte in the serial receive buffer
  Serial2.readBytes(buf,3); 

      // TEMPERATURE SENSOR MONITORING AND CONTROLLING 
      
      if ((buf[0]==START_BYTE) && (buf[1] == NODE1)){
      TEMP = buf[2];
     // Serial.print("TempVAUE:");
      Serial.println(TEMP,DEC);
      digitalWrite(TempStatusLED,HIGH);


      //HTTP POST request data
     String temperature_data;
     temperature_data = "api_key="+PROJECT_API_KEY;
     temperature_data += "&temperature="+String(TEMP);

     Serial.print("temperature_data: ");
     Serial.println(temperature_data);
     //--------------------------------------------------------------------------------
  
    WiFiClient client;
    HTTPClient http;

    http.begin(client, SERVER_NAME1);
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Send HTTP POST request
    int httpResponseCode = http.POST(temperature_data);
 
     //--------------------------------------------------------------------------------
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();

    // CONDITION FOR CONTROLLING THE PLC 
    
    if (TEMP >= 40){
      //TURN ON INPUT OF PLC 
      digitalWrite(TempRELAY,HIGH);           //1-CHANNEL RELAY
      }

     else{
      //TURN OFF INPUT OF PLC 
      digitalWrite(TempRELAY,LOW);           //1-CHANNEL RELAY
      }
      }


      // LEVEL SENSOR DATA FOR MONITORING AND CONTROLLING 
      else if ((buf[0]==START_BYTE) && (buf[1] == NODE2)){
      LEVEL= buf[2];
      //Serial.print("LevelVALUE:");
      Serial.println(LEVEL,DEC); 
      digitalWrite(LevelStatusLED,HIGH); 

      //HTTP POST request data
      String level_data;
      level_data = "api_key="+PROJECT_API_KEY;
      level_data += "&level="+String(LEVEL);
      //String(LEVEL);


      Serial.print("level_data: ");
      Serial.println(level_data);
 
  
      WiFiClient client;
      HTTPClient http;

      http.begin(client, SERVER_NAME2);
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Send HTTP POST request
      int httpResponseCode = http.POST(level_data);
 
      //--------------------------------------------------------------------------------
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    
      // Free resources
      http.end();

          // CONDITION FOR CONTROLLING THE PLC 
    if (LEVEL >= 20){
      //TURN ON INPUT OF PLC 
      digitalWrite(LevelRELAY,HIGH);           //1-CHANNEL RELAY
      }

     else{
      //TURN OFF INPUT OF PLC 
      digitalWrite(LevelRELAY,LOW);           //1-CHANNEL RELAY
      }
        }

      // PRESSURE SENSOR DATA FOR MONITORING AND CONTROLLING 
      else if ((buf[0]==START_BYTE) && (buf[1] == NODE3)){
      PRESSURE= buf[2];
      //Serial.print("LevelVALUE:");
      Serial.println(PRESSURE,DEC); 
  

      //HTTP POST request data
      String pressure_data;
      pressure_data = "api_key="+PROJECT_API_KEY;
      pressure_data += "&pressure="+String(PRESSURE);
      //String(LEVEL);


      Serial.print("pressure_data: ");
      Serial.println(pressure_data);
 
  
      WiFiClient client;
      HTTPClient http;

      http.begin(client, SERVER_NAME3);
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Send HTTP POST request
      int httpResponseCode = http.POST(pressure_data);
 
      //--------------------------------------------------------------------------------
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    
      // Free resources
      http.end();

     // CONDITION FOR CONTROLLING THE PLC 
    if (PRESSURE >= 0){
      //TURN ON INPUT OF PLC 
      digitalWrite(PressRELAY,LOW);           //1-CHANNEL RELAY
      }

     else{
      //TURN OFF INPUT OF PLC 
      digitalWrite(PressRELAY,LOW);           //1-CHANNEL RELAY
      }
        }
      
     
      //SERIAL SENDS WRONG DATA 
      else{
        Serial.println("Wrong data"); 
        digitalWrite(TempRELAY,LOW);  
        digitalWrite(LevelRELAY,LOW);
        digitalWrite(PressRELAY,HIGH);    
        } 
      

}
         
  }
