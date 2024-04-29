

#include <Adafruit_NeoPixel.h>

#define FALSE         0
#define TRUE          1

#define BLINK_PIN     1
#define BLINK_PERIOD  1000
#define BLINK_WIDTH   100

#define LED_PIN       0
#define LED_NUM       9

#define BUTTON_PIN    3

#define NUM_MODES     4   // Number of modes
#define OFF_MODE      0   // Off
#define DECO          1   // Decorative mode
#define LO_LIGHT      2   // Dim light mode
#define HI_LIGHT      3   // Bright light mode
#define LO_COLOR      122 // Brightness of dim mode
#define HI_COLOR      255 // Brightness of bright mode

#define DECOTIME      1800000 // 30 minutes

struct pulseControl_t
{
  bool ascending;
  long unsigned int  pulseTimer;
  long pulseCtr, pulseLen;
  uint8_t r, g, b;
} pixels [LED_NUM];

#define COLOR_NUM      7
uint8_t colors [COLOR_NUM][3] = { 57, 240, 0,//120,//green
                                  194, 0, 0,//250,//purple
                                  200, 190, 0,//10,//yellow
                                  253, 40, 0,//120,//pink
                                  255, 105, 0,//orange
                                  0, 240, 0,//67,//green
                                  20, 45, 0};//145};//blue

