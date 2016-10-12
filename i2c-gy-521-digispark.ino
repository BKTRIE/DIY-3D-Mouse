// Quick an dirty ... as attach to an youtube video

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
