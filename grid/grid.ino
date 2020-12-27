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
  
  theta.setMaxSpeed(40.0);
  phi.setMaxSpeed(115.0);

  strip.begin();
  strip.setPixelColor(0, 0, 0, 0);
  strip.show();
}

void loop()
{
  int circles = 5;
  int turnTheta = 200 / circles;
  int state = true;

  long pos1[2] = {200, 0};
  motors.moveTo(pos1);
  while(motors.run()) {
  }

  delay(5000);
  j = 0;

  for(int i = 0; i < circles; i++)
  {
    long down[2] = {200-(i*turnTheta), 203*state*4};
    motors.moveTo(down);
    while(motors.run()) {
      strip.setPixelColor(0, Wheel((j/10) & 255));
      strip.show();
      delay(5);
      j++;
    }
    
    long turn[2] = {200-((i+1)*turnTheta), 203*state*4};
    motors.moveTo(turn);
    while(motors.run()) {
      strip.setPixelColor(0, 0, 0, 0);
      strip.show();
    }

    state = !state;
  }

  long homeState[2] = {0, 0};
  motors.moveTo(homeState);
  while(motors.run()) {
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();
  }

  delay(5000);
  
  circles = 5;
  int turnPhi = 203*4 / circles;
  state = false;

  j = 0;

  for(int i = 1; i <= circles; i++)
  {
    long down[2] = {200*state, i*turnPhi};
    motors.moveTo(down);
    while(motors.run()) {
      strip.setPixelColor(0, 0, 0, 0);
      strip.show();
    }

    state = !state;
    
    long spin[2] = {200*state, i*turnPhi};
    motors.moveTo(spin);
    while(motors.run()) {
      strip.setPixelColor(0, Wheel((j/10) & 255));
      strip.show();
      delay(5);
      j++;
    }

  }

  motors.moveTo(homeState);
  while(motors.run()) {
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();
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
