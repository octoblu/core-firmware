#Octoblu microblu for Spark

This sketch will make a rest call to get its own uuid and token and store it to the eeprom until it has been invalidated by the server at which point it will be over written with new credentials.

See octoblu.com for more info

##Install (easy - requires Node and npm)
* Download core-firmware.bin from above
* Log in to https://www.spark.io/build
* Click cores in the bottom left corner
* In the white box that slides out, find the name of your spark and click the down arrow to reveal the core id
* Make sure your core is connected and updated and breathing Cyan. See http://docs.spark.io/ for more info
* install the spark command line tools ```npm install -g spark-cli```
* login with ```spark cloud login```
* flash the firmware ```spark cloud flash YOURCOREIDHERE core-firmware.bin```
*You can find your Octoblu UUID by logging into octoblu, or by looking on the serial console at 115200 but beware, it only prints on startup and its hard to catch!

##Use it
* See @monteslu's Skynet Serial example https://github.com/octoblu/serial/tree/master/examples/firmata/SkynetSerial

##Gotchas
* Many firmata clients are too chatty. Firmata was a usb serial protocol first where bandwidth and latency weren't a thing. Firmata.js https://github.com/jgautier/firmata/ specifically asks for the state and turns on reporting for every pin which currently runs so long the Spark reboots. Throttle your requests and don't bother getting the entire board state unless you need it. See Skynet serial above for an example that uses our fork https://github.com/monteslu/firmata/tree/nohandshake with the skipHandshake flag. 