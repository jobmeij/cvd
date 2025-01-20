// Balancing robot - january 2025
// 
// MP6500U on I2C
// 22 SCL
// 21 SDA 
//
// L298N motor driver on 
// D13 - IN1
// D12 - IN2
// D14 - IN3
// D27 - IN4

#include <L298N.h>



// Define pins
const uint8_t pin_en1 = 0;
const uint8_t pin_en2 = 1;
const uint8_t pin_in1 = 13;
const uint8_t pin_in2 = 12;
const uint8_t pin_in3 = 14;
const uint8_t pin_in4 = 27;
const uint8_t pin_scl = 22;
const uint8_t pin_sda = 21;

// Create one motor instance
// L298N motorL(pin_en1, pin_in1, pin_in2);
// L298N motorR(pin_en2, pin_in3, pin_in4);


void setup() {
  // Init L298N motor driver pins
  pinMode(pin_in1, OUTPUT);
  pinMode(pin_in2, OUTPUT);
  pinMode(pin_in3, OUTPUT);
  pinMode(pin_in4, OUTPUT);

  // Init MPU6500
  // TODO
  // motorL.setSpeed(128);
  // motorR.setSpeed(128);
}

void loop() {
  analogWrite(pin_in1, 128);
  analogWrite(pin_in3, 128);
  // motorL.forward();
  // motorR.forward();

  // delay(3000);

  // motorL.stop();
  // motorR.stop();

  // delay(3000);
}
