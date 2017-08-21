# ESPEasySlaves_ReadAndCacheDigitalPins
EspEasy Slave - read digital pins and push the active one to espeasy analog read via I2C

I needed to read 6 sensors very fast (at least a couple of times per second).

Having worked with the EspEasy library I  modified the EspEasySlaves library to make an Arduino Uno Slave that:
- on start, the location is set to 0 
- reads 6 sensors very fast (~10 times per second) - D2 to D7
- saves in memory the first sensor that has value 0 (for pin D2 saves 1, for pin D7 saves 6)
- when pin A4 or A5 is read by EspEasy via I2C, it returns the stored location as a fake analog read
