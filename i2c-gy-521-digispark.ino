// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Wire.h>
#include <DigiKeyboard.h>
const int MPU_addr = 0x68; // I2C address of the MPU-6050

long gyroX, gyroY, gyroZ;
int rotX, rotY, rotZ;

void setup()
{

  Wire.begin();
  DigiKeyboard.delay(3000);

  DigiKeyboard.println("\nI2C gy-521");
  setupMPU();
}


void setupMPU() {
  Wire.beginTransmission(MPU_addr); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();
  Wire.beginTransmission(MPU_addr); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4)
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s
  Wire.endTransmission();
}

void loop()
{
  recordGyroRegisters();
  DigiKeyboard.print(" | GyX = ");
  DigiKeyboard.print(rotX);
  DigiKeyboard.print(" | GyY = ");
  DigiKeyboard.print(rotY);
  DigiKeyboard.print(" | GyZ = ");
  DigiKeyboard.println(rotZ);
  DigiKeyboard.delay(1000);           // wait 5 seconds for next scan
}

void recordGyroRegisters() {
  Wire.beginTransmission(MPU_addr); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(MPU_addr, 6); //Request Gyro Registers (43 - 48)
  while (Wire.available() < 6);
  gyroX = Wire.read() << 8 | Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read() << 8 | Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read() << 8 | Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 1310;
  rotY = gyroY / 1310;
  rotZ = gyroZ / 1310;
}
