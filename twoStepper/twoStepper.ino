// Shows how to run three Steppers at once with varying speeds
//
// Requires the Adafruit_Motorshield v2 library 
//   https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
// And AccelStepper with AFMotor support 
//   https://github.com/adafruit/AccelStepper

// This tutorial is for Adafruit Motorshield v2 only!
// Will not work with v1 shields

#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Connect two steppers
Adafruit_StepperMotor *thetaStepper = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *phiStepper = AFMS.getStepper(513*4, 2);

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

void setup()
{  
  AFMS.begin(); //Start the shield
   
  theta.setMaxSpeed(20.0);
  theta.setAcceleration(20.0);
  theta.moveTo(200);

  phi.setMaxSpeed(200.0);
  phi.setAcceleration(200.0);
  phi.moveTo(513*4);
}

void loop()
{
    // Change direction at the limits
    //if (theta.distanceToGo() == 0)
    //  theta.moveTo(-theta.currentPosition());

    //if (phi.distanceToGo() == 0)
    //  phi.moveTo(-phi.currentPosition());
    phi.run();
    theta.run();
    
}
