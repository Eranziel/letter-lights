

#include <Adafruit_NeoPixel.h>

#define FALSE         0
#define TRUE          1

#define BLINK_PIN     1
#define BLINK_PERIOD  1000
#define BLINK_WIDTH   100

#define LED_PIN       0
#define LED_NUM       6

#define BUTTON_PIN    2

struct pulseControl_t
{
  bool ascending;
  long unsigned int  pulseTimer;
  long pulseCtr;
  uint8_t r, g, b;
} pixels [LED_NUM];

uint8_t colors [3][3] = { 57, 240, 120,
                          194, 0, 250,
                          200, 190, 10 };

