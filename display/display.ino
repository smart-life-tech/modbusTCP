#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS 10 // Chip select line for TFT display
#define TFT_RST 8 // Reset line for TFT (or connect to +3.3V)
#define TFT_DC 7  // Data/Command line for TFT
#define BL 9
int loadWidth = 100;
// Initialize the TFT display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
const int sectionHeight = 40; // Each section's height (pixels)
const int maxLoad = 210;      // Maximum load value
bool toggle = HIGH;

int loads[3] = {};

void setup()
{
  delay(1000);
  // tft.initR(INITR_BLACKTAB); // Initialize ST7735S display
  tft.initR(0x02);
  tft.setRotation(3);
  // Set text color and size
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(3);

  // Clear the screen
  tft.fillScreen(ST7735_BLACK);
  pinMode(BL, OUTPUT);
  loads[0] = 0;
  loads[1] = 0;
  loads[2] = 0;
}

void drawLoadBar(int section, int loadValue)
{

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
  switch (section)
  {
  case 0:
    if (loadValue <= 100)
    {
      loadWidth = map(loadValue, 0, 100, 0, tft.width());
      tft.fillRect(0, 0, tft.width(), sectionHeight, ST7735_BLACK);
      tft.fillRect(0, 0, loadWidth, sectionHeight, loadColor);
    }
    if (loadValue > 100)
    {
      loadValue = (200 * loadValue) / 210; // the overload value reminder in percentage 210 max
      loadValue = loadValue - 100;         // reminder value for the red
      loadValue = 100 - loadValue;         // value for the yellow
      loadWidth = map(loadValue, 0, 100, 0, tft.width());
      tft.fillRect(0, 0, tft.width(), sectionHeight, ST7735_BLACK);
      tft.fillRect(0, 0, loadWidth, sectionHeight, loadColor);
      tft.fillRect(loadWidth-5, 0, 160 - loadWidth, sectionHeight, ST7735_RED);
    }
    break;
  case 1:
    if (loadValue <= 100)
    {
      loadWidth = map(loadValue, 0, 100, 0, tft.width());
      tft.fillRect(0, 42, tft.width(), sectionHeight, ST7735_BLACK);
      tft.fillRect(0, 42, loadWidth, sectionHeight, loadColor);
    }
    if (loadValue > 100)
    {
      loadValue = (200 * loadValue) / 210; // the overload value reminder in percentage 210 max
      loadValue = loadValue - 100;         // reminder value for the red
      loadValue = 100 - loadValue;         // value for the yellow
      loadWidth = map(loadValue, 0, 100, 0, tft.width());
      tft.fillRect(0, 42, tft.width(), sectionHeight, ST7735_BLACK);
      tft.fillRect(0, 42, loadWidth, sectionHeight, loadColor);
      tft.fillRect(loadWidth-5, 42, 160 - loadWidth, sectionHeight, ST7735_RED);
    }
  case 2:
    if (loadValue <= 100)
    {
      loadWidth = map(loadValue, 0, 100, 0, tft.width());
      tft.fillRect(0, 84, tft.width(), sectionHeight, ST7735_BLACK);
      tft.fillRect(0, 84, loadWidth, sectionHeight, loadColor);
    }
    if (loadValue > 100)
    {
      loadValue = (200 * loadValue) / 210; // the overload value reminder in percentage 210 max
      loadValue = loadValue - 100;         // reminder value for the red
      loadValue = 100 - loadValue;         // value for the yellow
      loadWidth = map(loadValue, 0, 100, 0, tft.width());
      tft.fillRect(0, 84, tft.width(), sectionHeight, ST7735_BLACK);
      tft.fillRect(0, 84, loadWidth, sectionHeight, loadColor);
      tft.fillRect(loadWidth-5, 84, 160 - loadWidth, sectionHeight, ST7735_RED);
    }
  default:
    break;
  }
  // Draw the load bar
}
void textbg()
{
  tft.fillRect(20, 10, 45, 23, ST7735_BLACK);
  tft.fillRect(20, 50, 45, 23, ST7735_BLACK);
  tft.fillRect(20, 93, 45, 23, ST7735_BLACK);
}

int greatLoad(int section, int load)
{
  for (int i = 0; i < 3; i++)
  {
    if (section == i)
    {
      if (load > loads[i])
      {
        loads[i] = load;
        return load;
      }
      else
      {
        return loads[i];
      }
    }
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
  tft.setCursor(2, 15);
  tft.setTextColor(ST7735_BLUE);
  tft.print("S ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(loadS);
  tft.setCursor(120, 15);
  tft.setTextColor(ST7735_BLUE);
  tft.print(greatLoad(0, loadS));
  // tft.println("");

  tft.setCursor(2, 55);
  tft.setTextColor(ST7735_BLUE);
  tft.print("Z ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(loadZ);
  tft.setCursor(120, 55);
  tft.setTextColor(ST7735_BLUE);
  tft.print(greatLoad(1, loadZ));
  // tft.println("");

  tft.setCursor(2, 98);
  tft.setTextColor(ST7735_BLUE);
  tft.print("X ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(loadX);
  tft.setCursor(120, 98);
  tft.setTextColor(ST7735_BLUE);
  tft.print(greatLoad(2, loadX));
  // tft.println("");

  delay(5000); // Adjust the update interval as needed
}