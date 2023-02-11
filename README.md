# Grinder-Timer
Simple Arduino sketch for a grind-on-demand grinder

I copied a LOT of this sketch from other people

This is just my implementation of it, and it works well. Removed a lot of EEPROM statistics etc I didn't need. 
Added start-stop (i think? long time ago) functionality.
Altered the interface (I didn't like press&hold for example)

Still want to add a continuous ('single dose') grind mode.

VIDEO: https://youtube.com/shorts/ht_ll7_amRw?feature=share

BOM:
- Arduino Nano
- SSD1306 0.96" OLED (the very cheapest kind)
- 2 buttons, with 5v LEDs - I like the short throw ones https://nl.aliexpress.com/item/1005001732343587.html?
- Rotary encoder with push button (again, the very cheapest kind, don't forget to buy a knob to your liking)
- 5V relay optocoupler relay - In the images I use an SSR, I don't think this is wise for a grinder, atleast not without a snubber.

HB Forum

https://www.home-barista.com/repairs/arduino-grind-on-demand-mod-t85577.html#p912410

