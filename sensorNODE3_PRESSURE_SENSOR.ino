

#define START_BYTE 0x7e                //Starts every data packet
#define NODE3 0x03
#define TransmitStatusLED 10           //BLUE  

const int  pressure_sensor = A0;      //analog input pin for pressure sensor
const int pressure_zero = 102.4;       //analog reading at 0psi (x/1024 = 0.5/5) at 0.5V
const int pressure_hundred = 921.6 ;   //analog reading at 100psi (x/1024 = 4.5/5) at 4.5V
const int pressure_PSImax = 100;         // psi of pressure se sensor
// assuming the relation between voltage and psi is linear 

float pressure_value = 0;

byte buf[] = {START_BYTE,NODE3,0};

//FUNCTION TO BLINK LEDS 
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
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(TransmitStatusLED, OUTPUT);

 digitalWrite(TransmitStatusLED,LOW

}

void loop() {
  // put your main code here, to run repeatedly:
  pressure_value = analogRead(pressure_sensor);
  pressure_value = ((pressure_value - pressure_zero)*pressure_PSImax)/(pressure_hundred -pressure_zero);      // convert to psi
  buf[2]= pressure_value;

for(int i=0;i<3;i++){
      Serial.print(buf[i],HEX);
      Serial.print(",");
         }
         Serial.println();


Serial.write(buf,3);
flashLed(TransmitStatusLED,5,50);
delay(1000);

  
}
