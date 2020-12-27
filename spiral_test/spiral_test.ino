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
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

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
  
  theta.setMaxSpeed(30.0);
  theta.setAcceleration(30.0);

  phi.setMaxSpeed(60.0);
  phi.setAcceleration(50.0);

  strip.begin();
  strip.show();
}

void loop()
{
  
  theta.moveTo(200);
  phi.moveTo(0);
  while(theta.distanceToGo() != 0 || phi.distanceToGo() != 0) {
    theta.run();
    phi.run();
  }

  delay(5000);

  j = 0;
  
  theta.moveTo(-200);
  phi.moveTo(205*4);
  while(theta.distanceToGo() != 0 || phi.distanceToGo() != 0) {
    strip.setPixelColor(0, Wheel((j) & 255));
    strip.show();
    delay(20);
    j++;
    theta.run();
    phi.run();
  }

  strip.setPixelColor(0, 0, 0, 0);
  delay(5000);

  theta.moveTo(0);
  phi.moveTo(0);
  while(theta.distanceToGo() != 0 || phi.distanceToGo() != 0) {
    theta.run();
    phi.run();
  }

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
