#include <LiquidCrystal.h>
#include <DHTStable.h>

const int tempHumidSensorIn = 5;
const int tempHumidSensorOut = 6;

DHTStable DHTin; // Inside the building
DHTStable DHTout; // Outside the building

// 핀 번호 (RS, E, DB4, DB5, DB6, DB7)
LiquidCrystal lcd(44, 45, 46, 47, 48, 49); // LCD 연결

// Motors
int Enable1 = 38; // 모터 1
int PWM1 = 9;
int DIR1 = 39;
int Enable2 = 40; // 모터 2
int PWM2 = 10;
int DIR2 = 41;

// IR Sensor
int IROpen = 3;
int IRClose= 4;

// RGB LED
int RGB_LED[] = {6, 7, 8};

// Global variables
float tempIn = 0, tempOut = 0;
float humidIn = 0, humidOut = 0;
bool isWindowOpen = false;

/*
 * Function to print logs
 * Accepts a log_message and a value to print. Can also only accept log message.
 */
void Log(String log_message, String value = "") { // Function to print logs
  Serial.println(log_message + value + '\n');
}

/*
 * Read humidity and temperature values
 */
void ht_read() { 
  // start reading DHT data
  DHTin.read11(tempHumidSensorIn);
  DHTout.read11(tempHumidSensorOut);

  // get temperature data
  tempIn = DHTin.getTemperature();
  tempOut = DHTout.getTemperature();

  // get humidity data
  humidIn = DHTin.getHumidity();
  humidOut = DHTout.getHumidity();
}

/*
 * Separate temperature into 3 separate states. This is mainly used by the RGB LED.
 * Divide temperature into 0:cold, 1:moderate, 2:hot
 */
int read_temperature_states(float _temperature) { 

  if (_temperature < 18.00) {
    return 0;
  }
  else if (_temperature >= 18.00 && _temperature <= 25.00) {
    return 1;
  }
  else if (_temperature > 25.00) {
    return 2;
  }

}

/*
 * This will display the current temperature in LCD. No parameters necessary.
 */
void display_temperature_LCD() {
  lcd.print(tempIn);
}

/*
 * Sets the RGB color depending on the temperature state.
 */
void set_RGB_color{
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
 * Checks if outside is more humid that the inside
 */
bool is_humid_out() { 
  bool isMoreHumidOut = true;

  if (humidOut < humidIn) {
    isMoreHumidOut = false;
  }

  return isMoreHumidOut;
}


/*
 * Opens the window
 */
void open_window() { // Open the window
  if (isWindowOpen == false) {
    isWindowOpen = true;
    Log("Opening window");
  }
}

/*
 * Closes the window
 */
void close_window() { // Close the window
  if (isWindowOpen == true) {
    isWindowOpen = false;
    Log("Closing window");
  }
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(RGB_LED[i], OUTPUT);
  }
  lcd.begin(16, 2); // LCD Init
  lcd.setCursor(0, 0);
  lcd.write("Current Temp");
  lcd.setCursor(0, 1);
}

void loop() {

  ht_read();
  display_temperature_LCD();
  set_RGB_color();

  Log("Temperature in: ", String(tempIn));

  if (read_temperature_states(tempIn) == 1) {
    Log("Temperature is normal");
    if (is_humid_out() == false) {
      open_window();
    }
  }
  else if (read_temperature_states(tempOut) == 1) {
    Log("Outside temperature is normal");
    open_window();
  }

  delay(2000);
}
