/*
 *                SSSSS  kk                            tt
 *               SS      kk  kk yy   yy nn nnn    eee  tt
 *                SSSSS  kkkkk  yy   yy nnn  nn ee   e tttt
 *                    SS kk kk   yyyyyy nn   nn eeeee  tt
 *                SSSSS  kk  kk      yy nn   nn  eeeee  tttt
 *                               yyyyy
 *
 * SkynetClient for http://skynet.im, OPEN COMMUNICATIONS NETWORK & API FOR
 * THE INTERNET OF THINGS.
 *
 * This sketch uses the MQTT library to connect to skynet. It sends a message
 * on successful connection and logs data every second.
 *
 * Requires the MQTT library from Nick O'Leary http://knolleary.net/arduino-client-for-mqtt/
 * And one modification in PubSubClient.h, increase MQTT_MAX_PACKET_SIZE from 128 to something like 256
 *
 * Works with ethernet shields compatible with EthernetClient library from
 * Arduino. If you don't know, grab the original 
 * http://arduino.cc/en/Main/ArduinoEthernetShield
 *
 * Remember not to mess with ethernet's unavailable pins (10, 11, 12, 13 and 4 if using SD card)
 *
 * You will notice we're using F() in Serial.print. It is covered briefly on
 * the arduino print page but it means we can store our strings in program
 * memory instead of in ram.
 *
 */

/* Includes ------------------------------------------------------------------*/  
#include "application.h"
#include "PubSubClient.h"

/* Function prototypes -------------------------------------------------------*/
void onMessage(char* topic, byte* payload, unsigned int length);
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);

char server[] = "skynet.im";

//Your UUID and token for skynet.im TODO where to get one
char UUID[]  = "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX";
char TOKEN[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

//another UUID well send a message to
char TOUUID[] = "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"; 

TCPClient client;
PubSubClient skynet(server, 1883, onMessage, client);

//we'll run this if anyone messages us
void onMessage(char* topic, byte* payload, unsigned int length) {

  // handle incoming messages, well just print it for now
  Serial.println(topic);
  for(int i =0; i<length; i++){
    Serial.print((char)payload[i]);
  }    
  Serial.println();
}

void setup()
{

  Spark.function("digitalread", tinkerDigitalRead);
  Spark.function("digitalwrite", tinkerDigitalWrite);

  Spark.function("analogread", tinkerAnalogRead);
  Spark.function("analogwrite", tinkerAnalogWrite);

  Serial.begin(9600);
  
}

void loop()
{
  //we need to call loop for the mqtt library to do its thing and send/receive our messages
  if(skynet.loop()){
  
    //Lets log some data every time through the loop!
    //create a json string like "light":122
    //don't forget to escape those quotes!
    //here we grab whatever is hooked up to A0
    String messageString = "{\"light\":"+ String(int(analogRead(A0))) + "}";
    
    //housekeeping to turn our string into a character array
    char message[messageString.length()+1];
    messageString.toCharArray(message, messageString.length()+1);
  
    //Send away! The logging endpoint is called 'data'
    skynet.publish("data",message);
    
    //small delay here so we dont send too many mesages
    //but avoid delays and long running code, we need to let loop do its work!
    delay(1000);
  }else
  {
    //oops we're not connected yet or we lost connection
    Serial.println(F("connecting..."));
      
    // skynet doesnt use client so send empty client string and YOUR UUID and token
    if (skynet.connect("", UUID, TOKEN)){

      //success!
      Serial.println(F("connected"));

      //you need to subscribe to your uuid to get messages for you
      skynet.subscribe(UUID);
      
      //Lets send a message! give the destination and payload in a json object
      //{"devices":"XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX","payload":"hi!"}
      //don't forget to escape those quotes!
      //also note the wifi shield has a character limit of like 80 so stay below that
      String messageString = "{\"devices\":\"" + String(TOUUID) + "\",\"payload\":\"hi!\"}" ;
  
      //housekeeping to turn our string into a character array
      char message[messageString.length()+1];
      messageString.toCharArray(message, messageString.length()+1);
    
      //Send away! The device to device endpoint is called 'message'
      skynet.publish("message",message);
    }
  } 
}

/**
 ******************************************************************************
 * @file    application.cpp
 * @authors  Satish Nair, Zachary Crockett and Mohit Bhoite
 * @version V1.0.0
 * @date    05-November-2013
 * @brief   Tinker application
 ******************************************************************************
  Copyright (c) 2013 Spark Labs, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */
 
/*******************************************************************************
 * Function Name  : tinkerDigitalRead
 * Description    : Reads the digital value of a given pin
 * Input          : Pin 
 * Output         : None.
 * Return         : Value of the pin (0 or 1) in INT type
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerDigitalRead(String pin)
{
  //convert ascii to integer
  int pinNumber = pin.charAt(1) - '0';
  //Sanity check to see if the pin numbers are within limits
  if (pinNumber< 0 || pinNumber >7) return -1;

  if(pin.startsWith("D"))
  {
    pinMode(pinNumber, INPUT_PULLDOWN);
    return digitalRead(pinNumber);
  }
  else if (pin.startsWith("A"))
  {
    pinMode(pinNumber+10, INPUT_PULLDOWN);
    return digitalRead(pinNumber+10);
  }
  return -2;
}

/*******************************************************************************
 * Function Name  : tinkerDigitalWrite
 * Description    : Sets the specified pin HIGH or LOW
 * Input          : Pin and value
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerDigitalWrite(String command)
{
  bool value = 0;
  //convert ascii to integer
  int pinNumber = command.charAt(1) - '0';
  //Sanity check to see if the pin numbers are within limits
  if (pinNumber< 0 || pinNumber >7) return -1;

  if(command.substring(3,7) == "HIGH") value = 1;
  else if(command.substring(3,6) == "LOW") value = 0;
  else return -2;

  if(command.startsWith("D"))
  {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, value);
    return 1;
  }
  else if(command.startsWith("A"))
  {
    pinMode(pinNumber+10, OUTPUT);
    digitalWrite(pinNumber+10, value);
    return 1;
  }
  else return -3;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogRead
 * Description    : Reads the analog value of a pin
 * Input          : Pin 
 * Output         : None.
 * Return         : Returns the analog value in INT type (0 to 4095)
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerAnalogRead(String pin)
{
  //convert ascii to integer
  int pinNumber = pin.charAt(1) - '0';
  //Sanity check to see if the pin numbers are within limits
  if (pinNumber< 0 || pinNumber >7) return -1;

  if(pin.startsWith("D"))
  {
    pinMode(pinNumber, INPUT);
    return analogRead(pinNumber);
  }
  else if (pin.startsWith("A"))
  {
    pinMode(pinNumber+10, INPUT);
    return analogRead(pinNumber+10);
  }
  return -2;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogWrite
 * Description    : Writes an analog value (PWM) to the specified pin
 * Input          : Pin and Value (0 to 255)
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerAnalogWrite(String command)
{
  //convert ascii to integer
  int pinNumber = command.charAt(1) - '0';
  //Sanity check to see if the pin numbers are within limits
  if (pinNumber< 0 || pinNumber >7) return -1;

  String value = command.substring(3);

  if(command.startsWith("D"))
  {
    pinMode(pinNumber, OUTPUT);
    analogWrite(pinNumber, value.toInt());
    return 1;
  }
  else if(command.startsWith("A"))
  {
    pinMode(pinNumber+10, OUTPUT);
    analogWrite(pinNumber+10, value.toInt());
    return 1;
  }
  else return -2;
}
