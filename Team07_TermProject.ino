#include <MQ135.h>
#include "DHTStable.h" //DHTStable by Rob Tillaart

#include <Servo.h>

const int gasSensor = A2;
//const int rainDropSensor = ;
const int tempHumidSensor  = 5;
//const int IRSensor = ;
//const int servoMotor = ;

float gas = 0;
int rainDrop = 0;
float temp = 0;
float humid = 0;
int IR = 0;

MQ135 mq135Sensor(gasSensor);
DHTStable DHT;

Servo windowServo;    //Servo motor that controls the window

void SensorInit(void) {
  //gas Sensor init
  
  //RainDrop Sensor init

  //Temp Sensor init
  
  //humid Sensor init
  
  //IRSensor init
  //pinMode(IRSensor, INPUT);
}

void MotorInit(void) {
  //initializing servo or DC motor
  
}

void SensorData() {
  //getting data from each sensors

  //DHT11 data read
  DHT.read11(tempHumidSensor);
  //get temp
  temp = DHT.getTemperature();
  //get humidity
  humid = DHT.getHumidity();
  //get and correct the gas data based on temperature and humidity
  gas = mq135Sensor.getCorrectedPPM(temp, humid);
}

void OpenWindow(void) {
  
}

void CloseWindow(void) {
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //windowServo.attatch(servoMotor);

  SensorInit();
  MotorInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  SensorData();
  
  Serial.println("temperature : " + String(temp));
  Serial.println("humidity : " + String(humid));
  Serial.println("gas : " + String(gas));
  if(rainDrop) {        //raining
    if(!IR)             //Window is open
      CloseWindow();
  } else {              //not raining
    //implement
  }
  delay(2000);
}
