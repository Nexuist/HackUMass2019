/*
This code taken from https://github.com/Nexuist/Wii-Kama-API
Written by me (Andi!) three years ago
*/
// #include <Wire.h>

// #define WII_I2C_ID 0x52
// #define MAX_STR_BUF 255

// struct NunchuckData
// {
//     uint16_t JoyX;
//     uint16_t JoyY;
//     uint16_t AccelX;
//     uint16_t AccelY;
//     uint16_t AccelZ;
//     uint16_t BtnZ;
//     uint16_t BtnC;
// };

// NunchuckData *currentNunchuckData = new NunchuckData();
// static uint8_t nunchuckDataBuffer[6];
// static uint8_t nunchuckIDBuffer[6];
// static uint32_t nunchuckPacketCounter = 0;

// void setup()
// {
//     Serial.begin(115200);
//     delay(100); // Wait for serial setup
//     // Power receiver by turning A2 & A3 into ground & power pins //
//     DDRC |= _BV(PORTC3) | _BV(PORTC2);
//     PORTC &= ~_BV(PORTC2);
//     PORTC |= _BV(PORTC3);
//     delay(100); // Wait for things to stabilize
//     nunchuckInit();
// }

// void loop()
// {
//     if (!nunchuckGetData())
//     {
//         Serial.print("FAIL: DATA\n");
//         delay(1000);
//         return;
//     }
//     else
//     {
//         // Send data through serial
//         // There's almost certainly a better way to do this
//         Serial.print("DATA: ");
//         Serial.print(currentNunchuckData->JoyX);
//         Serial.print("|");
//         Serial.print(currentNunchuckData->JoyY);
//         Serial.print("|");
//         Serial.print(currentNunchuckData->AccelX);
//         Serial.print("|");
//         Serial.print(currentNunchuckData->AccelY);
//         Serial.print("|");
//         Serial.print(currentNunchuckData->AccelZ);
//         Serial.print("|");
//         Serial.print(currentNunchuckData->BtnC);
//         Serial.print("|");
//         Serial.print(currentNunchuckData->BtnZ);
//         Serial.println();
//         delay(15);
//     }
//     // Not implemented:
//     // If buffer values 0-5 == 0x00 then data is pegged low
//     // If buffer values 0-5 == 0xFF then data is pegged high, reinit required
// }

// int nunchuckGetData()
// {
//     int counter = 0;
//     Wire.requestFrom(WII_I2C_ID, 6);
//     while (Wire.available())
//     {
//         nunchuckDataBuffer[counter] = Wire.read();
//         counter++;
//     }
//     // Request new payload for next time
//     Wire.beginTransmission(WII_I2C_ID);
//     Wire.write((uint8_t)0x00);
//     Wire.endTransmission();
//     if (counter != 6)
//     {
//         return 0; // Not all bytes acquired, fail
//     }
//     nunchuckPacketCounter++;
//     currentNunchuckData->JoyX = nunchuckDataBuffer[0];
//     currentNunchuckData->JoyY = nunchuckDataBuffer[1];
//     // Unpack byte 5 as ZZYYXXcz using voodoo magic
//     currentNunchuckData->BtnZ = (nunchuckDataBuffer[5] >> 0) & 1;
//     currentNunchuckData->BtnC = (nunchuckDataBuffer[5] >> 1) & 1;
//     currentNunchuckData->AccelX = (nunchuckDataBuffer[5] >> 2) & 3 | (nunchuckDataBuffer[2] << 2);
//     currentNunchuckData->AccelY = (nunchuckDataBuffer[5] >> 4) & 3 | (nunchuckDataBuffer[3] << 2);
//     currentNunchuckData->AccelZ = (nunchuckDataBuffer[5] >> 6) & 3 | (nunchuckDataBuffer[4] << 2);
//     return 1; // Success
// }

// void nunchuckInit()
// {
//     // NOTE - This method does NOT support encyption and as such will not work with official Nintendo controllers
//     // Reset packet counter
//     nunchuckPacketCounter = 0;
//     Serial.print("INIT: BEGIN\n");
//     Wire.begin(); // Join I2C bus as master
//     boolean INITIALIZING = true;
//     while (INITIALIZING)
//     {
//         // Send init handshake //
//         // It seems critical for F0:55 and FB:00 to be transmitted separately when it comes to the Nyko Kama
//         Wire.beginTransmission(WII_I2C_ID);
//         Wire.write((uint8_t)0xF0);
//         Wire.write((uint8_t)0x55);
//         Wire.endTransmission();
//         Wire.beginTransmission(WII_I2C_ID);
//         Wire.write((uint8_t)0xFB);
//         Wire.write((uint8_t)0x00);
//         Wire.endTransmission();
//         Wire.beginTransmission(WII_I2C_ID);
//         Wire.write((uint8_t)0xFA);
//         Wire.endTransmission();
//         Wire.requestFrom(WII_I2C_ID, 6, true); // Request 6 bytes and then release bus
//         delay(1);                              // Wait for response
//         // Acquire nunchuck ident //
//         int counter = 0;
//         while (Wire.available())
//         {
//             nunchuckIDBuffer[counter] = Wire.read();
//             counter++;
//         }
//         if (counter == 6)
//         {
//             // All bytes obtained
//             // Init complete
//             Serial.print("INIT: SUCCESS\n");
//             INITIALIZING = false;
//         }
//         else
//         {
//             Serial.print("INIT: FAIL\n");
//             delay(1000);
//         }
//     }
// }