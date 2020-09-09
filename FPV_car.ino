#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(A0, 10);   // nRF24L01 (CE, CSN)
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
#define PWMB 9
#define BIN2 8
#define BIN1 7
#define STBY 6
#define AIN1 5
#define AIN2 4
#define PWMA 3
int motorSpeedA = 0;
int motorSpeedB = 0;
int  xAxis, yAxis;
const byte address[6] = "00001";
int led = 2;
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
};
Data_Package data; //Create a variable with the above structure
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();

  Serial.println("ON UART");
  pinMode(led, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  digitalWrite(STBY, 1);
  digitalWrite(led, 1);
  delay(500);
  digitalWrite(led,0);
  

}

void loop() {
  // put your main code here, to run repeatedly:
 if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }
  xAxis = data.j1PotX;
  yAxis = data.j1PotY;
  if (yAxis < 70) {
    // Set Motor A backward
   
    // Convert the declining Y-axis readings for going backward from 110 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 70, 0, 0, -255);
    motorSpeedB = map(yAxis, 70, 0, 0, -255);
//    Serial.print(motorSpeedA);
//    Serial.print("  ");
//    Serial.println(yAxis);
  }
  else if (yAxis > 90) {
    // Set Motor A forward
//    digitalWrite(in1, LOW);
//    digitalWrite(in2, HIGH);
//    // Set Motor B forward
//    digitalWrite(in3, LOW);
//    digitalWrite(in4, HIGH);
    // Convert the increasing Y-axis readings for going forward from 140 to 255 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 90, 175, 0, 180);
    motorSpeedB = map(yAxis, 90, 175, 0, 180);
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }
  // X-axis used for left and right control
  if (xAxis < 70) {
    // Convert the declining X-axis readings from 140 to 255 into increasing 0 to 255 value
    int xMapped = map(xAxis, 70, 0, 0, 100);
    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
  }
  if (xAxis > 90) {
    // Convert the increasing X-axis readings from 110 to 0 into 0 to 255 value
    int xMapped = map(xAxis, 90, 175, 0, 100);
    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedA > 100) {
      motorSpeedA = 100;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }
 Motor(motorSpeedA,motorSpeedB);
    
  // Print the data in the Serial Monitor
  Serial.print("j1PotX: ");
  Serial.print(data.j1PotX);
  Serial.print("; j1PotY: ");
  Serial.print(data.j1PotY);
  Serial.print("; button1: ");
  Serial.print(data.button1);
  Serial.print("; j2PotX: ");
  Serial.println(data.j2PotX); 

}
void Motor(int zuun, int baruun)
{

  analogWrite(PWMA, abs(baruun));
  analogWrite(PWMB, abs(zuun));
  if (baruun < 0)
  {
    digitalWrite(AIN1, 0);
    digitalWrite(AIN2, 1);
  }
  else if (baruun > 0)
  {
    digitalWrite(AIN1, 1);
    digitalWrite(AIN2, 0);

  }
  else
  {
    digitalWrite(AIN1, 1);
    digitalWrite(AIN2, 1);

  }
  if (zuun > 0)
  {
    digitalWrite(BIN1, 0);
    digitalWrite(BIN2, 1);
  }
  else if (zuun < 0)
  {
    digitalWrite(BIN1, 1);
    digitalWrite(BIN2, 0);

  }
  else
  {
    digitalWrite(BIN1, 1);
    digitalWrite(BIN2, 1);

  }
}
void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}
void buzzer(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(10, 1);
    delay(50);
    digitalWrite(10, 0);
    delay(50);

  }

}
