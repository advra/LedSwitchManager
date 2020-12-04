# Led Switch Manager

The motivation for this was to provide  example of a simple expandable hardware manager for interrupts. It follows a singleton pattern and manages the hardware on a seperate thread. This project manages the blink rates of an LED and Switch to power off the device.

## Requirements:
 The target device is a Raspberry Pi but can be modified for other devices. If using with WiringPi [see here for install instructions.](http://wiringpi.com/download-and-install/)

## Build:

```
cd build && cd build
cmake .. && make
```

## Run
```
./example.a
```