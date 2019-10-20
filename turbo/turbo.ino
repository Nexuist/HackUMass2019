#include <Wire.h>

/* Nunchuck Setup */
#define WII_I2C_ID 0x52

struct NunchuckData
{
    uint16_t JoyX;
    uint16_t JoyY;
    uint16_t AccelX;
    uint16_t AccelY;
    uint16_t AccelZ;
    uint16_t BtnZ;
    uint16_t BtnC;
};

NunchuckData *cnd = new NunchuckData();
static uint8_t nunchuckDataBuffer[6];
static uint8_t nunchukIDByteBuffer[6];

void setupNunchuck()
{
    Wire.begin();
    boolean INITIALIZING = true;
    while (INITIALIZING)
    {
        // It seems critical for F0:55 and FB:00 to be transmitted separately when it comes to the Nyko Kama
        Wire.beginTransmission(WII_I2C_ID);
        Wire.write((uint8_t)0xF0);
        Wire.write((uint8_t)0x55);
        Wire.endTransmission();
        Wire.beginTransmission(WII_I2C_ID);
        Wire.write((uint8_t)0xFB);
        Wire.write((uint8_t)0x00);
        Wire.endTransmission();
        Wire.beginTransmission(WII_I2C_ID);
        Wire.write((uint8_t)0xFA);
        Wire.endTransmission();
        Wire.requestFrom(WII_I2C_ID, 6, true); // Request 6 bytes and then release bus
        delay(1);                              // Wait for response
        int counter = 0;
        while (Wire.available())
        {
            nunchukIDByteBuffer[counter] = Wire.read();
            counter++;
        }
        if (counter == 6)
        {
            // Got everything we wanted, finish initializing
            Serial.println("MSG: Init Success");
            INITIALIZING = false;
        }
        else
        {
            Serial.println("MSG: Init Fail");
            delay(1000);
        }
    }
}

int refreshNunchuckData()
{
    int counter = 0;
    Wire.requestFrom(WII_I2C_ID, 6);
    while (Wire.available())
    {
        nunchuckDataBuffer[counter] = Wire.read();
        counter++;
    }
    // Request new payload for next time
    Wire.beginTransmission(WII_I2C_ID);
    Wire.write((uint8_t)0x00);
    Wire.endTransmission();
    if (counter != 6)
    {
        return 0; // Not all bytes acquired, fail
    }
    cnd->JoyX = nunchuckDataBuffer[0];
    cnd->JoyY = nunchuckDataBuffer[1];
    // Unpack byte 5 as ZZYYXXcz using voodoo magic
    cnd->BtnZ = (nunchuckDataBuffer[5] >> 0) & 1;
    cnd->BtnC = (nunchuckDataBuffer[5] >> 1) & 1;
    cnd->AccelX = (nunchuckDataBuffer[5] >> 2) & 3 | (nunchuckDataBuffer[2] << 2);
    cnd->AccelY = (nunchuckDataBuffer[5] >> 4) & 3 | (nunchuckDataBuffer[3] << 2);
    cnd->AccelZ = (nunchuckDataBuffer[5] >> 6) & 3 | (nunchuckDataBuffer[4] << 2);
    return 1;
}

/* Relay Setup */
int AXIS_DEADZONE = 8;
int leftPowerPin = 10;
int rightPowerPin = 11;
int relay1Pin = 2;
int relay2Pin = 3;

/* State variables */
int goingForward = 1;
int armed = 0;
int xValue = 0;
int yValue = 0;

/* Output */
int leftOutput = 0;
int rightOutput = 0;

void setup()
{
    pinMode(leftPowerPin, OUTPUT);
    pinMode(rightPowerPin, OUTPUT);
    pinMode(relay1Pin, OUTPUT);
    pinMode(relay2Pin, OUTPUT);
    Serial.begin(115200);
    setupNunchuck();
    Serial.println("MSG: Beginning loop");
}

int adjustForDeadzone(int val) {
  if ((val > -AXIS_DEADZONE) && (val < AXIS_DEADZONE)) {
    val = 0;
  }
  return val;
}

void loop()
{
    if (!refreshNunchuckData())
    {
        Serial.println("MSG: Nunchuck data refresh failed");
        analogWrite(leftPowerPin, 0);
        analogWrite(rightPowerPin, 0);
        delay(1000);
        return;
    }
    if (millis() % 100 <= 20) Serial.println((String)"DATA:" + armed + "|" + goingForward + "|" +  xValue + "|" + yValue + "|" + leftOutput + "|" + rightOutput);
    armed = (cnd->BtnZ == 0);
    xValue = cnd->JoyX;
    yValue = cnd->JoyY;
    goingForward = 1;
    if (armed) {
      int xMapped = adjustForDeadzone(map(xValue, 255, 0, -255, 255));
      int yMapped = adjustForDeadzone(map(yValue, 0, 255, -255, 255));
      leftOutput = (yMapped - xMapped);
      rightOutput = (yMapped + xMapped);
      goingForward = !(leftOutput < 0 && rightOutput < 0);
      if (leftOutput > 0) digitalWrite(relay1Pin, HIGH);
      if (leftOutput < 0) digitalWrite(relay1Pin, LOW);
      if (rightOutput > 0) digitalWrite(relay2Pin, HIGH);
      if (rightOutput < 0) digitalWrite(relay2Pin, LOW);
      leftOutput = abs(leftOutput);
      rightOutput = abs(rightOutput);
    }
    else {
      leftOutput = 0;
      rightOutput = 0;
    }
    analogWrite(leftPowerPin, leftOutput);
    analogWrite(rightPowerPin, rightOutput);
    delay(15);
}
