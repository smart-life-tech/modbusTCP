
#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>
#include <SPI.h>
// #include <Ethernet.h>
#include <EtherCard.h>

const int ledPin = LED_BUILTIN;
byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// IPAddress ip(192, 168, 1, 177);

// EthernetServer ethServer(502);
static byte myip[] = {192, 168, 1, 177}; // Static IP Definition/Setting the static IP
static byte gwip[] = {192, 168, 2, 1};   // Gateway IP address of the network
ModbusTCPServer modbusTCPServer;
byte Ethernet::buffer[500];
void setup()
{
    Serial.begin(9600);
    if (ether.begin(sizeof Ethernet::buffer, mac) == 0)
        Serial.println("Failed to access Ethernet controller");

    ether.staticSetup(myip, gwip);

    Serial.println("Network parameters");
    ether.printIp("IP: ", ether.myip);
    ether.printIp("GW: ", ether.gwip);
    ether.printIp("DNS: ", ether.dnsip);
    // You can use Ethernet.init(pin) to configure the CS pin
    // Ethernet.init(10); // Most Arduino shields
    // Ethernet.init(5);   // MKR ETH shield
    // Ethernet.init(0);   // Teensy 2.0
    // Ethernet.init(20);  // Teensy++ 2.0
    // Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
    // Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

    // start the Ethernet connection and the server:
    /*Ethernet.begin(mac, ip);

    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
        Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        while (true)
        {
            delay(1); // do nothing, no point running without Ethernet hardware
        }
    }
    if (Ethernet.linkStatus() == LinkOFF)
    {
        Serial.println("Ethernet cable is not connected.");
    }

    // start the server
    ethServer.begin();

    // start the Modbus TCP server
    if (!modbusTCPServer.begin())
    {
        Serial.println("Failed to start Modbus TCP Server!");
        while (1)
            ;
    }

    // configure the LED
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // configure a single coil at address 0x00
    modbusTCPServer.configureCoils(0x00, 1);

    Serial.println("Modbus RTU Server LED");
*/
    // start the Modbus RTU server, with (slave) id 1
    if (!ModbusRTUServer.begin(1, 9600))
    {
        Serial.println("Failed to start Modbus RTU Server!");
        while (1)
            ;
    }

    // configure the LED
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // configure a single coil at address 0x00
    ModbusRTUServer.configureCoils(0x00, 1);
}

void loop()
{
    // listen for incoming clients
    /* EthernetClient client = ethServer.available();

     if (client)
     {
         // a new client connected
         Serial.println("new client");

         // let the Modbus TCP accept the connection
         modbusTCPServer.accept(client);

         while (client.connected())
         {
             // poll for Modbus TCP requests, while client connected
             modbusTCPServer.poll();

             // update the LED
             updateLED();
         }

         Serial.println("client disconnected");
     }

     // poll for Modbus RTU requests
     int packetReceived = ModbusRTUServer.poll();

     if (packetReceived)
     {
         // read the current value of the coil
         int coilValue = ModbusRTUServer.coilRead(0x00);
         Serial.print("coil value");
         Serial.println(coilValue);

         if (coilValue)
         {
             // coil value set, turn LED on
             digitalWrite(ledPin, HIGH);
         }
         else
         {
             // coil value clear, turn LED off
             digitalWrite(ledPin, LOW);
         }
     }*/
    if (ether.packetLoop(ether.packetReceive()))
    {
        Serial.println(ether.packetReceive());
        Serial.println(ether.packetLoop(ether.packetReceive()));
        // memcpy_P(ether.tcpOffset(), page, sizeof page);
        // ether.httpServerReply(sizeof page - 1);
    }
}

void updateLED()
{
    // read the current value of the coil
    int coilValue = modbusTCPServer.coilRead(0x00);

    if (coilValue)
    {
        // coil value set, turn LED on
        digitalWrite(ledPin, HIGH);
    }
    else
    {
        // coild value clear, turn LED off
        digitalWrite(ledPin, LOW);
    }
}