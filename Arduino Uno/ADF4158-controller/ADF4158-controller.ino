#include <SPI.h>
#define BYTES_PER_REGISTER 4

void transferRegister(const byte registerName[]);

// SPI PinOut and Transmitted 16-bit Word
const int loadEnablePin = 8;
// Data is transferred from the shift register to one of eight latches on the rising edge of loadEnablePin.
const int chipEnablePin = 9;
const int clockPin = 13;
const int serialOutPin = 11;

int index=0;

// Register values (obtained with ADIsimPLL and ADF4150 evaluation software):
const byte R0[] = { B00000000,
                    B00000000,
                    B00000000,
                    B00000000};

const byte R1[] = { B00000000,
                    B00000000,
                    B00000000,
                    B00000000};

const byte R2[] = { B00000000,
                    B00000000,
                    B00000000,
                    B00000000};

const byte R3[] = { B00000000,
                    B00000000,
                    B00000000,
                    B00000000};

const byte R4[] = { B00000000,
                    B00000000,
                    B00000000,
                    B00000000};

const byte R5[] = { B00000000,
                    B00000000,
                    B00000000,
                    B00000000};

const byte R6[] = { B00000000,
                    B00000000,
                    B00000000,
                    B00000000};

const byte R7[] = { B00000000,
                    B00000000,
                    B00000000,
                    B00000000};

void setup() {
  pinMode(loadEnablePin,OUTPUT);
  digitalWrite(loadEnablePin,LOW);
  pinMode(chipEnablePin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(serialOutPin,OUTPUT);

  // Serial.begin(9600);

  // Serial.println("SPI communication started...");
  SPI.beginTransaction(SPISettings(125000,MSBFIRST,SPI_MODE0)); // min. 125kHz; strange signals on clockPin line due to this line.
  delay(1000);
  digitalWrite(chipEnablePin,LOW);
  delay(0.2);
  digitalWrite(chipEnablePin,HIGH);
  delay(0.2);

  transferRegister(R7);
  transferRegister(R6);
  transferRegister(R5);
  transferRegister(R4);
  transferRegister(R3);
  transferRegister(R2);
  transferRegister(R1);
  transferRegister(R0);
  
  delay(0.2);
  digitalWrite(chipEnablePin,LOW);
  delay(0.2);
  SPI.endTransaction();
  // Serial.println("SPI communication ended.");

  }

void loop() {

}

void transferRegister(const byte registerName[]){
  // Data transfer into shift register:
  for (index=1; index<=BYTES_PER_REGISTER; index++){
    SPI.transfer(registerName[BYTES_PER_REGISTER-index]);
  }
  // Load into the specific 32-bits latch:
  delay(0.2);
  digitalWrite(loadEnablePin,HIGH);
  delay(0.2);
  digitalWrite(loadEnablePin,LOW);
  return;
}

/* Oscilloscope settings:
 *  channel: serialOutPin or clockPin
 *  channel 2: chipSelectPin
 *  trigger: channel 2, mode:edge, slope:positive, sweep:single
 *  horizontal scale: 20us
 *  vertical scale: 2V for both channels
 *  trigger: 108.8 us earlier
 */
