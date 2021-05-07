#!/usr/bin/python

import spidev
import time
from gpiozero import OutputDevice

chipEnable = OutputDevice('BOARD5')
chipEnable.active_high = True
chipEnable.off()
loadEnable = OutputDevice('BOARD7')
loadEnable.active_high = True
loadEnable.off()

# Register values (obtained using ADIsimPLL and ADF4158 evaluation software)
R0 =        [0x80, 0x24, 0xB8, 0x00]
R1 =        [0x00, 0x00, 0x00, 0x01]
R2 =        [0x02, 0x10, 0xFF, 0xFA]
R3 =        [0x00, 0x00, 0x00, 0x43]
R4 =        [0x00, 0x18, 0xC8, 0x04]
R5_load1 =  [0x00, 0x4B, 0xFF, 0xFD]
R5_load2 =  [0x00, 0x80, 0x00, 0x7D]
R6_load1 =  [0x00, 0x00, 0x00, 0x56]
R6_load2 =  [0x00, 0x80, 0x00, 0x06]
R7 =        [0x00, 0x02, 0xFF, 0xFF]

spi0 = spidev.SpiDev()
# Use of /dev/spidev0.0, SPI0 with CE0 not used.
# SCLK: BOARD23
# MOSI: BOARD19
spi0.open(0,0)
spi0.max_speed_hz = 122000
# CPOL=0, CPHA=1
spi0.mode = 0b00

def transferRegister(register):
    spi0.xfer(register)
    # Load into the specific 32-bits latch:
    time.sleep(1e-4)
    loadEnable.on()
    time.sleep(1e-4)
    loadEnable.off()
    time.sleep(1e-4)

# Enable ADF4158 for SPI programming
print('SPI communication started...')
chipEnable.on()
time.sleep(1e-4)

transferRegister(R7)
transferRegister(R6_load1)
transferRegister(R6_load2)
transferRegister(R5_load1)
transferRegister(R5_load2)
transferRegister(R4)
transferRegister(R3)
transferRegister(R2)
transferRegister(R1)
transferRegister(R0) # last one to be loaded (double-buffered)

spi0.close()
time.sleep(1e-4)
# Disable ADF4158 after successfull programming
chipEnable.off()
print('SPI communication ended.')