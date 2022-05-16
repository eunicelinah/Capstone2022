#include <SoftwareSerial.h>

#include <DallasTemperature.h>              //Contains commands for getting temperature readings from the sensor
#include <OneWire.h>                        //Used for communication with one wire devices 

#define Dout 2    //TX
#define Din 3     //RX

#define TransmitStatusLED 10                       //BLUE


const int  Temp_sensor = 5 ;                      // D5 of arduino
int deg_Celcius = 0;

SoftwareSerial XBeeSerial (Dout, Din);          //RX of xbee goes to pin2, TX  of xbee goes to pin3 of arduino

OneWire mySensor(Temp_sensor);              // onewire instance to communicate with the sensor 
DallasTemperature  Sensor(&mySensor);       // Passing the onewire reference to the Dallas Temperature library


void flashLed(int pin, int times, int wait) {

  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}


void setup() {  
XBeeSerial.begin(9600);
Serial.begin(9600);
Sensor.begin();

pinMode(TransmitStatusLED, OUTPUT);
digitalWrite(TransmitStatusLED,LOW);
}

void loop() {

//delay(500);
digitalWrite(TransmitStatusLED,LOW);


Sensor.requestTemperatures();
deg_Celcius = Sensor.getTempCByIndex(0);
Serial.println(deg_Celcius);

XBeeSerial.write(deg_Celcius);
Serial.println("TempSENT");
flashLed(TransmitStatusLED,5,50);
delay(1000);

}
