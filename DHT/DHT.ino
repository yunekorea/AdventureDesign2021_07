#include <SoftwareSerial.h>
#include <MQ135.h>
#include <LiquidCrystal.h>
#include <DHTStable.h>

//UART
SoftwareSerial mySerial(9,10);


// Buzzer Note
#define NOTE_G4 392

// DHT sensors
const int tempHumidSensorIn = 5;
const int tempHumidSensorOut = 6;

// IR Sensor
int IRSensorOpen = 3;
int IRSensorClose = 4;

// RGB LED
int RGB_LED[] = {26, 27, 28};

// Gas sensors
const int gasSensorIn = A2;
const int gasSensorOut = A3;

// Rain sensors
const int rainDropSensor = A0;

// Switch button pins
const int transferControlButton = 11; // TCB
const int controlWindowButton = 12;

// Speaker pin
const int speakerPin = A6;

// Global variables
float tempIn = 0, tempOut = 0;
float humidIn = 0, humidOut = 0;
float gasIn = 0, gasOut = 0;
bool isRaining = false;
bool isAutoMode = true;
bool transferControlButtonCurrentState = true, transferControlButtonPreviousState = false; // Prevent multiple clicks
bool controlWindowButtonCurrentState = true, controlWindowButtonPreviousState = false; // Prevent multiple clicks

unsigned long previousTime, currentTime; // For delaying DHT read
int count = 0; // Lets ht_read run without waiting for 2000ms

// Gas sensor objects
MQ135 mq135In(gasSensorIn);
MQ135 mq135Out(gasSensorOut);

// DHT sensor objects
DHTStable DHTin; // Inside the building
DHTStable DHTout; // Outside the building

// LCD object
// 핀 번호 (RS, E, DB4, DB5, DB6, DB7)
LiquidCrystal lcd(44, 45, 46, 47, 48, 49); // LCD 연결

/*
   Function to print logs
   Accepts a log_message and a value to print. Can also only accept log message.
*/
void Log(String log_message, String value = "") { // Function to print logs
  Serial.println(log_message + value + '\n');
}

/*
   Dangerous air quality
*/
bool is_gas_outlier() {
  if (gasIn > 20) {
    buzzer_alert();
    Log("Gas value is outlier");
    open_window();
    return true;
  }
  else return false;
}

/*
   Controls window depending on gas sensor values
*/


/*
   Automatically control window depending on humidity and temperature values
*/
void auto_control_temperature() {

  Log("Temperature in: ", String(tempIn));

  if (read_temperature_states(tempIn) == 1 && read_temperature_states(tempOut)==1) {
    Log("Temperature is normal");
    if (is_humid_in() == true) {
      open_window();
    }
  }
  else if (read_temperature_states(tempOut) == 1) {
    Log("Outside temperature is normal");
    open_window();
  } else if (read_temperature_states(tempOut) != 1) {
    Log ("Outside temperature is not normal");
    if (is_humid_in == true) {
      open_window();
    }
  }
}

/*
   Checks if outside is more humid that the inside
*/
bool is_humid_in() {
  bool isMoreHumidIn = false;

  if (humidIn-humidOut>15) {
    isMoreHumidIn = true;
  }

  return isMoreHumidIn;
}
/*
   Window auto mode
*/
void auto_window_control() {
  if (!(is_raining && is_gas_outlier)) {
    auto_control_temperature();
  }
}

/*
   Window manual mode
*/
void manual_window_control() {
  if (is_window_open() == true) {
    close_window();
  }
  else open_window();
}

/*
   Transfers control to manual if auto and vice versa
*/
void transfer_control() {
  Log("Current mode is: ", String(isAutoMode));

  if (isAutoMode) {
    auto_window_control();
  }
  else {
    manual_window_control();
  }
}

/*
   Checks if its raining
*/
bool is_raining() {
  if (!isRaining) return true;
  else return false;
}

/*
   Alerts with buzzer sound.
*/
void buzzer_alert() {
  tone(speakerPin, NOTE_G4, 2000);
}

/*
   Returns true if window open, false if closed.
*/
bool is_window_open() {
  if (digitalRead(IRSensorOpen) ||
      !digitalRead(IRSensorClose)) {
    return false;
  }
  else if (!digitalRead(IRSensorOpen) ||
           digitalRead(IRSensorClose)) {
    return true;
  }
  //  return false;
}

/*
   Read humidity and temperature values
*/
void ht_read() {
  // start reading DHT data
  DHTin.read11(tempHumidSensorIn);
  DHTout.read11(tempHumidSensorOut);

  // get temperature data
  tempIn = DHTin.getTemperature()-4;
  tempOut = DHTout.getTemperature()-5;

  // get humidity data
  humidIn = DHTin.getHumidity();
  humidOut = DHTout.getHumidity();
}

/*
   Read gas data based on temperature and humidity
*/
void gas_read() {
  gasIn = mq135In.getCorrectedPPM(tempIn, humidIn);
  gasOut = mq135Out.getCorrectedPPM(tempOut, humidOut);
}

/*
   Read rain sensor data
*/
void rain_read() {
  isRaining = digitalRead(rainDropSensor);
}

/*
   Read sensor values. Calls respective functions for each type of sensor.
*/
void read_sensor(void) {
  currentTime = millis();
  if (currentTime - previousTime >= 2000 || !count) { // DHT tends to mess up if read too quickly, give 2 sec delay
    count = 1;
    previousTime = currentTime;
    ht_read();
  }
  gas_read();
  rain_read();
}

/*
   Separate temperature into 3 separate states. This is mainly used by the RGB LED.
   Divide temperature into 0:cold, 1:moderate, 2:hot
*/
int read_temperature_states(float _temperature) {
  if (_temperature < 18.00) {
    return 0;
  }
  else if (_temperature >= 18.00 && _temperature <= 24.00) {
    return 1;
  }
  else if (_temperature > 24.00) {
    return 2;
  }
}

/*
   This will display the current temperature in LCD. No parameters necessary.
*/
void display_temperature_LCD() {
  lcd.setCursor(0, 0);
  lcd.write("Temperature");
  lcd.setCursor(0, 2);
  lcd.print(tempIn);
  lcd.setCursor(5, 2);
  lcd.write(byte(0));
}

/*
   Sets the RGB color depending on the temperature state.
*/
void set_RGB_color() {
  int setting = read_temperature_states(tempOut);
  int colorValue = map(tempOut, 0, 18, 255, 120);

  switch (setting) {
    case 0:
      colorValue = map(tempOut, 0, 18, 255, 120);
      analogWrite(RGB_LED[0], 0);
      analogWrite(RGB_LED[1], 0);
      analogWrite(RGB_LED[2], colorValue);
      break;
    case 1:
      colorValue = map(tempOut, 18, 25, 255, 180);
      analogWrite(RGB_LED[0], 0);
      analogWrite(RGB_LED[1], colorValue);
      analogWrite(RGB_LED[2], 0);
      break;
    case 2:
      colorValue = map(tempOut, 25.1, 38, 120, 255);
      analogWrite(RGB_LED[0], colorValue);
      analogWrite(RGB_LED[1], 0);
      analogWrite(RGB_LED[2], 0);
      break;
  }
}



/*
   Opens the window
*/
void open_window() {
  while(is_window_open() == false){
    mySerial.write('1');
  }
  mySerial.write('3');
}

/*
   Closes the window
*/
void close_window() {
  while(is_window_open()== true){
    mySerial.write('2');
  }
  mySerial.write('3');
}



/*
 * Initialize LCD with texts
 */
void LCDInit(void) {
  byte celsius[8] = {B00000, B11000, B11011, B00100, B01000, B00100, B00011, B00000};
  lcd.createChar(0, celsius);
  lcd.begin(16, 2); // LCD Init
  lcd.clear();
}

/*
 * Initialize switch pins
 */
void ButtonInit(void) {
  pinMode(transferControlButton, INPUT);
  pinMode(controlWindowButton, INPUT);
}

/*
 * Initialize sensor pins
 */
void SensorInit(void) {
  // IR Sensor
  pinMode(IRSensorOpen, INPUT);
  pinMode(IRSensorClose, INPUT);

  // Raindrop Sensor
  pinMode(rainDropSensor, INPUT);
}

void setup() {
  Serial.begin(9600);
  // board2
  mySerial.begin(9600);
  // rgb
  for (int i = 0; i < 3; i++) {
    pinMode(RGB_LED[i], OUTPUT);
  }
  LCDInit();
  ButtonInit();
  SensorInit();
  previousTime = millis();
}

/*
 * Just to make main function look cleaner. This prevents multiple switch clicks.
 */
void prevent_multiple_click() {
  transferControlButtonCurrentState = digitalRead(transferControlButton);
  controlWindowButtonCurrentState = digitalRead(controlWindowButton);

  if (controlWindowButtonCurrentState) {
    if (controlWindowButtonPreviousState == false) {
      Log("Control mode set to manual");
      isAutoMode = false;
      manual_window_control();
      controlWindowButtonPreviousState = true;
    }
  } else controlWindowButtonPreviousState = false;

  if (transferControlButtonCurrentState) { // 버튼을 누른 경우
    if (transferControlButtonPreviousState == false) { // 이전 상태와 비교
      isAutoMode = !isAutoMode;
      transferControlButtonPreviousState = true;
    }
  } else transferControlButtonPreviousState = false;

}

void loop() {
  
  prevent_multiple_click();
  read_sensor(); // Read humidity and temperature
  display_temperature_LCD(); // Display current temperature in LCD
  set_RGB_color(); // Set RGB LED
  transfer_control();

  delay(50);
}
