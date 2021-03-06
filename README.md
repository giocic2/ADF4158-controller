# ADF4158-controller
ADF4158 programming with RaspberryPi (Python) or Arduino.
## Configure your Raspberry Pi from terminal
### 1. Enable SPI interface
1. From Raspberry Pi terminal:
```
sudo raspi-config
```
2. Select "Interface options" and enable SPI interface.
3. Load kernel module:
```
sudo modprobe spi-2835
```
4. Install _spidev_ Python module:
```
sudo apt install python/dev python3/dev
```
```
cd ~
```
```
git clone https://github.com/doceme/py-spidev.git
```
```
cd py-spidev
```
```
make
```
```
sudo make install
```

### 2. Clone this repository
From terminal:
```
git clone https://github.com/giocic2/ADF4158-controller.git
```
### 3. Execute the script, after made it executable
```
chmod +x ./ADF4158-controller/Raspberry/ADF4158-controller.py
./ADF4158-controller/Raspberry/ADF4158-controller.py
```
### 4. (Optional) Update repository discarding local changes, and make the Python script executable again
```
git add --all
git reset --hard
git pull
chmod +x /Raspberry/ADF4158-controller.py
```