#include <Wire.h>

#define I2C_MSG_IN_SIZE    4
#define I2C_MSG_OUT_SIZE   4

#define CMD_DIGITAL_WRITE  1
#define CMD_DIGITAL_READ   2
#define CMD_ANALOG_WRITE   3
#define CMD_ANALOG_READ    4


unsigned long Interval = 100;
unsigned long LastRead;
unsigned long Now;
const byte ReadMin = 2 ;
const byte ReadMax = 7;
byte j;
byte Read;
byte Position = 0;
byte Found = 0;
byte Reported = 0;

volatile uint8_t sendBuffer[I2C_MSG_OUT_SIZE];

void setup()
{
  //Serial.begin(57600);
  for(j=ReadMin;j<=ReadMax;j++){
    pinMode(j,INPUT);
    digitalRead(j);
    delay(5);
  }
  Wire.begin(0x7f);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  LastRead = millis();
}

void loop() {
  Now=millis();
  if(Now - LastRead >= Interval){
    LastRead = Now;
    Found=0;
    for(j=ReadMin;j<=ReadMax;j++){
      Read = digitalRead(j);
      if(Read == 0){
        Found =  j;
        break;
      }
    }
    if(Found!=0 && Found != Position){
      Position=Found;
      Reported = Found - ReadMin+1;
      //Serial.println(Reported);
    }
  }
}


void receiveEvent(int count)
{
  if (count == I2C_MSG_IN_SIZE)
  {
    byte cmd = Wire.read();
    byte port = Wire.read();
    int value = Wire.read();
    value += Wire.read()*256;
    switch(cmd)
      {
        case CMD_DIGITAL_WRITE:
          if(port<ReadMin || port>ReadMax){
            pinMode(port,OUTPUT);
            digitalWrite(port,value);
          }
          break;
        case CMD_DIGITAL_READ:
          if(port<ReadMin || port>ReadMax){
            pinMode(port,INPUT_PULLUP);
            clearSendBuffer();
            sendBuffer[0] = digitalRead(port);
          }else{
            clearSendBuffer();
            sendBuffer[0] = 0;
          }
          break;
        case CMD_ANALOG_WRITE:
          if(port<4 || port>5){
            analogWrite(port,value);
          }
          break;
        case CMD_ANALOG_READ:
          if(port<4 || port>5){
            clearSendBuffer();
            int valueRead = analogRead(port);
            sendBuffer[0] = valueRead & 0xff;
            sendBuffer[1] = valueRead >> 8;
          }else{
            clearSendBuffer();
            int valueRead = map(Reported,0,6,0,1020);
            sendBuffer[0] = valueRead & 0xff;
            sendBuffer[1] = valueRead >> 8;
          }
          break;
      }
  }
}

void clearSendBuffer()
{
  for(byte x=0; x < sizeof(sendBuffer); x++)
    sendBuffer[x]=0;
}

void requestEvent()
{
  Wire.write((const uint8_t*)sendBuffer,sizeof(sendBuffer));
}
