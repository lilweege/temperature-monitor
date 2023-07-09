# temperature-monitor
Simple ESP32 temperature monitor with logging web server

### Hardware

- [ESP32C3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/hw-reference/esp32c3/user-guide-devkitc-02.html)
- [BMP180](https://media.digikey.com/pdf/Data%20Sheets/Bosch/BMP180.pdf)

The ESP is probably overkill for this project, any arduino-like board should suffice. In any case, the wiring for my setup is as follows:
```
VIN - 3V3
GND - GND
SCL - 4
SDA - 5
```
Note that the pins used for SCL and SDA should support I2C on your board.

### Sketch Compilation

#### Libraries
- Wire (built in)
- [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)
- [HTTPClient](https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient)

#### Definitions
Add the following definitions to your compilation command, or define them in the file itself.
```
-DWIFI_SSID="<YOUR SSID>"
-DWIFI_PASS="<YOUR PASS>"
-DSERVER_URL="<YOUR URL>"
-DSERVER_USER="<YOUR AUTH USER>"
-DSERVER_PASS="<YOUR AUTH PASS>"
```

### Web Server
On your machine, run the web server with the following arguments.
```sh
python LoggingServer.py <YOUR AUTH USER> <YOUR AUTH PASS>
```
