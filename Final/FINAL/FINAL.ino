#include <Keypad.h>
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

const byte ROWS = 4; 
const byte COLS = 3; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3}; 

int buttState;
char curr_key;
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
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
  
  Serial.begin(9600);
  buttState = 0;
}
 
void loop() {
  
  if(buttState == 0) {
    char customKey = customKeypad.getKey();
    if(customKey && customKey != '#') {
      curr_key = customKey;
      buttState = 1;
    }
  }
  else if(buttState == 1) {
    char customKey = customKeypad.getKey();
    if(customKey) {
      if(customKey == '#') {
        if(curr_key == '1') {

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
          
          buttState = 0;
        }
        else if(curr_key == '2') {
          theta.setMaxSpeed(100.0);
          phi.setMaxSpeed(100.0);
          int circles = 8;
          int turnPhi = 203*4 / circles;
          int state = false;

          delay(5000);
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

          long homeState[2] = {0, 0};
          motors.moveTo(homeState);
          while(motors.run()) {
            strip.setPixelColor(0, 0, 0, 0);
            strip.show();
          }
  
          
          buttState = 0;
        }
        else if(curr_key == '3') {
          theta.setMaxSpeed(100.0);
          phi.setMaxSpeed(100.0);
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
          buttState = 0;
        }
        else if(curr_key == '4') {
          theta.setMaxSpeed(40.0);
          phi.setMaxSpeed(115.0);

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
          
          buttState = 0;
        }

        else if(curr_key == '5') {
          theta.setMaxSpeed(60.0);
          theta.setAcceleration(20.0);
          phi.setMaxSpeed(60.0);
          phi.setAcceleration(50.0);
        
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
                  
                  buttState = 0;
                }
                
              }
              else {
                curr_key == customKey;
              }
            }
  }
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
