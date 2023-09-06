#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS 10 // Chip select line for TFT display
#define TFT_RST 8 // Reset line for TFT (or connect to +3.3V)
#define TFT_DC 7  // Data/Command line for TFT
#define BL 9
// Initialize the TFT display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
const int sectionHeight = 60; // Each section's height (pixels)
const int maxLoad = 200;      // Maximum load value
bool toggle = HIGH;
void setup()
{
  delay(1000);
  tft.initR(INITR_BLACKTAB); // Initialize ST7735S display
  tft.setRotation(3);
  // Set text color and size
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);

  // Clear the screen
  tft.fillScreen(ST7735_BLACK);
  pinMode(BL, OUTPUT);
}

void drawLoadBar(int y, int load)
{
  int loadWidth = map(load, 0, maxLoad, 0, tft.width());

  // Determine the color based on the load value
  uint16_t loadColor;
  if (load < 100)
  {
    loadColor = ST7735_GREEN;
  }
  else if (load < 200)
  {
    loadColor = ST7735_YELLOW;
  }
  else
  {
    loadColor = ST7735_RED;
  }

  // Draw the load bar
  tft.fillRect(0, y, tft.width(), sectionHeight, ST7735_BLACK);
  tft.fillRect(0, y, loadWidth, sectionHeight, loadColor);
}
void loop()
{
  int loadS = 85; // Replace with your actual load values
  int loadZ = 128;
  int loadX = 32;

  // Clear the screen
  tft.fillScreen(ST7735_BLACK);

  // Draw section labels and load values
  tft.setCursor(5, 5);
  tft.print("S ");
  tft.print(loadS);
  tft.println("°");

  tft.setCursor(5, sectionHeight + 5);
  tft.print("Z ");
  tft.print(loadZ);
  tft.println("°");

  tft.setCursor(5, 2 * sectionHeight + 5);
  tft.print("X ");
  tft.print(loadX);
  tft.println("°");

  // Draw load bars
  drawLoadBar(25, loadS);
  drawLoadBar(25 + sectionHeight, loadZ);
  drawLoadBar(25 + 2 * sectionHeight, loadX);

  delay(1000); // Adjust the update interval as needed
  toggle = !toggle;
  digitalWrite(BL, toggle);
}