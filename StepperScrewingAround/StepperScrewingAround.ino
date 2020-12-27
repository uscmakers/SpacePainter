// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
// Requires the Adafruit_Motorshield v2 library 
//   https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
// And AccelStepper with AFMotor support 
//   https://github.com/adafruit/AccelStepper

// This tutorial is for Adafruit Motorshield v2 only!
// Will not work with v1 shields

#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myStepper1 = AFMS.getStepper(513, 2);
Adafruit_StepperMotor *myStepper2 = AFMS.getStepper(200, 1);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void forwardstep1() {  
  myStepper1->onestep(FORWARD, SINGLE);
}
void backwardstep1() {  
  myStepper1->onestep(BACKWARD, SINGLE);
}
void forwardstep2() {  
  myStepper2->onestep(FORWARD, DOUBLE);
}
void backwardstep2() {  
  myStepper2->onestep(BACKWARD, DOUBLE);
}

AccelStepper stepper1(forwardstep1, backwardstep1); // use functions to step
AccelStepper stepper2(forwardstep2, backwardstep2); // use functions to step

void setup()
{  
   Serial.begin(9600);           // set up Serial library at 9600 bps
   Serial.println("Stepper test!");
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  //Astepper1.setMaxSpeed(1000);
  //Astepper1.setSpeed(800);
}

void loop()
{  
   //Astepper1.runSpeed();
   stepper1.setMaxSpeed(100.0);
   stepper1.setAcceleration(100.0);
   
   stepper2.setMaxSpeed(100.0);
   stepper2.setAcceleration(100.0);

   stepper1.moveTo(256.5);
   stepper1.setSpeed(80.0);
   
   stepper2.moveTo(256.5);
   stepper2.setSpeed(80.0);
   
   stepper1.runSpeedToPosition();
   stepper2.runSpeedToPosition();
   
   //delay(1000);

   stepper1.moveTo(0);
   stepper1.setSpeed(40.0);

   stepper2.moveTo(0);
   stepper2.setSpeed(40.0);
   
   stepper1.runSpeedToPosition();
   stepper2.runSpeedToPosition();
   
   //delay(1000);
}
