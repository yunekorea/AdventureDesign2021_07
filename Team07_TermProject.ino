#include <MQ135.h>
#include "DHTStable.h" //DHTStable by Rob Tillaart

#include <Servo.h>

//DC Motor pin
#define DCOPEN_EN   38
#define DCOPEN_PWM  9
#define DCOPEN_DIR  39
#define DCCLOSE_EN  40
#define DCCLOSE_PWM 10
#define DCCLOSE_DIR 41

const int switchButton = ;

const int gasSensorIn = A2;
const int gasSensorOut = A3;
//const int rainDropSensor = ;
const int tempHumidSensorIn = 5;
const int tempHumidSensorOut = 6;
const int IRSensorOpen = 3;
const int IRSensorClose = 4;

//const int servoMotor = ;

float gasIn = 0;
float gasOut = 0;
float tempIn = 0;
float tempOut = 0;
float humidIn = 0;
float humidOut = 0;
int rainDrop = 0;
int IROpen = 0;
int IRClose = 0;

MQ135 mq135In(gasSensorIn);
MQ135 mq135Out(gasSensorOut);
DHTStable DHTin;
DHTStable DHTout;

Servo windowServo;    //Servo motor that controls the window

void SensorInit(void) {
  //gas Sensor init
  
  //RainDrop Sensor init

  //Temp/Humid Sensor init
  
  //IRSensor init
  pinMode(IRSensorOpen, INPUT);
  pinMode(IRSensorClose, INPUT);
}

void MotorInit(void) {
  //initializing DC motor
    pinMode(DCOPEN_EN, OUTPUT);
  pinMode(DCOPEN_PWM, OUTPUT);
  pinMode(DCOPEN_DIR, OUTPUT);
  pinMode(DCCLOSE_EN, OUTPUT);
  pinMode(DCCLOSE_PWM, OUTPUT);
  pinMode(DCCLOSE_DIR, OUTPUT);
  digitalWrite(DCOPEN_EN, HIGH);
  digitalWrite(DCCLOSE_EN, HIGH);
}



void SensorData() {
  //getting data from each sensors

  //DHT11 data read
  DHTin.read11(tempHumidSensorIn);
  DHTout.read11(tempHumidSensorOut);
  //get temp
  tempIn = DHTin.getTemperature();
  tempOut = DHTout.getTemperature();
  //get humidity
  humidIn = DHTin.getHumidity();
  humidOut = DHTout.getHumidity();
  
  //get and correct the gas data based on temperature and humidity
  gasIn = mq135In.getCorrectedPPM(tempIn, humidIn);
  gasOut = mq135Out.getCorrectedPPM(tempOut, humidOut);

  //IRSensor read
  IROpen = digitalRead(IRSensorOpen);
  IRClose = digitalRead(IRSensorClose);
}

void openWindow(){
  digitalWrite(DCOPEN_DIR, HIGH); // 정방향 회전
  //int pwm_value = 255 - speed;
  while(digitalRead(IRSensorOpen) ||
        !digitalRead(IRSensorClose))//열려있지 않거나 닫혀있는 동안 
    analogWrite(DCOPEN_PWM, 0);
  analogWrite(DCOPEN_PWM, 255);
}

void CloseWindow(void) {
  digitalWrite(DCCLOSE_DIR, HIGH);
  while(digitalRead(IRSensorClose) ||
        !digitalRead(IRSensorOpen))//닫혀있지 않거나 열려있는 동안 
    analogWrite(DCCLOSE_PWM,0);
  analogWrite(DCCLOSE_PWM, 255);
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
  
  Serial.println("temperature inside  : " + String(tempIn));
  Serial.println("humidity inside     : " + String(humidIn));
  Serial.println("gas inside          : " + String(gasIn));
  Serial.println("-------------------------------------");
  Serial.println("temperature outside : " + String(tempOut));
  Serial.println("humidity outside    : " + String(humidOut));
  Serial.println("gas outside         : " + String(gasOut));
  Serial.println("=====================================\n");
  if(rainDrop) {        //raining
    if(!IR)             //Window is open
      CloseWindow();
  } else {              //not raining
    //implement
  }
  delay(2000);
}
