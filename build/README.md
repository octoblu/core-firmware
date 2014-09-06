Octoblu microblu for Spark

See octoblu.com

This sketch will make a rest call to get its own uuid and token and store it to the eeprom until it has been invalidated by the server at which point it will be over written with new credentials.

#Install (easy - requires Node and npm)
* Download core-firmware.bin from above
* Log in to https://www.spark.io/build Find your core id 
* Click cores in the bottom left corner
* In the white box that slides out, find the name of your spark and click the down arrow to reveal the core id

Make sure your core is connected and updated and breathing Cyan. See http://docs.spark.io/ for more info


go to a command line and type 
```npm install -g spark-cli```

type and answer the questions
```spark cloud login```

type 
```spark cloud flash YOURCOREIDHERE core-firmware.bin```


You can find your Octoblu UUID by logging into octoblu, or by looking on the serial console


When the Spark is breathing Cyan, the serial port will be available at baud rate 115200. It only prints the uuid on startup though, so it can be hard to catch it!
