#include <MQ135.h>
#include "DHTStable.h" //DHTStable by Rob Tillaart

#include <Servo.h>

//Function prototypes
void SensorInit();
void MotorInit();
void SensorData();
void OpenWindow();
void CloseWindow();

//DC Motor pin
#define DCOPEN_EN   38
#define DCOPEN_PWM  9
#define DCOPEN_DIR  39
#define DCCLOSE_EN  40
#define DCCLOSE_PWM 10
#define DCCLOSE_DIR 41

const int switchButton1 = ;
const int switchButton2 = ;

const int gasSensorIn = A2;
const int gasSensorOut = A3;
//const int rainDropSensor = ;
const int tempHumidSensorIn = 5;
const int tempHumidSensorOut = 6;
const int IRSensorOpen = 3;
const int IRSensorClose = 4;

//variables storing sensor values
float gasIn = 0;
float gasOut = 0;
float tempIn = 0;
float tempOut = 0;
float humidIn = 0;
float humidOut = 0;
int rainDrop = 0;
int IROpen = 0;
int IRClose = 0;
int button1 = 0;
int button2 = 0;

//an outlier value for the gas sensor in the room
const int gasInOutlier = 500;

//automatically control the window based on the sensor values
// 1 : automatic
// 0 : manual
int autoControl = 1;

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

  //Button Init
  pinMode(switchButton1, INPUT);
  pinMode(switchButton2, INPUT);
  
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

  //Button read
  button1 = digitalRead(switchButton1);
  button2 = digitalRead(switchButton2);
  
}

void OpenWindow(){
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

//A function for the buttons
void AutoManualTrans() {
  if(button1) {           //button1 -> switch to manual
    autoControl = 0;
    if(IROpen == 1)       //open or close the window
      CloseWindow();
    else if(IRClose == 1)
      OpenWindow();
  }
  if(button2) {           //button2 -> switch to automatic
    autoControl = 1;
  }
}


//Automatically controls the window
//based on the sensor values
void AutoWindowControl() {
  //내부 대기질 임계점 이상 : 창문열기 
  if(gasIn > gasInOutlier) {
    OpenWindow();
    return;
  }

  //빗물 감지 : 창문닫기 
  if(rainDrop == 1) {
    CloseWindow();
    return;
  }

  //온도확인, 이후에 습도확인 
  if(18 <= tempIn <= 25) {  //적정 온도
    if(humidIn - humidOut > 5) {  //실내 습도 > 실외 습도
      OpenWindow();
      return;
    }
  }else if(18 <= tempOut <= 25) {//실외가 적정 온도일 경우
    OpenWindow();
    return;
  }
  else if(humidIn - humidOut > 5) {//실내외 모두 적정온도 아님
    OpenWindow();                  //실내 습도 > 실외 습도
    return;
  }
  
  //위의 모든 조건에서 정상 결과일 경우 : 창문닫기 
  CloseWindow();
}

/* #### main part #### */

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

  AutoManualTrans();
  
  if(autoControl)
    AutoWindowControl();

  Serial.println("temperature inside  : " + String(tempIn));
  Serial.println("humidity inside     : " + String(humidIn));
  Serial.println("gas inside          : " + String(gasIn));
  Serial.println("-------------------------------------");
  Serial.println("temperature outside : " + String(tempOut));
  Serial.println("humidity outside    : " + String(humidOut));
  Serial.println("gas outside         : " + String(gasOut));
  Serial.println("=====================================\n");
  delay(2000);
}
