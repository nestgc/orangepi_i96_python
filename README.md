# i96

Documentation and apps for the OrangePi-i96 board. Fixes the GPIO access, helps the WiFi keep a persistent MAC address and the [wiki](http://alt.pbeirne.com:3000/patb/i96/wiki) includes notes to help make the board more useable.

To get full access to the GPIO pins, download the `gpio_fixup.py` file onto your OrangePi-i96. I usually install it into `/usr/local/bin'.

``` 
sudo -i                  # change to root
cd /usr/local/bin
wget http://wiki.pbeirne.com/patb/i96/raw/master/gpio_fixup.py

# add a line to /etc/rc.local to execute this at startup
sed -i "/^exit 0$/i\/usr/local/bin/gpio_fixup.py" /etc/rc.local
```

That little script will run at boot time, and change the gpio pins on the 40 pin connector (pins 23-34) into true GPIO pins, and it'll restore proper operation of the UART2 and SPI2 pins (pins 3,9,8,10,12,14) **NOTE: this is OrangePi-i96 specific; do not use on the 2G-iot board**

`gpio_fixup.sh` is the equivalent script written in bash, using `devmem2` to access the cpu registers.

`devmem2.py` is a local version of the program to access the cpu registers. Use it if you can't download `devmem2` from the Debian/Ubuntu repositories. `devmem2.py` uses Python3 and is very slow, and must be run by root or sudo.

`blink.py` is just an example to show how you might play with the GPIO lines.

The `rdawfmac.ko` file included here contains the change that allows the WiFi MAC address to persist between reboots. This file (kernel module) can be copied into `/lib/modules/3.10.62-rel5.0.2+/kernel/drivers/net/wireless/rdaw80211/rdawlan/` to overwrite the existing file. If you decide to use this version of the module, be sure to create a folder off the root called `/data/misc/wifi/` so the module can store its randomized MAC address. **NOTE: this module is only useable on the 3.10.62-rel5.0.2 kernel.** It works on the OrangePi-2Giot board as well.

The `i2c` folder contains test programs for the I2C bus on this board. The native Linux `i2cdetect` from the `i2c-tools` packages doesn't seem to work very well on this board, so I wrote something similar in Python, it's `i2c/i2cscan2.py` and requires that you install the 'python-smbus' package; that package doesn't seem to exist yet for Python3, so I have included a local version called `smbus.py`. I have also included a couple of LCD demo programs, using the I2C version of the 4x20 LCD modules that are easily available.  The `pip3` library system is helpful in finding libraries to handle various peripherals.

**NOTE**: if you are going to use the I2C bus on the OrangePi-i96 board, you can access them through `/dev/i2c-1` and `/dev/i2c-2`; the other one `/dev/i2c-0` is used to communicate with the modem chip, and for the camera interface. If you are going to use I2C on the OrangePi-2Giot, avoid the `/dev/i2c-0` device (40 pin connector, pins 3/5) because it's used for communication between the CPU and the modem chips, and for the camera connector. Also on the 2Giot, the `/dev/i2c-2` bus is shared between the 40 pin connector and the LCD connector. Use `opio -2 statusx` to see the Linux driver name for each pin-group.
