#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <Adafruit_GFX.h>
#if defined(_GFXFONT_H_)           //are we using the new library?
#include <Fonts/FreeSans9pt7b.h>
#define ADJ_BASELINE 11            //new fonts setCursor to bottom of letter
#else
#define ADJ_BASELINE 0             //legacy setCursor to top of letter
#endif

// Screen Prep
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define RGB(r, g, b) (((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3))
#define GREY      RGB(127, 127, 127)
#define DARKGREY  RGB(64, 64, 64)
#define TURQUOISE RGB(0, 128, 128)
#define PINK      RGB(255, 128, 192)
#define OLIVE     RGB(128, 128, 0)
#define PURPLE    RGB(128, 0, 128)
#define AZURE     RGB(0, 128, 255)
#define ORANGE    RGB(255,128,64)

uint16_t ID;

// Sensor Prep
#include "DHT.h"
#define DHTPIN1 22
#define DHTTYPE1 DHT22 
DHT dht1(DHTPIN1, DHTTYPE1);
#define DHTPIN2 23
#define DHTTYPE2 DHT22 
DHT dht2(DHTPIN2, DHTTYPE2);

#include <stdio.h>

int x_pos = 0; //start the graph

void setup() {
  Serial.begin(9600);
  tft.reset();
  ID = tft.readID();
  tft.begin(ID);
  tft.invertDisplay(1);
  tft.setRotation(1);
  Serial.println(F("Loading!"));
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Dehydrator Sensor v0.1");
  
  dht1.begin();    
  dht2.begin();
  delay(5000); //  Clear the fancy boot message
  tft.fillScreen(BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0 + ADJ_BASELINE);
  tft.setTextColor(RED);
  tft.print("T1:");
  tft.setCursor(120, 0 + ADJ_BASELINE);
  tft.setTextColor(YELLOW);
  tft.print("H1:");
  tft.setCursor(240, 0 + ADJ_BASELINE);
  tft.setTextColor(BLUE);
  tft.print("T2:");
  tft.setCursor(360, 0 + ADJ_BASELINE);
  tft.setTextColor(GREEN);
  tft.print("H2:");
}

void loop() {
  // 15 seconds between pulses
  delay(15000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float f1 = dht1.readTemperature(true);
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  float f2 = dht2.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h1) || isnan(t1) || isnan(f1)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    //return;
  }
  if (isnan(h2) || isnan(t2) || isnan(f2)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    //return;
  }
  // Update the Top Bar
  // Sensor 1 Garbage
  // S1 Temperature
  tft.fillRect(40, 0 + ADJ_BASELINE, 60, 15, BLACK);
  tft.setCursor(40, 0 + ADJ_BASELINE);
  tft.setTextColor(RED);
  tft.print(f1);
  // S1 Humidity
  tft.fillRect(160, 0 + ADJ_BASELINE, 60, 15, BLACK);
  tft.setCursor(160, 0 + ADJ_BASELINE);
  tft.setTextColor(YELLOW);
  tft.print(h1);

  // Sensor 2 Garbage
  // S1 Temperature
  tft.fillRect(280, 0 + ADJ_BASELINE, 60, 15, BLACK);
  tft.setCursor(280, 0 + ADJ_BASELINE);
  tft.setTextColor(BLUE);
  tft.print(f2);
  // S2 Humidity
  tft.fillRect(400, 0 + ADJ_BASELINE, 60, 15, BLACK);
  tft.setCursor(400, 0 + ADJ_BASELINE);
  tft.setTextColor(GREEN);
  tft.print(h2);

  // Graph section
  // Use lines 20-320, so 300
  // Assume top temperature will be 160
  // We have 480 wide, reset to 0 at 480
float t1_y_pos,t2_y_pos,h1_y_pos,h2_y_pos;

t1_y_pos = 320- (f1 *1.9);
t2_y_pos = 320- (f2 *1.9);
h1_y_pos = 320- (h1 *2.9);
h2_y_pos = 320- (h2 *2.9);
tft.drawLine(x_pos, 30, x_pos, 320, BLACK);
tft.drawPixel(x_pos,t1_y_pos, RED);
tft.drawPixel(x_pos,h1_y_pos, YELLOW);
tft.drawPixel(x_pos,t2_y_pos, BLUE);
tft.drawPixel(x_pos,h2_y_pos, GREEN);

if (x_pos == 480) {
  x_pos = 0;
} else {
  x_pos++;
}

tft.drawLine(x_pos, 30, x_pos, 320, ORANGE);
}
