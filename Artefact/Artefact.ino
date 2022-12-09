#include <FastLED.h>
#include "Wire.h"
#include <SimpleSDAudio.h>

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x=0, gyro_y=0, gyro_z=0; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7];
const int REED_PIN = 2;
const int REED_PIN_LOCK = 3;

bool soundLockOpen = false;
bool soundLockClose = false;
bool soundOpenChest = false;
bool soundCloseChest = false;
bool soundCoins = false;
#define NUM_LEDS 2
CRGB leds[NUM_LEDS];

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
   sprintf(tmp_str, "%6d", i);
   return tmp_str;
 }
    
void setup() {

  
  Serial.begin(115200);

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(false);
  FastLED.addLeds<WS2812B,5, BGR>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255); 
  FastLED.show();


  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(REED_PIN_LOCK, INPUT_PULLUP);
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER)) { while(1); } // Error while initialization of SD card -> stop.   leds[0] = CHSV(30,255,255); leds[1] = CHSV(32,255,255);FastLED.show();
}

void loop() {
  int box_open_close = digitalRead(REED_PIN);
  int lock_open = digitalRead(REED_PIN_LOCK);
//  Serial.println("loop");

  if (box_open_close == HIGH && !soundCloseChest) {   if(!SdPlay.setFile("OPEN.AFM")) {   while(1);   }        SdPlay.play();       soundCloseChest = true;  leds[0] = CHSV(110,255,255); leds[1] = CHSV(100,255,255);FastLED.show();    }
  if (box_open_close == HIGH && soundCloseChest) {   soundOpenChest = false;    }

  if (box_open_close == LOW && soundCloseChest) {    soundCloseChest = false;     }
  if (box_open_close == LOW && !soundOpenChest) {    if(!SdPlay.setFile("CLOSE.AFM")) {   while(1);    }       SdPlay.play();     soundOpenChest = true; leds[0] = CRGB::Black; leds[1] = CRGB::Black; FastLED.show();   }

  if (lock_open == LOW && !soundLockOpen) {         if(!SdPlay.setFile("1.AFM")) {   while(1);    }       SdPlay.play();    soundLockOpen = true;    }
  if (lock_open == HIGH && soundLockOpen) {         soundLockOpen = false;   }

  if (lock_open == HIGH && !soundLockClose) {         if(!SdPlay.setFile("1.AFM")) {   while(1);    }       SdPlay.play();     soundLockClose = true;    }
  if (lock_open == LOW && soundLockClose) {         soundLockClose = false;    }

  if(gyro_z >= 4000 && !soundCoins){            if(!SdPlay.setFile("COINS.AFM")) {   while(1);   }       SdPlay.play();      soundCoins = true;    }  
  if (gyro_z <= 2000 && soundCoins) {           soundCoins = false;   }

      Wire.beginTransmission(MPU_ADDR);
      Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
      Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
      Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
      
      // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
      accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
      accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
      accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
      temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
      gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
      gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
      gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
 


     // print out data
      Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
      Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
      Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));
      // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
      Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
      Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
      Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
      Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));
      Serial.println();
         // delay (100);
 }
