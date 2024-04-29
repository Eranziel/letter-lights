/*
  Art project time!
*/
#include <avr/power.h>
#include "LetterLight.h"

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LED_PIN, NEO_GRB + NEO_KHZ800);
long unsigned int  blinktime = 0;
int pulseRate = 30;
long pulseDelay = 40; // Delay between sequential pixels
int pulseSteps = 100;
int color = 0;
//long unsigned int  pulseTimer, pulseCtr = 0;
uint8_t red, green, blue;// r, g, b, rStep, gStep, bStep;
bool lastButton;// rAsc, gAsc, bAsc;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup()
{
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  
  // initialize the digital pin as an output.
  pinMode(BLINK_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  blinktime = millis();

  red=colors[color][0];
  green=colors[color][1];
  blue=colors[color][2];

  for (int i=0; i < LED_NUM; i++)
  {
    pixels[i].ascending = TRUE;
    pixels[i].pulseTimer = blinktime;
    pixels[i].pulseCtr = 0 - i*pulseDelay;
    pixels[i].r = pixels[i].g = pixels[i].b = 0;
  }

  lastButton = HIGH;
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
 
void loop()
{
    checkButton();
    pulse();    
    strip.show();
    blinkled();
}

void checkButton()
{
  if (digitalRead(BUTTON_PIN) == LOW and lastButton == HIGH)
  {
    color++;
    if (color >= 3) color=0;

    red=colors[color][0];
    green=colors[color][1];
    blue=colors[color][2];
  }
  lastButton = digitalRead(BUTTON_PIN);
}

void pulse()
{
    unsigned long int ms = millis();
    // Update each pixel
    for (int j = 0; j < LED_NUM; j++)
    {
      if (ms - pixels[j].pulseTimer > pulseRate)
      {
        pixels[j].pulseTimer = ms;

        // Determine what the new step number is
        if (pixels[j].ascending) pixels[j].pulseCtr++;
        else pixels[j].pulseCtr--;

        // Check if this pixel needs to switch direction
        if (pixels[j].pulseCtr == pulseSteps)
        {
          pixels[j].ascending = FALSE;
        }
        else if (pixels[j].pulseCtr == 0)
        {
          pixels[j].ascending = TRUE;
        }

        // If this pixel is active (counter >= 0), update its intensity
        if (pixels[j].pulseCtr >= 0)
        {
          // Set the new intensity of the pixel
//          pixels[j].r = (uint8_t)((float)red * ((float)pixels[j].pulseCtr/(float)pulseSteps));
//          pixels[j].g = (uint8_t)((float)green * ((float)pixels[j].pulseCtr/(float)pulseSteps));
//          pixels[j].b = (uint8_t)((float)blue * ((float)pixels[j].pulseCtr/(float)pulseSteps));
          strip.setPixelColor(j, (uint8_t)((float)red * ((float)pixels[j].pulseCtr/(float)pulseSteps)), 
                                 (uint8_t)((float)green * ((float)pixels[j].pulseCtr/(float)pulseSteps)), 
                                 (uint8_t)((float)blue * ((float)pixels[j].pulseCtr/(float)pulseSteps)));
        }
      }
    }
  
//  if (millis() - pulseTimer > pulseRate)
//  {
//    pulseTimer = millis();
//    
//    // Determine what the new step number is
//    if (ascending) pulseCtr++;
//    else pulseCtr--;
//
//    // Check if we need to switch direction
//    if (pulseCtr == pulseSteps)
//    {
//      ascending = FALSE;
//    }
//    if (pulseCtr == 0)
//    {
//      ascending = TRUE;
////      if (rAsc and red > 250) rAsc = FALSE;
////      if (!rAsc and red < 5) rAsc = TRUE;
////
////      if (gAsc and green > 250) gAsc = FALSE;
////      if (!gAsc and green < 5) gAsc = TRUE;
////
////      if (bAsc and blue > 250) bAsc = FALSE;
////      if (!bAsc and blue < 5) bAsc = TRUE;
////      
////      if (rAsc) red+=5;
////      else red-=5;
////
////      if (gAsc) green+=5;
////      else green-=5;
////
////      if (bAsc) blue+=5;
////      else blue-=5;
//    }
//
//    r = (uint8_t)((float)red * ((float)pulseCtr/(float)pulseSteps));
//    g = (uint8_t)((float)green * ((float)pulseCtr/(float)pulseSteps));
//    b = (uint8_t)((float)blue * ((float)pulseCtr/(float)pulseSteps));
//    for (int i=0; i < LED_NUM; i++)
//    {
//      strip.setPixelColor(i, r, g, b);
//    }
//  }
}

void blinkled()
{
  bool state = digitalRead(BLINK_PIN);
  long unsigned int ms = millis();
  if (state == HIGH and ms - blinktime > BLINK_WIDTH)
  {
    digitalWrite(BLINK_PIN, LOW);
  }
  else if (state == LOW and ms - blinktime > BLINK_PERIOD)
  {
    digitalWrite(BLINK_PIN, HIGH);
    blinktime = ms;
  }
}

