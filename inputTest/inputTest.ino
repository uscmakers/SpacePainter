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

int thetaInput = 0;
bool thetaReceieved = false;
int phiInput = 0;
bool phiReceieved = false;

void setup()
{
  Serial.begin(9600);
  AFMS.begin(); //Start the shield
   
  theta.setMaxSpeed(20.0);
  theta.setAcceleration(20.0);

  phi.setMaxSpeed(200.0);
  phi.setAcceleration(200.0);
}

void loop() {
  while(!thetaReceieved) {
    if(Serial.available() > 0) {
      thetaInput = Serial.parseInt();
      Serial.print("Theta going to position: ");
      Serial.println(thetaInput);
      thetaReceieved = true;
    }
  }

  while(!phiReceieved) {
    if(Serial.available() > 0) {
      phiInput = Serial.parseInt();
      Serial.print("Phi going to position: ");
      Serial.println(phiInput);
      phiReceieved = true;
    }
  }
  
  theta.moveTo(thetaInput);
  phi.moveTo(phiInput*4);

  while(theta.distanceToGo() != 0 || phi.distanceToGo() != 0) {
    theta.run();
    phi.run();
  }

  Serial.println("Completed!");
  Serial.println();
  
  thetaReceieved = false;
  phiReceieved = false;
}
