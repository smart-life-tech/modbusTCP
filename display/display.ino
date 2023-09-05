#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS 10 // Chip select line for TFT display
#define TFT_RST 8 // Reset line for TFT (or connect to +3.3V)
#define TFT_DC 7  // Data/Command line for TFT

// Initialize the TFT display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup()
{
  tft.initR(INITR_BLACKTAB); // Initialize ST7735S display
  tft.setRotation(3);
  // Set text color and size
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);

  // Clear the screen
  tft.fillScreen(ST7735_BLACK);
}

void drawLoadBar(int section, int load)
{
  int x = 0;
  int y = section * (tft.height() / 3);             // Divide the screen into three equal sections
  int barWidth = map(load, 0, 200, 0, tft.width()); // Map the load to the display width

  // Clear the section
  tft.fillRect(0, y, tft.width(), tft.height() / 3, ST7735_BLACK);

  // Draw text and load value
  tft.setCursor(10, y + 10);
  tft.print("S 85°");
  tft.setCursor(10, y + 40);
  tft.print("Z 128°");
  tft.setCursor(10, y + 70);
  tft.print("X 32°");

  // Draw load bar
  if (load < 100)
  {
    tft.fillRect(x, y + 100, barWidth, 10, ST7735_GREEN);
  }
  else if (load >= 100 && load < 200)
  {
    tft.fillRect(x, y + 100, barWidth, 10, ST7735_YELLOW);
  }
  else
  {
    tft.fillRect(x, y + 100, tft.width(), 10, ST7735_YELLOW);
    tft.fillRect(x + barWidth, y + 100, tft.width() - barWidth, 10, ST7735_RED);
  }
}

void loop()
{
  // Sample load values for each section (you can replace these with actual load values)
  int loadS = 85;
  int loadZ = 128;
  int loadX = 32;

  // Draw load bars for each section
  drawLoadBar(0, loadS);
  drawLoadBar(1, loadZ);
  drawLoadBar(2, loadX);

  // Delay or update load values as needed
  delay(30); // Adjust the delay time as needed for your application
}
