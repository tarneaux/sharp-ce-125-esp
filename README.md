# Sharp CE-125 ESP

This is a program that can be used to control the sharp CE-125 thermal printer
(released in 1982) with an ESP32-based microcontroller like a Seeed XIAO.

It is heavily based on code from the
[cavefischer website](http://www.cavefischer.at/spc/html/CE-125_Print-to.html)
(in german).

For more information on the CE-125's communication protocol, see its operation
manual ([download](https://www.usersmanualguide.com/sharp/calculator/ce-125)).

## Usage

I highly doubt that anyone will ever use this except me, but here is a brief
description of the system.

I've by no means tried to document this so that other people could use it. If 
you have any issues or just want to tell me this was actually useful to you,
feel free to [send me an email](https://tarneo.fr/).

### Hardware

An ESP32C3-based microcontroller is connected to the sharp CE-125 through some
kind of level shifter (like a
[TI L293DNE](https://www.ti.com/product/L293D/part-details/L293DNE)).

The level shifter can use the ESP32C3's GND and +3.3V pins on one side and the
CE-125[^1] pins 2 (VCC) and 3 (GND).

All data goes from the ESP32 to the CE-125 through the level shifter, meaning
that:
- used pins from the ESP32 are all configured as outputs and connected to the
  level shifter's inputs.
- used pins from the CE-125 are all inputs and connected to the level shifter's
  outputs.


| ESP32C3 pin | CE-125 pin[^1] |
|-------------|----------------|
| D0          | 4              |
| D1          | 5              |
| D2          | 10             |
| D3          | 11             |

Also make sure the printer is plugged in (duh).

### Software

Software can be built and uploaded by running `make install`.

Since arduino-cli's devs made the questionable choice of requiring the exact
same directory name and main file name (in this case `printer.ino`), you'll have
to clone this repository into a directory called `printer`:
```sh
git clone https://github.com/tarneaux/sharp-ce-125-esp printer
cd printer
```
As an alternative you could rename the `printer.ino` file to match your
directory's name.

Since the ESP32C3 needs to connect to a WiFi network, you'll have to configure
an SSID and password by creating `secrets.h`:
```
#define SECRET_SSID "<your wifi ssid here>"
#define SECRET_PASS "<your wifi password here>"
```

After flashing to the ESP32 with `make upload`, you can try changing the HOST
variable in `example-client.py` and run the script.

The WiFi protocol is briefly described in `example-client.py`.

### Printing a character table

To print a character table, copy `table.ino.example` to `printer.ino` and
upload.

[^1]: All pins for the CE-125 are counted from the top and are the ones on the
    11-pin header where the "pocket computer" is meant to be plugged in.
