/*
  Keyboard Message test
  For the Adafruit Neo Trinkey
  Sends a text string when a one touchpad is pressed, opens up Notepad if the other is pressed
*/

#include <Adafruit_NeoPixel.h>
#include "Adafruit_FreeTouch.h"
#include "Mouse.h"

// Create the neopixel strip with the built in definitions NUM_NEOPIXEL and PIN_NEOPIXEL
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Create the two touch pads on pins 1 and 2:
Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
Adafruit_FreeTouch qt_2 = Adafruit_FreeTouch(2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);


bool previousTouch1State  = false;   // for checking the state of touch 1
bool previousTouch2State  = false;   // for checking the state of touch 1
bool mouseJiggle          = false;           // We don't want to fuck with the mouse until cap button pressed
int touch1counter         = 0;              // touch 1 counter
int touch2counter         = 0;              // touch 2 counter
int brightCount           = 0;
int brightInc             = -1;
int brightStep            = 0;
int vCount                =  10;
int hCount                = 10;
int cursorDir             = 0;
int cursorDist            = 10;
int delayCount            = 0;

void setup() {
  Serial.begin(9600);
  
  strip.begin(); // start pixels
  strip.show(); // Initialize all pixels to 'off'

  if (! qt_1.begin())  
    Serial.println("Failed to begin qt on pin 1");
  if (! qt_2.begin())  
    Serial.println("Failed to begin qt on pin 2");
    
  // initialize control over the keyboard:
  Mouse.begin();
}

void loop() {
  
  // measure the captouches
  uint16_t touch1 = qt_1.measure();
  uint16_t touch2 = qt_2.measure();
  // whether we think they are being touched
  bool Touch1State, Touch2State;

  if (touch1 > 500) {
    Touch1State = true;  // is touched
  } else {
    Touch1State = false;  // is not touched
  }


  if (touch2 > 500) {
    Touch2State = true;  // is touched
  } else {
    // turn them off
    Touch2State = false;  // is not touched
  }
  

  // If we are now touching #1, and we weren't before...
  if (Touch1State and !previousTouch1State) {
    touch1counter++; // increment the touch counter
    // type out a message
    mouseJiggle = !mouseJiggle;
  }
  // save the current touch state for comparison next time:
  previousTouch1State = Touch1State;


  // If we are now touching #2, and we weren't before...
  if (Touch2State and !previousTouch2State) {
    touch2counter++; // increment the touch counter
    // type out a message
//    Serial.print("You touched the captouch #2 ");
//    Serial.print(touch2counter);
//    Serial.println(" times.");
  }

  if(mouseJiggle){
    
    switch(cursorDir) {
      case 0:
        Mouse.move(vCount, ++hCount);  
        if(hCount >= cursorDist+random(2))
          cursorDir = 1;
        break;
      case 1:
        Mouse.move(++vCount, hCount);  
        if(vCount >= cursorDist+random(2))
          cursorDir = 2;
        break;
      case 2:
        Mouse.move(vCount, --hCount);
        if(hCount <= (cursorDist * -1)-random(2))
          cursorDir = 3;
        break;
      case 3:
        Mouse.move(--vCount, hCount);
        if(vCount <= (cursorDist * -1)-random(2))
          cursorDir = 0;
        break;
      default:
        break;
    }

    if(++brightStep % 12 == 0 ){
      Serial.println(brightCount);
      if(brightCount>=25){
        brightInc = -1;
      }
  
      if(brightCount<=0){
        delayCount++;
        brightInc = 0;
        if(delayCount==25){
          brightInc = 1;
          delayCount = 0;
        }
      }
  
       brightCount += brightInc;
       //Serial.println(brightCount);
    }
    strip.setPixelColor(0, brightCount, 0, 0);
    strip.setPixelColor(1, brightCount, 0, 0);
    strip.setPixelColor(2, brightCount, 0, 0);
    strip.setPixelColor(3, brightCount, 0, 0);
  }else{
    brightCount = 25;
    strip.setPixelColor(0, 0x0);
    strip.setPixelColor(1, 0x0);        
    strip.setPixelColor(2, 0x0);
    strip.setPixelColor(3, 0x0);        
  }



  // If we are now touching #2, and we weren't before...
  if (Touch2State and !previousTouch2State) {
    cursorDist = ++cursorDist < 12 ? cursorDist : 1 ;
  }


  strip.show(); // display pixel changes

  // save the current touch state for comparison next time:
  previousTouch2State = Touch2State;
  
  delay(5);  // and pause a moment
}
