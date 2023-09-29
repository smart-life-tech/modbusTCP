#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <ModbusMaster.h>
#include <SoftwareSerial.h>
SoftwareSerial spindleSerial(2, 3);
#define TFT_CS 10 // Chip select line for TFT display
#define TFT_RST 8 // Reset line for TFT (or connect to +3.3V)
#define TFT_DC 7  // Data/Command line for TFT
#define BL 9
int loadWidth = 100;
int offset = 8;
// Initialize the TFT display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
const int sectionHeight = 40; // Each section's height (pixels)
const int maxLoad = 210;      // Maximum load value
bool toggle = HIGH;

int loads[3] = {};

#define MAX485_DE 3
#define MAX485_RE_NEG 2
int loadS = 10; // Replace with your actual load values
int loadZ = 10;
int loadX = 10;
// instantiate ModbusMaster object
ModbusMaster node;
ModbusMaster spindle;

void preTransmission()
{
    digitalWrite(MAX485_RE_NEG, 1);
    digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 0);
}

bool state = true;

void setup()
{
    delay(1000);
    // tft.initR(INITR_BLACKTAB); // Initialize ST7735S display
    tft.initR(0x00);
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
    pinMode(MAX485_RE_NEG, OUTPUT);
    pinMode(MAX485_DE, OUTPUT);
    // Init in receive mode
    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 0);
    Serial.begin(115200);
    spindleSerial.begin(115200);
    node.begin(1, Serial);
    spindle.begin(2, spindleSerial);
    // Callbacks allow us to configure the RS485 transceiver correctly
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);
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
        // yellow will be one of this color
        /*
      ST7735_CYAN
      ST7735_MAGENTA
        ST7735_ORANGE
        */
        loadColor = ST7735_CYAN;
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
            tft.fillRect(loadWidth - offset, 0, 160, sectionHeight, ST7735_BLUE);
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
            tft.fillRect(loadWidth - offset, 42, 160, sectionHeight, ST7735_BLUE);
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
            tft.fillRect(loadWidth - offset, 84, 160, sectionHeight, ST7735_BLUE);
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

uint64_t getData()
{
    uint8_t result;
    uint16_t data;

    // Toggle the coil at address 0x0002 (Manual Load Control)
    result = node.writeSingleCoil(0x0002, state);
    state = !state;

    // Read 16 registers starting at 0x3100)
    data = spindle.readHoldingRegisters(0x2228, 16);
    result = node.readHoldingRegisters(0x2228, 10);

    if (result == node.ku8MBSuccess)
    {
        Serial.print("current1: ");
        Serial.println(node.getResponseBuffer(0x04) / 100.0f);
        Serial.print("current: ");
        Serial.println(node.getResponseBuffer(0xC0) / 100.0f);
        Serial.print("current3: ");
        Serial.println((node.getResponseBuffer(0x0D) + node.getResponseBuffer(0x0E) << 16) / 100.0f);

        loadS = node.getResponseBuffer(0x00); // Replace with your actual load values
        int loadZ = random(tft.height());
        int loadX = random(120);
    }

    if (data == node.ku8MBSuccess)
    {
        Serial.print("current1a: ");
        Serial.println(spindle.getResponseBuffer(0x04) / 100.0f);
        Serial.print("current2a: ");
        Serial.println(spindle.getResponseBuffer(0xC0) / 100.0f);
        Serial.print("current3a: ");
        Serial.println((spindle.getResponseBuffer(0x0D) + node.getResponseBuffer(0x0E) << 16) / 100.0f);

        loadS = spindle.getResponseBuffer(0x00); // Replace with your actual load values
        loadZ = random(tft.height());
        loadX = random(120);
    }
    return result;
}
void loop()
{
    /* int loadS = random(tft.width()); // Replace with your actual load values
     int loadZ = random(tft.height());
     int loadX = random(120);*/
    tft.fillScreen(ST7735_BLACK);
    getData();
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
    tft.setTextColor(ST7735_RED);
    tft.print("S ");
    tft.setTextColor(ST7735_WHITE);
    tft.print(loadS);
    tft.setCursor(120, 15);
    tft.setTextColor(ST7735_RED);
    tft.print(greatLoad(0, loadS));
    // tft.println("");

    tft.setCursor(2, 55);
    tft.setTextColor(ST7735_RED);
    tft.print("Z ");
    tft.setTextColor(ST7735_WHITE);
    tft.print(loadZ);
    tft.setCursor(120, 55);
    tft.setTextColor(ST7735_RED);
    tft.print(greatLoad(1, loadZ));
    // tft.println("");

    tft.setCursor(2, 98);
    tft.setTextColor(ST7735_RED);
    tft.print("X ");
    tft.setTextColor(ST7735_WHITE);
    tft.print(loadX);
    tft.setCursor(120, 98);
    tft.setTextColor(ST7735_RED);
    tft.print(greatLoad(2, loadX));
    // tft.println("");

    delay(5000); // Adjust the update interval as needed
}