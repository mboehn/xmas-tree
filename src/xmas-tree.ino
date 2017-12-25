/*
 * Mathias Bøhn Grytemark, 2017.
 * Based on GPL code by Stephan Martin.
 */

/*
 * Christmas tree by designer2k2.at
 * Stephan Martin 2016
 * http://www.designer2k2.at
 * https://github.com/designer2k2/xmas-tree
 * 
 * This code is made to run on a Digispark
 * Feeding Neopixel Rings, stacked to a tree.
 * 
 * Brightness is reduced as there is only USB power supply.
 * 
 */


// NeoPixel is LGPL
#include <Adafruit_NeoPixel.h> 


#define NUMPIXELS     93
#define LIGHTS_SPACE  4
#define PIN           2
#define BRIGHTNESS    5
#define WAIT_SCENE   7000
#define WAIT_LED      50
#define NUMRINGS      6

int ringstart[] = {0, 32, 56, 72, 84, 92};
int ringlen[] = {32, 24, 16, 12, 8, 1};


Adafruit_NeoPixel neo = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


uint32_t off = neo.Color(0, 0, 0);
uint32_t green = neo.Color(0, 255, 0);
uint32_t red = neo.Color(255, 0, 0);
uint32_t blue = neo.Color(0, 0, 255);
uint32_t white = neo.Color(127, 127, 127);
uint32_t magenta = neo.Color(255, 0, 255);
uint32_t yellow = neo.Color(255, 255, 0);


void neo_fill(uint32_t color)
{
  neo_fillrange(color, 0, neo.numPixels());
}


void neo_fillrange(uint32_t color, uint16_t start, uint16_t stop)
{
  for(uint16_t i=start; i <= stop; i++)
  {
    neo.setPixelColor(i, color);
    neo.show();
    delay(WAIT_LED);
  }
}


uint32_t get_color(int ring)
{
  switch(ring)
  {
    case 0:
      return green;
      break;
    case 1:
      return red;
      break;
    case 2:
      return magenta;
      break;
    case 3:
      return blue;
      break;
    case 4:
      return yellow;
      break;
    case 5:
    default:
      return white;
      break;
  }
}


void tree_ringcolors()
{
  for (int i=0; i < NUMRINGS; i++)
  {
    for (int l=0; l < ringlen[i]; l++)
    {
      uint32_t color = get_color(i);
      int led = l + ringstart[i];
      neo.setPixelColor(led, color);
    }
    delay(WAIT_LED);
  }
  neo.show();
}


void tree_with_lights()
{
  neo_fill(green);

  for (uint16_t i=0; i < neo.numPixels(); i++)
  {
    if ((i % LIGHTS_SPACE) == 0)
    {
      neo.setPixelColor(i, white);
      neo.show();
    }
  }
}


void tree_with_colors()
{
  neo_fill(magenta);
  neo_fill(blue);
  neo_fill(green);
  neo_fill(red);
}


void tree_race()
{
  for(uint16_t i=neo.numPixels(); i >= 0; i--)
  {
    if (i != neo.numPixels())
    {
      neo.setPixelColor(i + 1, off);
    }
    neo.setPixelColor(i, white);
    neo.show();
    delay(WAIT_LED * 1.5);
    if (i == 0)
    {
      neo.setPixelColor(i, off);
      neo.show();
    }
  }
}


void setup()
{
  pinMode(PIN, OUTPUT);
  neo.begin();
  neo.setBrightness(BRIGHTNESS);
  neo.show();
}


void loop()
{
  tree_with_lights();
  delay(WAIT_SCENE);
  tree_with_colors();
  delay(WAIT_SCENE);
  tree_race();
  delay(WAIT_SCENE);
  tree_ringcolors();
  delay(WAIT_SCENE);
  neo_fill(off);
  delay(WAIT_SCENE);
}

