#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <MultiStepper.h>
#include <Adafruit_NeoPixel.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Connect two steppers
Adafruit_StepperMotor *thetaStepper = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *phiStepper = AFMS.getStepper(513*4, 2);

#define PIN 10
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);


// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() {  
  thetaStepper->onestep(FORWARD, SINGLE);
}
void backwardstep1() {  
  thetaStepper->onestep(BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  phiStepper->onestep(FORWARD, SINGLE);
}
void backwardstep2() {  
  phiStepper->onestep(BACKWARD, SINGLE);
}

// Now we'll wrap the 2 steppers in an AccelStepper object
AccelStepper theta(forwardstep1, backwardstep1);
AccelStepper phi(forwardstep2, backwardstep2);

MultiStepper motors;

int j = 0;

void setup()
{  
  AFMS.begin(); //Start the shield

  motors.addStepper(theta);
  motors.addStepper(phi);
  
  theta.setMaxSpeed(80.0);
  //theta.setAcceleration(20.0);

  phi.setMaxSpeed(30.0);
  //phi.setAcceleration(50.0);

  strip.begin();
  strip.setPixelColor(0, 0, 0, 0);
  strip.show();
}

void loop()
{
  long pos1[2] = {200, 0};
  motors.moveTo(pos1);
  while(motors.run()) {
  }

  delay(2000);

  j = 0;

  long pos2[2] = {-200, 212*4};
  motors.moveTo(pos2);
  //motors.runToPosition()
  while(motors.run()) {
    strip.setPixelColor(0, Wheel((j/10) & 255));
    strip.show();
    delay(5);
    j++;
  }

  delay(5000);

  long pos3[2] = {0, 0};
  motors.moveTo(pos3);
  while(motors.run()) {
  }

  strip.setPixelColor(0, 0, 0, 0);
  strip.show();
  
  delay(200000);
  
}

int32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
