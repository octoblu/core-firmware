#Octoblu microblu for Spark

This sketch will make a rest call to get its own uuid and token and store it to the eeprom until it has been invalidated by the server at which point it will be over written with new credentials.

See octoblu.com for more info

##Install (easy - requires Node and npm)
* Download core-firmware.bin from the build folder above or https://github.com/octoblu/core-firmware/raw/skynet-mqtt-firmata/build/core-firmware.bin
* Follow the [instructions](http://docs.spark.io/start/) to get your spark core set up for the first time. 
* Log in to https://www.spark.io/build
* Click cores in the bottom left corner
* In the white box that slides out, find the name of your spark and click the down arrow to reveal the core id
* Make sure your core is connected and updated and breathing Cyan. See http://docs.spark.io/ for more info
* Install [node.js](http://nodejs.org/)
* Are installing open any terminal if on Mac or Linux. On windows open the node.js command prompt.
* Install the spark command line tools by entering this into your terminal.  ```npm install -g spark-cli```
* Login with ```spark cloud login```
* Flash the firmware ```spark cloud flash YOURCOREIDHERE core-firmware.bin```


You can find your Octoblu UUID by logging into Octoblu and going to the Channels page and clicking on the Spark.io icon. If you're on the same network as the spark core it will show up as an unclaimed device.


You can also find it by looking on the serial console at 115200 but beware, it only prints on startup and its hard to catch!

##Use it
* See @monteslu's Skynet Serial example https://github.com/octoblu/serial/tree/master/examples/firmata/SkynetSerial

##Gotchas
* Many firmata clients are too chatty. Firmata was a usb serial protocol first where bandwidth and latency weren't a thing. Firmata.js https://github.com/jgautier/firmata/ specifically asks for the state and turns on reporting for every pin which currently runs so long the Spark reboots. Throttle your requests and don't bother getting the entire board state unless you need it. See Skynet serial above for an example that uses our fork https://github.com/monteslu/firmata/tree/nohandshake with the skipHandshake flag. 
* Requires Spark 0.3.3 As of this writing, you need to update your core from ARM toolchain, but the fix should be out in a release shortly. https://github.com/spark/core-firmware/compare/feature/release3.3 There was a bug in Spark prior to that which swapped input and output enums meaning you have to set input when you mean output, and reverse https://github.com/spark/core-firmware/issues/282
