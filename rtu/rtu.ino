#include "ModbusRTU.h"//https://github.com/norgor/ModbusRTU/tree/master

ModbusRTU::ModbusRTUSlave<16> slave;

unsigned short encoder = 0;
unsigned short analogOut[3];
bool digitalOut[4];
bool digitalIn[6];
unsigned short analogIn[2];

void setup()
{
    slave.begin(115200);
    slave.addInputRegister(&analogIn[0], 0x2300);// id aadress changing p20_00
    slave.update();

    slave.addHoldingRegister(&analogOut[0], 0x2228);//spindle
    slave.addHoldingRegister(&analogOut[1], 0x1008);// z axis
    slave.addHoldingRegister(&analogOut[2], 0x3002);// y axis(not sure yet)
}

void loop()
{
    analogIn[0] = 1;
    slave.update();
    Serial.print("current of spinle ");
    Serial.println(analogOut[0]);
    analogIn[0] = 2;
    slave.update();
    Serial.print("current of z ");
    Serial.println(analogOut[1]);
    analogIn[0] = 3;
    slave.update();
    Serial.print("current of x ");
    Serial.println(analogOut[2]);
    delay(1000);
    // analogWrite(10, analogOut[0]);
    //  analogWrite(9, analogOut[1]);
    // analogWrite(6, analogOut[2]);
}