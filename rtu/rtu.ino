#include "ModbusRTU.h" //https://github.com/norgor/ModbusRTU/tree/master

ModbusRTU::ModbusRTUSlave<16> master;

unsigned short analogOut[3];
unsigned short analogIn[2];

void setup()
{
    master.begin(9600);
    master.addInputRegister(&analogIn[0], 0x2300); // id aadress changing p20_00
    master.update();

    master.addHoldingRegister(&analogOut[0], 0x2228); // spindle
    master.addHoldingRegister(&analogOut[1], 0x1008); // z axis
    master.addHoldingRegister(&analogOut[2], 0x3002); // y axis(not sure yet)
}

void loop()
{
    analogIn[0] = 1;
    master.update();
    Serial.print("current of spinle ");
    Serial.println(analogOut[0]);
    analogIn[0] = 2;
    master.update();
    Serial.print("current of z ");
    Serial.println(analogOut[1]);
    analogIn[0] = 3;
    master.update();
    Serial.print("current of x ");
    Serial.println(analogOut[2]);
    delay(1000);
    // analogWrite(10, analogOut[0]);
    //  analogWrite(9, analogOut[1]);
    // analogWrite(6, analogOut[2]);
}