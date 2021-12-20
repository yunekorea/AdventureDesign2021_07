#include <Pitch.h>

#include <MQ135.h>
#include "DHTStable.h" //DHTStable by Rob Tillaart

#include <Servo.h>

const int gasSensorIn = A2;
const int gasSensorOut = A3;
const int rainDropSensor = A0;
const int tempHumidSensorIn = 5;
const int tempHumidSensorOut = 6;
const int speakerPin = A6;
//const int IRSensor = ;
//const int servoMotor = ;

float gasIn = 0;
float gasOut = 0;
float tempIn = 0;
float tempOut = 0;
float humidIn = 0;
float humidOut = 0;
int rainDrop = A0;
int IR = 0;

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
  //pinMode(IRSensor, INPUT);
}

void MotorInit(void) {
  //initializing servo or DC motor
  
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
}
void alert(){
  tone(speakerPin,)
}
void OpenWindow(void) {
  
}

void CloseWindow(void) {
  
}

boolean is_outlier(float gasIn){
  if(gasIn>20){
    CloseWindow();
    return true;
  }
  else{return false;}
}
void is_openAirQuality(float gasIn, float gasOut){
  if(gasIn-gasOut<-10){
    return CloseWindow();
  }
  else(gasIn-gasOut>10){
    return OpenWindow();
  }
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
