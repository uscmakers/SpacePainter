#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <MultiStepper.h>
#include <Adafruit_NeoPixel.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Connect two steppers
Adafruit_StepperMotor *thetaStepper = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *phiStepper = AFMS.getStepper(513 * 4, 2);

#define PIN 10
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1()
{
  thetaStepper->onestep(FORWARD, SINGLE);
}
void backwardstep1()
{
  thetaStepper->onestep(BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardstep2()
{
  phiStepper->onestep(FORWARD, SINGLE);
}
void backwardstep2()
{
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

  theta.setMaxSpeed(60.0);
  theta.setAcceleration(20.0);

  phi.setMaxSpeed(60.0);
  phi.setAcceleration(50.0);

  strip.begin();
  strip.setPixelColor(0, 0, 0, 0);
  strip.show();
}

void loop()
{
  long base_phi = 212 * 2;
  long loc[2] = {0, base_phi};

  j = 0;

  //initial circle
  motors.moveTo(loc);
  motors.runSpeedToPosition();
  delay(10000);

  loc[0] = 200;

  motors.moveTo(loc);
  motors.runSpeedToPosition();

  strip.setPixelColor(0, Wheel((j) & 255));
  strip.show();
  j = j + 40;
  
  //start at theta = 200
  //need to go to theta = 0, then theta = 200
  long delta_delta = 150;
  long delta = delta_delta;

  for (int i = 0; i < 2; i++)
  {
    strip.setPixelColor(0, Wheel((j) & 255));
    strip.show();
    j = j + 40;
    
    if (base_phi - delta > 0 && base_phi + delta < 212 * 4)
    {
      loc[0] = 150;
      loc[1] = base_phi - delta;
      motors.moveTo(loc);
      motors.runSpeedToPosition();

      strip.setPixelColor(0, Wheel((j) & 255));
      strip.show();
      j = j + 40;

      loc[0] = 100;
      loc[1] = base_phi;
      motors.moveTo(loc);
      motors.runSpeedToPosition();

      strip.setPixelColor(0, Wheel((j) & 255));
      strip.show();
      j = j + 40;

      loc[0] = 50;
      loc[1] = base_phi + delta;
      motors.moveTo(loc);
      motors.runSpeedToPosition();

      strip.setPixelColor(0, Wheel((j) & 255));
      strip.show();
      j = j + 40;

      loc[0] = 0;
      loc[1] = base_phi;
      motors.moveTo(loc);
      motors.runSpeedToPosition();

      strip.setPixelColor(0, Wheel((j) & 255));
      strip.show();
      j = j + 40;

      loc[0] = 50;
      loc[1] = base_phi - delta;
      motors.moveTo(loc);
      motors.runSpeedToPosition();

      strip.setPixelColor(0, Wheel((j) & 255));
      strip.show();
      j = j + 40;

      loc[0] = 100;
      loc[1] = base_phi;
      motors.moveTo(loc);
      motors.runSpeedToPosition();

      strip.setPixelColor(0, Wheel((j) & 255));
      strip.show();
      j = j + 40;

      loc[0] = 150;
      loc[1] = base_phi + delta;
      motors.moveTo(loc);
      motors.runSpeedToPosition();

      strip.setPixelColor(0, Wheel((j) & 255));
      strip.show();
      j = j + 40;

      loc[0] = 200;
      loc[1] = base_phi;
      motors.moveTo(loc);
      motors.runSpeedToPosition();

      delta += delta_delta;
    }
  }

  delay(6000);

  strip.setPixelColor(0, 0, 0, 0);
  strip.show();

  loc[0] = 0;
  loc[1] = 0;
  motors.moveTo(loc);
  motors.runSpeedToPosition();

  delay(1000000000);
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
