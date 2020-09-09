#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#define jB1 1  // Joystick button 1
RF24 radio(9, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Address

struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  //  byte j2PotX;
  //  byte j2PotY;
  //  byte j2Button;
  //  byte pot1;
  //  byte pot2;
  //  byte tSwitch1;
  //  byte tSwitch2;
  //  byte button1;
  //  byte button2;
  //  byte button3;
  //  byte button4;
};
Data_Package data; //Create a variable with the above structure

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  data.j1PotX = 127; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  data.j1PotY = 127;
  //  data.j2PotX = 127;
  //  data.j2PotY = 127;
  data.j1Button = 1;
  //  data.j2Button = 1;
  //  data.pot1 = 1;
  //  data.pot2 = 1;
  //  data.tSwitch1 = 1;
  //  data.tSwitch2 = 1;
  //  data.button1 = 1;
  //  data.button2 = 1;
  //  data.button3 = 1;
  //  data.button4 = 1;

}

void loop() {
  // put your main code here, to run repeatedly:
  data.j1PotX = map(analogRead(A0), 0, 1023, 0, 255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.j1PotY = map(analogRead(A1), 0, 1023, 0, 255);
  data.j1Button = digitalRead(jB1);

     radio.write(&data, sizeof(Data_Package));
}
