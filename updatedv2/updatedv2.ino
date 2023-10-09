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

#define MAX485_DE 5
#define MAX485_RE_NEG 4
const int buzzerPin = 4;
int loadS = 10; // Replace with your actual load values
int loadZ = 10;
int loadX = 10;
// instantiate ModbusMaster object
ModbusMaster node;
ModbusMaster spindle;

bool state = true;

const int axisCount = 4; // Assuming you have 4 axes

int modbusID[axisCount] = {1, 2, 3, 4};
char axisName[axisCount] = {'S', 'X', 'Y', 'Z'};
int registerAddress[axisCount] = {100, 101, 102, 103};
int baudRate[axisCount] = {9600, 115200, 19200, 57600};
char stopBits[axisCount][4] = {"8N2", "8E1", "8O1", "8N1"};
int loadValue[axisCount] = {100, 100, 100, 100};
int timeInterval[axisCount] = {1000, 1500, 2000, 2500};
int alarmTrigger[4] = {100, 100, 100, 100};

// Pins for buttons
const int buttonSetParameters = A0;  // Example pin, replace with your actual pin
const int buttonResetAlarm = A1;     // Example pin, replace with your actual pin
const int buttonResetMaxValues = A2; // Example pin, replace with your actual pin

void preTransmission()
{
    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
    digitalWrite(MAX485_RE_NEG, 1);
    digitalWrite(MAX485_DE, 0);
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

    node.clearTransmitBuffer();
    node.clearResponseBuffer();
    data = node.readHoldingRegisters(0x1008, 1);
    delay(500);
    Serial.print("holding registers current1a: ");
    Serial.println(node.getResponseBuffer(0x00));
    Serial.print("holding registers current2a: ");
    Serial.println(node.getResponseBuffer(0x01));
    Serial.print("holding registers current3a: ");
    Serial.println(node.getResponseBuffer(0x02));

    if (node.getResponseBuffer(0) > 0)
    {
        loadS = node.getResponseBuffer(0x00); // Replace with your actual load values
    }
    if (node.getResponseBuffer(1) > 0)
    {
        loadZ = node.getResponseBuffer(0x01);
    }
    if (node.getResponseBuffer(2) > 0)
    {
        loadX = node.getResponseBuffer(0x02);
    }
    Serial.print("data result 3: ");
    Serial.println(result);
    return result;
}

void displayParameters(int axisIndex)
{
    tft.setCursor(5, 5);
    tft.print("Axis ");
    tft.print(axisName[axisIndex]);
    tft.println(" Parameters:");

    tft.setCursor(5, 2 * sectionHeight);
    tft.print("Modbus ID: ");
    tft.print(modbusID[axisIndex]);
    tft.println();

    tft.setCursor(5, 3 * sectionHeight);
    tft.print("Register Address: ");
    tft.print(registerAddress[axisIndex]);
    tft.println();

    tft.setCursor(5, 4 * sectionHeight);
    tft.print("Baud Rate: ");
    tft.print(baudRate[axisIndex]);
    tft.println();

    tft.setCursor(5, 5 * sectionHeight);
    tft.print("Stop Bits: ");
    tft.print(stopBits[axisIndex]);
    tft.println();

    tft.setCursor(5, 6 * sectionHeight);
    tft.print("Load Value: ");
    tft.print(loadValue[axisIndex]);
    tft.println();

    tft.setCursor(5, 7 * sectionHeight);
    tft.print("Time Interval: ");
    tft.print(timeInterval[axisIndex]);
    tft.println();
}

void checkAlarms()
{
    // Check alarms and trigger actions
    // ... (implement alarm logic based on loadValue and alarmTrigger[])
    for (int i = 0; i < axisCount; i++)
    {
        if (loadValue[i] > alarmTrigger[i])
        {
            // Load value exceeds the alarm trigger for this axis
            // Trigger an alarm (e.g., turn on a buzzer)
            digitalWrite(buzzerPin, HIGH);

            // Display the axis that triggered the alarm
            tft.setCursor(5, 8 * sectionHeight);
            tft.print("Alarm triggered for Axis ");
            tft.print(axisName[i]);
            tft.println();
        }
        else
        {
            digitalWrite(buzzerPin, LOW);
        }
    }
}

void resetAlarms()
{
    // Reset alarms for all axes
    // ... (implement alarm reset logic)
}

void resetMaxValues()
{
    // Reset max values for all axes
    for (int i = 0; i < axisCount; i++)
    {
        loads[i] = 0;
    }
}

void setParameters()
{
    uint16_t data;
    node.clearTransmitBuffer();
    node.clearResponseBuffer();
    data = node.readHoldingRegisters(222, 16);
    delay(500);

    auto intToStopBit = [](int value)
    {
        switch (value)
        {
        case 0:
            return "8N2";
        case 1:
            return "8E1";
        case 2:
            return "8O1";
        case 3:
            return "8N1";
        default:
            return "Invalid";
        }
    };

    auto intToAxisName = [](int value)
    {
        switch (value)
        {
        case 0:
            return 'S';
        case 1:
            return 'X';
        case 2:
            return 'Y';
        case 3:
            return 'Z';
        default:
            return 'I'; // Invalid
        }
    };

    // for Axis S (index 0)
    axisName[0] = intToAxisName(node.getResponseBuffer(0x00));
    registerAddress[0] = node.getResponseBuffer(0x01);
    baudRate[0] = node.getResponseBuffer(0x02);
    strcpy(stopBits[0], intToStopBit(node.getResponseBuffer(0x03)));
    loadValue[0] = node.getResponseBuffer(0x04);
    timeInterval[0] = node.getResponseBuffer(0x05);
    alarmTrigger[0] = node.getResponseBuffer(0x06);

    // for Axis X (index 1)
    axisName[1] = intToAxisName(node.getResponseBuffer(0x07));
    registerAddress[1] = node.getResponseBuffer(0x08);
    baudRate[1] = node.getResponseBuffer(0x09);
    strcpy(stopBits[1], intToStopBit(node.getResponseBuffer(0x0A)));
    loadValue[1] = node.getResponseBuffer(0x0B);
    timeInterval[1] = node.getResponseBuffer(0x0C);
    alarmTrigger[1] = node.getResponseBuffer(0x0D);

    // for Axis Y (index 2)
    axisName[2] = intToAxisName(node.getResponseBuffer(0x0E));
    registerAddress[2] = node.getResponseBuffer(0x0F);
    baudRate[2] = node.getResponseBuffer(0x10);
    strcpy(stopBits[2], intToStopBit(node.getResponseBuffer(0x11)));
    loadValue[2] = node.getResponseBuffer(0x12);
    timeInterval[2] = node.getResponseBuffer(0x13);
    alarmTrigger[2] = node.getResponseBuffer(0x14);

    // for Axis Z (index 3)
    axisName[3] = intToAxisName(node.getResponseBuffer(0x15));
    registerAddress[3] = node.getResponseBuffer(0x16);
    baudRate[3] = node.getResponseBuffer(0x17);
    strcpy(stopBits[3], intToStopBit(node.getResponseBuffer(0x18)));
    loadValue[3] = node.getResponseBuffer(0x19);
    timeInterval[3] = node.getResponseBuffer(0x1A);
    alarmTrigger[3] = node.getResponseBuffer(0x1B);
}

void setup()
{
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
    node.begin(1, spindleSerial);
    // spindle.begin(2, spindleSerial);

    // Callbacks allow us to configure the RS485 transceiver correctly
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);
    pinMode(buttonSetParameters, INPUT_PULLUP);
    pinMode(buttonResetAlarm, INPUT_PULLUP);
    pinMode(buttonResetMaxValues, INPUT_PULLUP);
    // ... (other setup code)
    pinMode(buzzerPin, OUTPUT);
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
    checkAlarms();

    // Check button presses and perform actions
    if (digitalRead(buttonSetParameters) == LOW)
    {
        // Button to set parameters pressed
        // ... (implement setting parameters functionality)
        setParameters();
    }

    if (digitalRead(buttonResetAlarm) == LOW)
    {
        // Button to reset alarms pressed
        resetAlarms();
    }

    if (digitalRead(buttonResetMaxValues) == LOW)
    {
        // Button to reset max values pressed
        resetMaxValues();
    }
    delay(1000); // Adjust the update interval as needed
}
