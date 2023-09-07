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
    // tft.fillRect(0, 0, tft.width(), sectionHeight, ST7735_BLACK);
    tft.fillRect(0, 0, loadWidth, sectionHeight, loadColor);
    break;
  case 1:
    // tft.fillRect(0, 0, tft.width(), sectionHeight, ST7735_BLACK);
    tft.fillRect(0, 40, loadWidth, sectionHeight, loadColor);
  case 2:
    // tft.fillRect(0, 0, tft.width(), sectionHeight, ST7735_BLACK);
    tft.fillRect(0, 80, loadWidth, sectionHeight, loadColor);
  default:
    break;
  }
  // Draw the load bar
}
void loop()
{
  int loadS = tft.width(); // Replace with your actual load values
  int loadZ = tft.height();
  int loadX = 32;

  // Clear the screen
  // tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);
  // Draw section labels and load values
  tft.setCursor(0, 20);
  tft.print("S ");
  tft.print(loadS);
  tft.println("");

  tft.setCursor(0, 60);
  tft.print("Z ");
  tft.print(loadZ);
  tft.println("");

  tft.setCursor(0, 100);
  tft.print("X ");
  tft.print(loadX);
  tft.println("");

  // Draw load bars
  drawLoadBar(0, loadS);
  drawLoadBar(1, loadZ);
  drawLoadBar(2, loadX);

  delay(1000); // Adjust the update interval as needed
  toggle = !toggle;
  digitalWrite(BL, toggle);
}