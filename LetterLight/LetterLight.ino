/*
  Art project time!
  Dependencies:
    - Adafruit AVR Boards (https://learn.adafruit.com/adafruit-arduino-ide-setup/arduino-1-dot-6-x-ide)
    - Adafruit Neopixels (https://github.com/adafruit/Adafruit_NeoPixel)
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
long pulseDelay = 400; // Delay between sequential pixels
int pulseSteps = 128;
int color = 0;
//long unsigned int  pulseTimer, pulseCtr = 0;
uint8_t red, green, blue;// r, g, b, rStep, gStep, bStep;
bool lastButton;// rAsc, gAsc, bAsc;
int mode = 0;
long decostart = 0;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup()
{
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  
  // initialize the digital pin as an output.
//  pinMode(BLINK_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
//  blinktime = millis();

//  red=colors[color][0];
//  green=colors[color][1];
//  blue=colors[color][2];

  for (int i=0; i < LED_NUM; i++)
  {
    pixels[i].ascending = TRUE;
    pixels[i].pulseTimer = blinktime;
    pixels[i].pulseLen = random(pulseSteps, 3*pulseSteps);
    pixels[i].pulseCtr = random(-pulseSteps*3, 0);
    pixels[i].r = pixels[i].g = pixels[i].b = 0;
  }

  lastButton = HIGH;
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
 
void loop()
{
    checkButton();
    if (mode == DECO)
    {
      if (millis() - decostart > DECOTIME) mode = OFF_MODE;
      pulse();
    }
    else if (mode == LO_LIGHT)
    {
      for (int i = 0; i < LED_NUM; i++)
      {
        strip.setPixelColor(i, LO_COLOR, LO_COLOR, LO_COLOR);
      }
    }
    else if (mode == HI_LIGHT)
    {
      for (int i = 0; i < LED_NUM; i++)
      {
        strip.setPixelColor(i, HI_COLOR, HI_COLOR, HI_COLOR);
      }
    }
    else if (mode == OFF_MODE)
    {
      for (int i = 0; i < LED_NUM; i++)
      {
        strip.setPixelColor(i, 0);
      }
    }
    strip.show();
//    blinkled();
}

void checkButton()
{
  if (digitalRead(BUTTON_PIN) == LOW and lastButton == HIGH)
  {
//    color++;
//    if (color >= COLOR_NUM) color=0;
//
//    red=colors[color][0];
//    green=colors[color][1];
//    blue=colors[color][2];
      mode++;
      if (mode >= NUM_MODES) mode = OFF_MODE;
      if (mode == DECO) decostart = millis();
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
      if (pixels[j].pulseCtr == pixels[j].pulseLen)
      {
        pixels[j].ascending = FALSE;
      }
      else if (pixels[j].pulseCtr == -pixels[j].pulseLen/2)
      {
        pixels[j].ascending = TRUE;
      }

      // If this pixel is ascending and crossing 0, pick a new color
      if (pixels[j].ascending and pixels[j].pulseCtr == 0)
      {
        randomColor(&pixels[j]);
      }

      // If this pixel is active (counter >= 0), update its intensity
      if (pixels[j].pulseCtr >= 0)
      {
        // Set the new intensity of the pixel
        strip.setPixelColor(j, fadeColor(pixels[j].r, pixels[j].pulseCtr, pixels[j].pulseLen), 
                               fadeColor(pixels[j].g, pixels[j].pulseCtr, pixels[j].pulseLen), 
                               fadeColor(pixels[j].b, pixels[j].pulseCtr, pixels[j].pulseLen));
      }
    }
  }
}

void randomColor(pulseControl_t *p)
{
  int c = random(0, COLOR_NUM-1);

  p->r = colors[c][0];
  p->g = colors[c][1];
  p->b = colors[c][2];
}

uint8_t fadeColor(uint8_t color, long numer, int denom)
{
  return (uint8_t)(0.2*(float)color * ((float)numer/(float)denom));
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

