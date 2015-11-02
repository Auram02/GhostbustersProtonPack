/*************************************************** 
  This is an example for our Adafruit 24-channel PWM/LED driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1429

  These drivers uses SPI to communicate, 3 pins are required to  
  interface: Data, Clock and Latch. The boards are chainable

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 1

#define data   4
#define clock   5
#define latch   6
#define oe  -1  // set to -1 to not use the enable pin (its optional)
#define cyclotronCycle 750U
#define powerLevelCycle 175U
#define packBlueCycle 175U

#define whiteVentCycle 100U
#define wandWhiteFlashCycle 500U

#define barrelCycle 300U



#define cyclotronLED1     0
#define cyclotronLED2     1
#define cyclotronLED3     2
#define cyclotronLED4     3  // update to 4

#define powerLevelLED1   4   // update to 5 6 7 8
#define powerLevelLED2   5
#define powerLevelLED3   6

#define wandWhiteStaticLED   7
#define wandOrangeStaticLED   8

//#define packBlueLED1   9   
//#define packBlueLED2   10
//#define packBlueLED3   11
//#define packBlueLED4   12
//#define packBlueLED5   13   
//#define packBlueLED6   14
//#define packBlueLED7   15
//#define packBlueLED8   16
//#define packBlueLED9   17



#define packBlueLED7   9   
#define packBlueLED8   10
#define packBlueLED4   11
#define packBlueLED6   12
#define packBlueLED1   13   
#define packBlueLED5   14
#define packBlueLED2   15
#define packBlueLED3   16
#define packBlueLED9   17 //

#define whiteVentLED1  18
#define whiteVentLED2  19

#define wandWhiteFlashLED  20


#define barrelLED1  21
#define barrelLED2  22


unsigned long cyclotronLastMillis = 0;
unsigned long powerLevelLastMillis = 0;
unsigned long packBlueLastMillis = 0;
unsigned long whiteVentLastMillis = 0;
unsigned long wandWhiteFlashLastMillis = 0;
unsigned long barrelLastMillis = 0;

boolean cyclotronLED1State = false;
boolean cyclotronLED2State = false;
boolean cyclotronLED3State = false;
boolean cyclotronLED4State = false;

boolean powerLevelLED1State = false;
boolean powerLevelLED2State = false;
boolean powerLevelLED3State = false;


boolean packBlueLEDState1 = true;
boolean packBlueLEDState2 = false;
boolean packBlueLEDState3 = false;
boolean packBlueLEDState4 = false;
boolean packBlueLEDState5 = false;
boolean packBlueLEDState6 = false;
boolean packBlueLEDState7 = false;
boolean packBlueLEDState8 = false;
boolean packBlueLEDState9 = false;


boolean whiteVentLEDState1 = false;
boolean whiteVentLEDState2 = false;
boolean wandWhiteFlashLEDState = false;

int whiteVentLEDBrightnessLevel = 0;
int barrelLED1BrightnessLevel = 0;
int barrelLED2BrightnessLevel = 4095;

boolean powerLevelIncreasing = true;
boolean whiteVentLevelIncreasing = true;

boolean barrelLevelIncreasing = true;

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

int redReactorDelay = 500;
int onPWM = 2095;
int offPWM = 0;

void setup() {
  Serial.begin(57600);
  
  Serial.println("TLC5974 test");
  tlc.begin();
  if (oe >= 0) {
    pinMode(oe, OUTPUT);
    digitalWrite(oe, LOW);
  }

  
  tlc.setPWM(wandWhiteStaticLED,onPWM);
  tlc.setPWM(wandOrangeStaticLED,onPWM);
        
  tlc.setPWM(packBlueLED1,onPWM);
  tlc.write();
}

void loop() {
  
   if(cycleCheck(&cyclotronLastMillis, cyclotronCycle))
   {
      Cyclotron();
   }

   if (cycleCheck(&packBlueLastMillis, packBlueCycle))
   {
      PackBlue();
   }

   if(cycleCheck(&powerLevelLastMillis, powerLevelCycle))
   {
      PowerLevel();
   }
   
   if(cycleCheck(&whiteVentLastMillis, whiteVentCycle))
   {
      WhiteVent();
   }
   
   if(cycleCheck(&wandWhiteFlashLastMillis, wandWhiteFlashCycle))
   {
      WandWhiteFlash();
   }
   
   if(cycleCheck(&barrelLastMillis, barrelCycle))
   {
      Barrel();
   }
}


void Barrel()
{

  int barrelAdjustment = 2050;
  
  if (barrelLevelIncreasing) {
      
    barrelLED1BrightnessLevel += barrelAdjustment;
    barrelLED2BrightnessLevel -= barrelAdjustment;
  } else {
    barrelLED1BrightnessLevel -= barrelAdjustment;
    barrelLED2BrightnessLevel += barrelAdjustment;
  }
  
  if (barrelLED1BrightnessLevel > onPWM)
  {
    barrelLevelIncreasing = false;
    
    Serial.println("Starting decrease - barrel");
  
  } else if (barrelLED1BrightnessLevel < 0){
    barrelLevelIncreasing = true;
    barrelLED1BrightnessLevel = 50;
    barrelLED2BrightnessLevel = 4095;
    Serial.println("Starting INCREASE - barrel");
  }

  Serial.print("Barrel 1: ");
  Serial.println(barrelLED1BrightnessLevel);
  
  Serial.print("Barrel 2: ");
  Serial.println(barrelLED2BrightnessLevel);
  
  tlc.setPWM(barrelLED1, barrelLED1BrightnessLevel);
  tlc.setPWM(barrelLED2, barrelLED2BrightnessLevel);
  tlc.write();
}

void WhiteVent()
{
  if (whiteVentLevelIncreasing) {
      
    whiteVentLEDBrightnessLevel += 100;
  } else {
    whiteVentLEDBrightnessLevel -= 100;
  }
  
  if (whiteVentLEDBrightnessLevel > onPWM)
  {
    whiteVentLevelIncreasing = false;
    
    Serial.println("Starting decrease");
  
  } else if (whiteVentLEDBrightnessLevel < 0){
    whiteVentLevelIncreasing = true;
    whiteVentLEDBrightnessLevel = 50;
    Serial.println("Starting INCREASE");
  }
  //Serial.println(whiteVentLEDBrightnessLevel);
  
    tlc.setPWM(whiteVentLED1, whiteVentLEDBrightnessLevel);
    tlc.setPWM(whiteVentLED2, whiteVentLEDBrightnessLevel);
    tlc.write();
}

void WandWhiteFlash() {
  if (wandWhiteFlashLEDState)
  {
    tlc.setPWM(wandWhiteFlashLED, offPWM);
  } else {
    tlc.setPWM(wandWhiteFlashLED, onPWM);
  }

  tlc.write();
  wandWhiteFlashLEDState = !wandWhiteFlashLEDState;
}

void Cyclotron()
{
  if (cyclotronLED1State)
      {

        tlc.setPWM(cyclotronLED2,onPWM);
        tlc.setPWM(cyclotronLED1,offPWM);
        tlc.write();
        cyclotronLED1State = !cyclotronLED1State;
        cyclotronLED2State = true;
      } else if (cyclotronLED2State) {
        tlc.setPWM(cyclotronLED3,onPWM);
        tlc.setPWM(cyclotronLED2,offPWM);
        tlc.write();
        cyclotronLED2State = !cyclotronLED2State;
        cyclotronLED3State = true;
      } else if (cyclotronLED3State) {
        tlc.setPWM(cyclotronLED4,onPWM);
        tlc.setPWM(cyclotronLED3,offPWM);
        tlc.write();
        cyclotronLED3State = !cyclotronLED3State;
        cyclotronLED4State = true;
      } else if (cyclotronLED4State) {
        tlc.setPWM(cyclotronLED4,offPWM);
        tlc.setPWM(cyclotronLED1,onPWM);
        tlc.write();
        cyclotronLED4State = !cyclotronLED4State;
        cyclotronLED1State = true;
      } else {
        tlc.setPWM(cyclotronLED1,onPWM);
        tlc.write();
        cyclotronLED1State = true;
      }
}

void PackBlue()
{

//  
//boolean packBlueLEDState1 = false;
//boolean packBlueLEDState2 = false;
//boolean packBlueLEDState3 = false;
//boolean packBlueLEDState4 = false;
//boolean packBlueLEDState5 = false;
//boolean packBlueLEDState6 = false;
//boolean packBlueLEDState7 = false;
//boolean packBlueLEDState8 = false;

  if (packBlueLEDState9 && packBlueLEDState8 && packBlueLEDState7 && packBlueLEDState6 && packBlueLEDState5 && packBlueLEDState4 && packBlueLEDState3 && packBlueLEDState2 && packBlueLEDState1)
  {
    tlc.setPWM(packBlueLED2,offPWM);
    tlc.setPWM(packBlueLED3,offPWM);
    tlc.setPWM(packBlueLED4,offPWM);
    tlc.setPWM(packBlueLED5,offPWM);
    tlc.setPWM(packBlueLED6,offPWM);
    tlc.setPWM(packBlueLED7,offPWM);
    tlc.setPWM(packBlueLED8,offPWM);
    tlc.setPWM(packBlueLED9,offPWM);
    tlc.write();
    packBlueLEDState2 = false;
    packBlueLEDState3 = false;
    packBlueLEDState4 = false;
    packBlueLEDState5 = false;
    packBlueLEDState6 = false;
    packBlueLEDState7 = false;
    packBlueLEDState8 = false;   
    packBlueLEDState9 = false;    
  } else if (packBlueLEDState8 && packBlueLEDState7 && packBlueLEDState6 && packBlueLEDState5 && packBlueLEDState4 && packBlueLEDState3 && packBlueLEDState2 && packBlueLEDState1)
  {
    tlc.setPWM(packBlueLED9,onPWM);
    tlc.write();
    packBlueLEDState9 = true;
  } else if (packBlueLEDState7 && packBlueLEDState6 && packBlueLEDState5 && packBlueLEDState4 && packBlueLEDState3 && packBlueLEDState2 && packBlueLEDState1)
  {
    tlc.setPWM(packBlueLED8,onPWM);
    tlc.write();
    packBlueLEDState8 = true;
  } else if (packBlueLEDState6 && packBlueLEDState5 && packBlueLEDState4 && packBlueLEDState3 && packBlueLEDState2 && packBlueLEDState1)
  {
    tlc.setPWM(packBlueLED7,onPWM);
    tlc.write();
    packBlueLEDState7 = true;
  } else if (packBlueLEDState5 && packBlueLEDState4 && packBlueLEDState3 && packBlueLEDState2 && packBlueLEDState1)
  {
    tlc.setPWM(packBlueLED6,onPWM);
    tlc.write();
    packBlueLEDState6 = true;
  } else if (packBlueLEDState4 && packBlueLEDState3 && packBlueLEDState2 && packBlueLEDState1)
  {
    tlc.setPWM(packBlueLED5,onPWM);
    tlc.write();
    packBlueLEDState5 = true;
  } else if (packBlueLEDState3 && packBlueLEDState2 && packBlueLEDState1)
  {
    tlc.setPWM(packBlueLED4,onPWM);
    tlc.write();
    packBlueLEDState4 = true;
  } else if (packBlueLEDState2 && packBlueLEDState1)
  {
    tlc.setPWM(packBlueLED3,onPWM);
    tlc.write();
    packBlueLEDState3 = true;
  } else if (packBlueLEDState1)
  {
    tlc.setPWM(packBlueLED2,onPWM);
    tlc.write();
    packBlueLEDState2 = true;
  }


  
}

void PowerLevel()
{

   if (powerLevelIncreasing)
   {
       if (powerLevelLED1State && powerLevelLED2State && powerLevelLED3State ) {
          powerLevelIncreasing = !powerLevelIncreasing;
          powerLevelLED3State = !powerLevelLED3State;
          //powerLevelLED4State = !powerLevelLED4State;
          //tlc.setPWM(powerLevelLED4,onPWM);
          //tlc.write();
        } else if (powerLevelLED1State && powerLevelLED2State) {
          
          powerLevelLED3State = !powerLevelLED3State;
          tlc.setPWM(powerLevelLED3,onPWM);
          tlc.write();
        } else if (powerLevelLED1State) {
          
          powerLevelLED2State = !powerLevelLED2State;
          tlc.setPWM(powerLevelLED2,onPWM);
          tlc.write();

        } else {
          tlc.setPWM(powerLevelLED1,onPWM);
          tlc.write();
          powerLevelLED1State = true;
        }
    
   } else {
        
        if (powerLevelLED1State && powerLevelLED2State && powerLevelLED3State ) {
          
          powerLevelLED3State = !powerLevelLED3State;
          tlc.setPWM(powerLevelLED3,offPWM);
          tlc.write();
        } else if (powerLevelLED1State && powerLevelLED2State) {
          
          powerLevelLED2State = !powerLevelLED2State;
          tlc.setPWM(powerLevelLED2,offPWM);
          tlc.write();
        } else if (powerLevelLED1State) {
          
          powerLevelLED1State = !powerLevelLED1State;
          tlc.setPWM(powerLevelLED1,offPWM);
          tlc.write();

        } else {
          tlc.setPWM(powerLevelLED1,onPWM);
          tlc.write();
          powerLevelLED1State = true;
          powerLevelIncreasing = true;
        }
    
   }

 
    //  7,8,10,23
    
 
}

boolean cycleCheck(unsigned long *lastMillis, unsigned int cycle) 
{
 unsigned long currentMillis = millis();
 if(currentMillis - *lastMillis >= cycle)
 {
   *lastMillis = currentMillis;
   return true;
 }
 else
   return false;
}

// Fill the dots one after the other with a color
void colorWipe(uint16_t r, uint16_t g, uint16_t b, uint8_t wait) {
  for(uint16_t i=0; i<8*NUM_TLC5974; i++) {
      tlc.setLED(i, r, g, b);
      tlc.write();
      delay(wait);
      
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint32_t i, j;

  for(j=0; j<4096; j++) { // 1 cycle of all colors on wheel
    for(i=0; i< 8*NUM_TLC5974; i++) {
      Wheel(i, ((i * 4096 / (8*NUM_TLC5974)) + j) & 4095);
    }
    tlc.write();
    delay(wait);
  }
}

// Input a value 0 to 4095 to get a color value.
// The colours are a transition r - g - b - back to r.
void Wheel(uint8_t ledn, uint16_t WheelPos) {
  if(WheelPos < 1365) {
    tlc.setLED(ledn, 3*WheelPos, 4095 - 3*WheelPos, 0);
  } else if(WheelPos < 2731) {
    WheelPos -= 1365;
    tlc.setLED(ledn, 4095 - 3*WheelPos, 0, 3*WheelPos);
  } else {
    WheelPos -= 2731;
    tlc.setLED(ledn, 0, 3*WheelPos, 4095 - 3*WheelPos);
  }
}
