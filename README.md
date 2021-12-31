# Signpost
An electronic signpost with automatic left/right signals for sports. 

## Interface

The signpost uses a motion sensor to trigger an event, which shows a left or right signal (random order or alternating). The signpost can also flash every 5, 10, 15, or 30 seconds. Modes are selected by the mode button. 

## Components

* TP4056 Li-poly charger and under-voltage protection circuit

  * Amazon
  * May need to modify to reduce charging rate for a lower energy cell. See the datasheet for current-limiting R values.

* PIR (motion) sensor

  * https://www.adafruit.com/product/189

  * remove/bypass the voltage regulator to power from 3.3V

    <img src="./references/PIR_module_vreg_bypass.png" alt="Voltage regulator bypass" style="zoom:50%;" />

* Adafruit Trinket M0

  * https://www.adafruit.com/product/3500

* Neopixels (WS2812B)

  * https://www.adafruit.com/product/4356
  * Future: add a custom PCB to hold these directly without having to wire them by hand...

* Momentary push button (momentary SPDT) 6mm

* Sliding power switch (DPDT)

* Li-Poly battery (400mAh+)





