#include "application.h"

#include "pgmspace.h"

#include "ringbuffer.h"

#include "b64.h"

#include "jsmn.h"

#include "SkynetClient.h"

TCPClient client;
SkynetClient skynetclient(client);

char hostname[] = "skynet.im";
int port = 80;

void setup() {

	Serial.begin(115200);
	delay(5000);

    Serial.println("going in");
}

unsigned long lastBeat = 0;
void loop() {

  while(!skynetclient.monitor()){

    bool skynetStatus = false;
    do {
    	Serial.println("trying");
    	skynetStatus = skynetclient.connect(hostname, port);
    } while (!skynetStatus);
    
    Serial.println("Connected!");
    
    char uuid[UUIDSIZE];
  
    skynetclient.getUuid(uuid);
    Serial.print("uuid: ");
    Serial.println(uuid);
    
    skynetclient.getToken(uuid);
    Serial.print("token: ");
    Serial.println(uuid);

  }

  	//if its been 5 seconds
	if (((unsigned long)(millis() - lastBeat) >= 5000))
	{
		lastBeat = millis();
		//message to log MUST be comma seperated key value pair(s) and may
		//not be be an object or array
		skynetclient.logMessage("\"humidity\":123,\"temperature\":\"21C\"");
	}
}