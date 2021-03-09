#include <SPI.h>
#define BYTES_PER_REGISTER 4

void transferRegister(const byte registerName[]);

void trigger();

// SPI pinout
const int loadEnablePin = 8;
// Data is transferred from the shift register to one of eight latches on the rising edge of loadEnablePin.
const int chipEnablePin = 9;
const int chipSelectNegPin = 10; // must be set as OUTPUT, also if not used.
const int clockPin = 13;
const int serialOutPin = 11;

/*
 * DSO external triggering:
 * Several transitions of clockPin occurs before the SPI initialization.
 * This additional signal is used to trigger the DSO (one shot) when you want.
 */
const int externalTriggerPin = 7;

// Register values (obtained using ADIsimPLL and ADF4158 evaluation software):
const byte R0[] =       { 0x00, 0x00, 0x00, 0x00};
const byte R1[] =       { 0x00, 0x00, 0x00, 0x00};
const byte R2[] =       { 0x00, 0x00, 0x00, 0x00};
const byte R3[] =       { 0x00, 0x00, 0x00, 0x00};
const byte R4[] =       { 0x00, 0x00, 0x00, 0x00};
const byte R5_ramp1[] = { 0x00, 0x00, 0x00, 0x00};
const byte R5_ramp2[] = { 0x00, 0x00, 0x00, 0x00};
const byte R6_ramp1[] = { 0x00, 0x00, 0x00, 0x00};
const byte R6_ramp2[] = { 0x00, 0x00, 0x00, 0x00};
byte R7[] =             { 0x00, 0x00, 0x00, 0x00};

void setup() {
  pinMode(chipSelectNegPin,OUTPUT);
  pinMode(loadEnablePin,OUTPUT);
  digitalWrite(loadEnablePin,LOW);
  pinMode(externalTriggerPin,OUTPUT);
  pinMode(externalTriggerPin,LOW);
  pinMode(chipEnablePin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(serialOutPin,OUTPUT);
  
  Serial.begin(9600);
  Serial.println("SPI communication started...");

  SPI.beginTransaction(SPISettings(125000,MSBFIRST,SPI_MODE0)); // min. 125kHz; strange signals on clockPin line due to this line.
  trigger();
  digitalWrite(chipEnablePin,LOW);
  delayMicroseconds(20);
  digitalWrite(chipEnablePin,HIGH);
  delayMicroseconds(20);

  transferRegister(R7);
  transferRegister(R6_ramp1);
  transferRegister(R6_ramp2);
  transferRegister(R5_ramp1);
  transferRegister(R5_ramp2);
  transferRegister(R4);
  transferRegister(R3);
  transferRegister(R2);
  transferRegister(R1);
  transferRegister(R0); //last one to be loaded (double-buffered).

  delayMicroseconds(20);
  digitalWrite(chipEnablePin,LOW);
  delayMicroseconds(20);
  SPI.endTransaction();

  Serial.println("SPI communication ended.");

  }

void loop() {
  
}

void transferRegister(const byte registerName[]){
  // Data transfer into shift register:
  for (int index=0; index<BYTES_PER_REGISTER; index++){
    SPI.transfer(registerName[index]);
  }
  // Load into the specific 32-bits latch:
  delayMicroseconds(20);
  digitalWrite(loadEnablePin,HIGH);
  delayMicroseconds(100);
  digitalWrite(loadEnablePin,LOW);
  return;
}

void trigger(){
  digitalWrite(externalTriggerPin,HIGH);
  delayMicroseconds(20);
  digitalWrite(externalTriggerPin,LOW);
  return;
}

/* Oscilloscope settings:
 *  channel 1: serialOutPin or chipEnablePin
 *  channel 2: clockPin
 *  trigger: EXT, level:1.2V, mode:edge, slope:positive, sweep:single
 *  vertical scale: 2V for both channels
 *  horizontal scale: 200us, trigger anticipated of 1.184ms
 */
