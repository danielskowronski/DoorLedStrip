# DoorLedStrip

A custom project to drive RGBIC LED strip attached to door frame from a networked ESP32. The goal is to have an automatic night light and a signal light (similar to "on air" signs) controlled over a network (including Home Assistant and HomeKit) with extreme customization and automation.

The project is based around a cheap LED strip controlled by SK6812 (less than $6 per meter with 60 RGBW LEDs), ESP32 microcontroller and BH1750 ambient light sensor for nighttime operation. Since the entire system is powered from USB, there are also 2.5A PTC and INA219 current sensor in-line with LED power.

**This is a work in progress**

General project overview can be found below, more specific docs are in relevant subdirectories.

---

## Functionality

### Night light

If no other mode is activated, based on input from ambient light sensor, very dim white light is activated as a form of night light on the corridor. It also covers the situation when a closed door blocks any daylight to the corridor.

### Signal light

Using simple presets, user can trigger warning and indication signals visible from outside the room. For example, red may indicate that person inside has a video-call, yellow - when person is busy and pink, when some dangerous work is performed (e.g. soldering) and not everyone (e.g. cats) should enter or be let in accidentally.

Initially, it'll just be operated as a HomeKit device, later some physical buttons or dedicated remote will be added.

---

## Design decisions

### Why make it by myself in the first place

There are many smart LED strips from brands like Aqara which are great products for "normal use", but they lack complex automation ability, even if paired with smart home light sensors. It's especially problematic with custom logic. Letting completely external systems like Home Assistant control signal lights might prove unreliable and require network to work all the time.

On AliExpress and other places you may also find "ESP32 RGBIC" controllers, which seem to offer fully programmable ESP32, MQTT and app control support, as well as basic inputs with button and physical remote. However, I couldn't find any which exposes extra GPIO pins for attaching extra sensors easily.

Therefore, I decided to make my own to keep the system autonomous, very expandable. While it will end up probably costing similarly to an off-the-shelf LED strip with Zigbee controller, it'll be more fun.

### Power

The majority of power consumption comes from LED strip. However, I'm going only to use only 100cm of it - 80cm on top of the door frame and 10cm on both sides. Additionally, full light output will never be used, and it can be easily limited from microcontroller.

To make it easy to program/control the device at the same time it's powered, I decided to use regular USB and 5V. While the USB 2.0 standard states it allows 500mA drain, it's pretty much a standard in reliable chargers (and powered USB hubs) to get 2.4A per-port. It's safe to give 2A for the LED strip, which is plenty of power to generate a decent amount of light. Moreover, some ICs in LED strips like WS2812B and clones (SK6812) work with 3.3V logic, even if they are designed for 5V, so they can be driven directly from ESP32.

Since I'm not trusting cheap ESP32 devkits, I wanted to avoid using USB for data and extra power input. There are two other options I considered: 5V Power Delivery and 12V Power Delivery. 5V PD could give me guaranteed 3A, but would require accommodating PD trigger (cheap and easy to set, but additional component in-line) and sacrificing extra port on charger, while giving me only 25% more current (3A). 12V PD seems like the best choice, since it provides up to 60W in PD 3.0, but requires many extra components, mainly step-down for ESP32 and logic level converter for IC control.

I decided to go with 60 instead of 30 LED per meter variant of LED strip because provides light that is much more evenly spread (visible on such short distance in a package without waterproof coating and installed in a well visible place). Since I can control light output and monitor current, it's not a problem that the maximum declared power consumption is 18W (instead of 9W) per meter, which exceeds USB 5V power supplies. Additionally, LEDs are more efficient at lower power anyway.

To keep it safe, I'm taking a number of precautions. First, I'm not using `VIN` port of ESP32 devkit (which is direct line from USB socket on board) because it could burn tracks and does not provide any safety like fuse. I chose to use a USB-C socket mountable in a plastic enclosure (round and screwable) and join it internally with basic USB-C cable going into ESP32 devkit, but add extra high current path to VIN and GND. Positive rail goes via resettable fuse and INA219 terminals to the LED strip, and ground goes there directly. Therefore, I can monitor current and control LED outputs from ESP32, but the system is safe even when code malfunctions as the fuse is also in line. As a last resort, fire safety is guaranteed by a safe USB charger which should limit current.

### Sensors

For a start, I'm only using an ambient light sensor and no movement sensor. It could save some power to run only when someone is moving around, but it can both cause false-positives (e.g. cats) and fail to activate from around corners.

Later, some extra sensors may be added, preferably using the I2C bus to keep the number of wires low.

### Enclosure

For external connections, I chose screwable connectors: USB-C socket for panel mounting and cable gland for the rest of wires (3 thicker for LED, 4 thin for I2C).

I opted for a ready-made enclosure (Kradex Z58JH PS) instead of custom 3D-printed to avoid the hassle of printing holes for external connectors and to save time (even MVP requires an enclosure due to hanging wires and high current).
