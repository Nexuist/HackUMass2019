# TurboChair

Innovation Without Purpose

**UPDATE:** I wrote a [blog post about this thing!](https://duro.me/stories/2019/10/23/ballad-of-turbochair)

## Overview

![A side view of TurboChair](docs/side.jpeg)

The TurboChair electronics system contains both a Raspberry Pi and an Arduino. The Arduino is responsible for mission critical controls while the Pi drives the infotainment touchscreen display.

The Arduino pairs remotely with a Nyko Kama Wii nunchuck, allowing the driver to control the chair no matter where they are.

![The Kama nunchuck](docs/kama.png)

## Operator's Guide

To drive the chair, turn on the emergency shut off switch and also turn on the nunchuck remote. Once connected (shown via a solid blue LED on the nunchuck), hold the Z button on the nunchuck to "arm" drive mode and use the joystick to provide power. As a safety measure, you must hold down the Z button in order to continue moving. On the touchscreen you will see various metrics such as the output for each motor and your joystick input. When reversing, you will see an "R" on the screen to indicate that both motors are inverted. At all times you will have a rear view livestream to maintain situational awareness.

To debug the chair without risk of unintentional motion, the user can simply connect a 5V cable to the Arduino. This will enable the user to control the relays and motor controllers without causing the motors to move themselves.

## Hardware

- There is an emergency shut off switch that must be turned on before the chair receives power from its on board battery.

- There are two motor controllers, one for each motor. There is also a relay for each motor that allows us to reverse each motor's direction individually. These relays and motor controllers are controlled in `turbo.ino` using commands from the nunchuck.

- There is an RGB LED light strip that can be controlled with its own remote.

- The Raspberry Pi Model 4 is mounted to the back of the 7" infotainment touchscreen LCD.

- A webcam is mounted in the back to provide a rear view livestream.

## Software

The Arduino firmware lies in `turbo/turbo.ino.` Also in `turbo/` is `kama.ino` which is the first version of our nunchuck code.

The Raspberry Pi runs two programs: `bridge.py` and Google Chrome, which is opened to `index.html`.

`bridge.py` starts a WebSocket server that broadcasts the following values:

- Is the chair armed?
- Is the chair moving forward?
- Joystick X value
- Joystick Y value
- Left motor power value
- Right motor power value

`index.html` is our infotainment system. It is a Vue app that connects to the bridge server and the USB webcam and shows both on the same screen.

## Future

In the future we hope to incorporate object tracking and autonomous driving to improve the lives of our users. In addition each chair will come with its own private blockchain.
