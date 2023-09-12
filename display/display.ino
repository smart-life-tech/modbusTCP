#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS 10 // Chip select line for TFT display
#define TFT_RST 8 // Reset line for TFT (or connect to +3.3V)
#define TFT_DC 7  // Data/Command line for TFT
#define BL 9
// Initialize the TFT display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
const int sectionHeight = 40; // Each section's height (pixels)
const int maxLoad = 200;      // Maximum load value
bool toggle = HIGH;
struct section
{
  int SECTION_ONE;
  int SECTION_TWO;
  int SECTION_THREE;
};
section sect;
void setup()
{
  delay(1000);
  tft.initR(INITR_BLACKTAB); // Initialize ST7735S display
  tft.setRotation(3);
  // Set text color and size
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(3);

  // Clear the screen
  tft.fillScreen(ST7735_BLACK);
  pinMode(BL, OUTPUT);
}

void drawLoadBar(int section, int loadValue)
{
  int loadWidth = map(loadValue, 0, maxLoad, 0, tft.width());

  // Determine the color based on the load value
  uint16_t loadColor;
  if (loadValue < 100)
  {
    loadColor = ST7735_GREEN;
  }
  else if (loadValue < 200)
  {
    loadColor = ST7735_YELLOW;
  }
  else
  {
    loadColor = ST7735_RED;
  }
  switch (section)
  {
  case 0:
    tft.fillRect(0, 0, tft.width(), sectionHeight, ST7735_BLACK);
    tft.fillRect(0, 0, loadWidth, sectionHeight, loadColor);
    if (loadValue > 100)
    {
      tft.fillRect(loadWidth, 0, 160 - loadWidth, sectionHeight, ST7735_RED);
    }
    break;
  case 1:
    tft.fillRect(0, 42, tft.width(), sectionHeight, ST7735_BLACK);
    tft.fillRect(0, 42, loadWidth, sectionHeight, loadColor);
    if (loadValue > 100)
    {
      tft.fillRect(loadWidth, 42, 160 - loadWidth, sectionHeight, ST7735_RED);
    }
  case 2:
    tft.fillRect(0, 84, tft.width(), sectionHeight, ST7735_BLACK);
    tft.fillRect(0, 84, loadWidth, sectionHeight, loadColor);
    if (loadValue > 100)
    {
      tft.fillRect(loadWidth, 84, 160 - loadWidth, sectionHeight, ST7735_RED);
    }
  default:
    break;
  }
  // Draw the load bar
}
void textbg()
{
  tft.fillRect(20, 18, 40, 15, ST7735_BLACK);
  tft.fillRect(20, 58, 40, 15, ST7735_BLACK);
  tft.fillRect(20, 98, 40, 15, ST7735_BLACK);
}
void colors(int value)
{
  if (value < 30)
  {
    tft.setTextColor(ST7735_WHITE);
  }
  else
  {
    tft.setTextColor(ST7735_BLUE);
  }
}
void loop()
{
  int loadS = random(tft.width()); // Replace with your actual load values
  int loadZ = random(tft.height());
  int loadX = random(120);
  tft.fillScreen(ST7735_BLACK);
  // Draw load bars
  textbg();
  drawLoadBar(0, loadS);
  drawLoadBar(1, loadZ);
  drawLoadBar(2, loadX);
  // Clear the screen

  tft.setTextSize(2);
  textbg();
  // Draw section labels and load values
  tft.setCursor(0, 20);
  tft.setTextColor(ST7735_BLUE);
  tft.print("S ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(loadS);
  tft.setCursor(100, 20);
  tft.setTextColor(ST7735_BLUE);
  tft.print("228");
  //tft.println("");

  tft.setCursor(0, 62);
  tft.setTextColor(ST7735_BLUE);
  tft.print("Z ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(loadZ);
  tft.setCursor(100, 62);
  tft.setTextColor(ST7735_BLUE);
  tft.print("228");
  //tft.println("");

  tft.setCursor(0, 102);
  tft.setTextColor(ST7735_BLUE);
  tft.print("X ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(loadX);
  tft.setCursor(100, 102);
  tft.setTextColor(ST7735_BLUE);
  tft.print("228");
  //tft.println("");

  delay(5000); // Adjust the update interval as needed
}