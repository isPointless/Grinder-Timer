# Grinder-Timer

Simple Arduino sketch for a grind-on-demand grinder

#### > I copied a LOT of this sketch from other people - so I can't take credit.
#### > Still want to add a continuous ('single dose') grind mode.
#### > Check the definitions for pindef, the image isnt really up to date.

> VIDEO: https://youtube.com/shorts/ht_ll7_amRw?feature=share


BOM:
- Arduino Nano (clone or genuine) 
- SSD1306 0.96" OLED (https://aliexpress.com/item/1005001621838435.html?)
- 2 buttons, with 5v LEDs (https://nl.aliexpress.com/item/32988505438.html?) or (https://nl.aliexpress.com/item/1005001732343587.html?) or whatever you like.
- Rotary encoder with push button (https://nl.aliexpress.com/item/1005001621899401.html?) 
- 5V relay optocoupler relay + snubber (https://nl.aliexpress.com/item/1005004183334986.html?) + (https://nl.aliexpress.com/item/1005003128938478.html?) 
- AC - 12v power supply: (https://nl.aliexpress.com/item/33012749903.html?) 
- 12v - 5V step down: (https://nl.aliexpress.com/item/1005001821783419.html?) 


## How to?

Implementation instructions: >> Make sure you are familiar with the risks & have basic electronics knowledge. 

1) Unplug Grinder from all power connections, wait 30 minutes MINIMUM to allow the capacitors to discharge. 
2) Figure out your current circuit. Which probably only involved a momentary switch or timer. This is where the relay + snubber will connect
3) Make a test setup of everything - use 5V from your computers USB to power everything.
4) Download & install: Arduino IDE, VSCode. -> Install PlatformIO plugin on VSCode (with all required packages like python)
5) Change settings as necessary (ENC_TOL) / prob not necessary
6) Upload to Arduino with VS-code / PlatformIO
7) Test! You can see in the video how the setup is supposed to perform - that is however without LEDs.
8) Implement! Be careful with High voltage AC!

#### VIDEO:

https://www.youtube.com/watch?v=ht_ll7_amRw&ab_channel=WoutOsse


#### Questions: Please reach out on Discord: itsPointless#6048


#### HB Forum

https://www.home-barista.com/repairs/arduino-grind-on-demand-mod-t85577.html#p912410

