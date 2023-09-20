#include <ArduinoModbus.h>
#include <ModbusMaster.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>

// Modbus TCP server configuration
IPAddress ip(192, 168, 1, 100);
EthernetServer server(502);
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// Modbus RTU slave configuration
const uint8_t MODBUS_RTU_SLAVE_ID = 1;
SoftwareSerial modbusSerial(2, 3); // RX, TX
ModbusMaster node;

void setup()
{
    Ethernet.begin(mac, ip);
    server.begin();

    // Initialize Modbus RTU
    modbusSerial.begin(9600);
    node.begin(MODBUS_RTU_SLAVE_ID, modbusSerial);

    // Add your setup code here
}

void loop()
{
    // Modbus TCP handling
    EthernetClient client = server.available();
    if (client)
    {
        // Handle Modbus TCP requests
        // Example: ArduinoModbus::instance().processRequest();
        // For simplicity, let's just send a fixed response
        uint8_t responseFrame[] = {0x01, 0x03, 0x04, 0x0A, 0x00, 0x05, 0x2B, 0x85};
        int responseLength = sizeof(responseFrame);

        // Send the response
        client.write(responseFrame, responseLength);

        // Print the sent response frame
        Serial.print("Sent Modbus TCP response: ");
        for (int i = 0; i < responseLength; i++)
        {
            Serial.print(responseFrame[i], HEX);
            Serial.print(" ");
        }
        Serial.println();

        // Close the connection
        client.stop();
    }

    // Modbus RTU handling
    uint8_t result = node.readCoils(0, 16);
    if (result == node.ku8MBSuccess)
    {
        for (int i = 0; i < 16; i++)
        {
            Serial.print("RTU Coil ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(node.getResponseBuffer(i));
        }
    }
    else
    {
        Serial.print("Failed to read RTU coils! Error: ");
        Serial.println(result, HEX);
    }

    // Add your application logic here

    delay(10); // Add a small delay to reduce CPU load
}
