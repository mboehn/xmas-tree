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
#define WAIT_SCENE   1000
#define WAIT_LED      50


int ringlen[] = {32, 24, 16, 12, 8, 1};
int numrings = sizeof(ringlen) / sizeof(*ringlen);


Adafruit_NeoPixel neo = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


uint32_t off = neo.Color(0, 0, 0);
uint32_t red = neo.Color(255, 0, 0);
uint32_t green = neo.Color(0, 255, 0);
uint32_t blue = neo.Color(0, 0, 255);
uint32_t yellow = neo.Color(255, 255, 0);
uint32_t magenta = neo.Color(255, 0, 255);
uint32_t cyan = neo.Color(0, 255, 255);
uint32_t white = neo.Color(127, 127, 127);

uint32_t colors[] = {green, red, blue, yellow, magenta, cyan};
int numcolors = sizeof(colors) / sizeof(*colors);


void neo_fill(uint32_t color, boolean single = true);
void neo_fillrange(uint32_t color, uint16_t start, uint16_t stop, boolean single = true);



void neo_fill(uint32_t color, boolean single)
{
  neo_fillrange(color, 0, neo.numPixels(), single);
}


void neo_fillrange(uint32_t color, uint16_t start, uint16_t stop, boolean single)
{
  for(uint16_t i=start; i <= stop; i++)
  {
    neo.setPixelColor(i, color);
    if(single) {
      neo.show();
      delay(WAIT_LED);
    }
  }
  if(!single)
  {
    neo.show();
  }
}

uint32_t get_random_color()
{
  return colors[rand() % numcolors];
}

uint32_t get_color(int ring)
{
  // FIXME: this fails if num of rings if more than wanted colors... need some circular logic here, but math is not my strong suit.
  return colors[ring];
}


void tree_ringcolors()
{
  int start = 0;
  int stop = 0;
  for (int i=0; i < numrings; i++)
  {
    if (i != 0)
    {
      start = start + ringlen[i-1];
    }
    stop = stop + ringlen[i];
    uint32_t color = get_color(i);
    neo_fillrange(color, start, stop, false);
  }
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
  for (int i=0; i < numcolors; i++)
  {
    neo_fill(colors[i]);
  }
}


void tree_race()
{
  // XXX: this should probably be i >= 0, but I can't seem to understand why that fails with uint16_t and works with int.
  // this looks quite OK though, so I'll keep it like this 'till I become smarter.
  for(uint16_t i=neo.numPixels(); i > 0; i--)
  {
    if (i != neo.numPixels())
    {
      neo.setPixelColor(i + 1, off);
    }
    neo.setPixelColor(i, white);
    neo.show();
    delay(WAIT_LED * 1.5);
    if (i == 1) // XXX: and this is a hack to fix the 0 pixel.
    {
      neo.setPixelColor(i, off);
      neo.setPixelColor(0, white);
      neo.show();
      delay(WAIT_LED * 1.5);
      neo.setPixelColor(0, off);
      delay(WAIT_LED * 1.5);
    }
  }
}

void tree_blink()
{

  uint32_t color = get_random_color();

  neo_fill(off, false);
  delay(WAIT_LED * 10);

  neo_fill(color, false);
  delay(WAIT_LED * 10);
  neo_fill(off, false);
  delay(WAIT_LED * 10);
  neo_fill(color, false);
  delay(WAIT_LED * 10);
  neo_fill(off, false);
  delay(WAIT_LED * 10);
  neo_fill(color, false);
  delay(WAIT_LED * 10);
  neo_fill(off, false);
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

  tree_blink();
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

