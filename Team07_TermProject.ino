#include <Servo.h>

const int gasSensor = ;
const int rainDropSensor = ;
const int tempSensor  = ;
const int humidSensor = ;
const int IRSensor = ;
const int servoMotor = ;

int gas = 0;
int rainDrop = 0;
int temp = 0;
int humid = 0;
int IR = 0;

Servo windowServo;    //Servo motor that controls the window

void SensorInit(void) {
  //gas Sensor init
  
  //RainDrop Sensor init

  //Temp Sensor init

  //humid Sensor init

  //IRSensor init
  pinMode(IRSensor, INPUT);
}

void MotorInit(void) {
  //initializing servo or DC motor
  
}

void SensorData(int *gas, int* rain, int* temp, int* humid, int* IR) {
  //getting data from each sensors
}

void OpenWindow(void) {
  
}

void CloseWindow(void) {
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  windowServo.attatch(servoMotor);

  SensorInit();
  MotorInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  SensorData(&gas, &rainDrop, &temp, &humid, &IR);
  if(rainDrop) {        //raining
    if(!IR)             //Window is open
      CloseWindow();
  } else {              //not raining
    //implement
  }
}
