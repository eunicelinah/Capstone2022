
#include<SoftwareSerial.h>

#define START_BYTE 0x7e               //Starts every data packet 
#define NODE2 0x2                    //Address of the sensorNODE2 is 0010=2

#define Dout 2                     // Connect to RX (pin2) of arduino
#define Din 3                      // Connect to TX(pin3) of arduino

#define TransmitStatusLED 10   //blue

const int  trigger_pin = 5;
const int  echo_pin = 4;

int duration ;
int water_level;                    //IN CM

int8_t LevelDATA = 0; 
byte buf[3] = {START_BYTE,NODE2,0};

SoftwareSerial XBeeSerial(Dout, Din);


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
 
 Serial.begin(9600);
 XBeeSerial.begin(9600);
 pinMode(trigger_pin, OUTPUT);
 pinMode(echo_pin, INPUT);
 pinMode(TransmitStatusLED, OUTPUT);

  
  //Clear trigger pin
  digitalWrite(trigger_pin, LOW);
  digitalWrite(TransmitStatusLED,LOW);
  delayMicroseconds(1000);

}

void loop() {


digitalWrite(TransmitStatusLED,LOW);

digitalWrite(trigger_pin,HIGH);
delayMicroseconds(1000);
digitalWrite(trigger_pin, LOW);
delayMicroseconds(1000);
duration = pulseIn(echo_pin, HIGH);

water_level = (duration*0.034)/2;     //IN CM
Serial.println(water_level);

buf[2] = water_level;
XBeeSerial.write(buf,3);
flashLed(TransmitStatusLED,5,50);
delay(1000);

}
