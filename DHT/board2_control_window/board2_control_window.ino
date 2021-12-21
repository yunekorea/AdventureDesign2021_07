//DC Motor pin
#define DCOPEN_EN   38
#define DCOPEN_PWM  9
#define DCOPEN_DIR  39
#define DCCLOSE_EN  40
#define DCCLOSE_PWM 10
#define DCCLOSE_DIR 41


void Log(String log_message, String value = "") { // Function to print logs
  Serial.println(log_message + value + '\n');
}
/*
 * Initialize dual motors
 */
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
/*
   Opens the window
*/
void open_window() {
  digitalWrite(DCOPEN_DIR, HIGH);
  while (Serial1.read() != '3') {
    Log("Opening window");
    analogWrite(DCOPEN_PWM, 0);
  }
  analogWrite(DCOPEN_PWM, 255);
}

  

/*
   Closes the window
*/
void close_window() {
  digitalWrite(DCCLOSE_DIR, HIGH);
  while (Serial1.read() != '3') {
    Log("Closing window");
    analogWrite(DCCLOSE_PWM, 0);
  }
  analogWrite(DCCLOSE_PWM, 255);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  MotorInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available()){
    char data = Serial1.read();
    if(data=='1'){
      open_window();
    }else if(data=='2')
      close_window();
  }
}
