#include <Ethernet.h>// Ethernet.h library
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x06};
#include <PubSubClient.h>
#include <Wire.h>
#include "auth.h"
String ip = "";
int inPin = 49;
int reading;
int previous = LOW;
unsigned long time = 0;
long debounce = 200;
int state = HIGH;
int mqttRetryAttempt = 0;
char messageBuffer[100];
char topicBuffer[100];
char clientBuffer[50];
long lastActivityTime   = 0;
                      
EthernetClient ethClient;
PubSubClient mqttClient;

long previousMillis;

/*Relay pins*/
//const char r1 = 22;
//const char r2 = 23;
//const char r3 = 24;
//const char r4 = 25;
//const char r5 = 26;
//const char r6 = 27;
//const char r7 = 28;
//const char r8 = 29;
//const char r9 = 30;
//const char r10 = 31;
//const char r11 = 32;
//const char r12 = 33;
//const char r13 = 34;
//const char r14 = 35;
//const char r15 = 36;
//const char r16 = 37;
//const char r17 = 38;
//const char r18 = 39;
//const char r19 = 40;
//const char r20 = 41;
//const char r21 = 42;

/* ************************************************************************************* */
/* Button setup */
static byte lastButtonState[20] = {   0,  0,  0,  0,    0,  0,  0,  0,
                                      0,  0,  0,  0,    0,  0,  0,  0,
                                      0,  0,  0,  0     };


static byte relayArray[21]      =  { 22, 23, 24, 25, 26, 27, 28,
                                     29, 30, 31, 32, 33, 34, 35,
                                     36, 37, 38, 39, 40, 41, 42
                                   };



byte lastButtonPressed         = 0;
#define DEBOUNCE_DELAY 50
/* ************************************************************************************* */

/* topics */
#define R1_TOPIC "r1/r1"
#define R2_TOPIC "r1/r2"
#define R3_TOPIC "r1/r3"
#define R4_TOPIC "r1/r4"
#define R5_TOPIC "r1/r5"
#define R6_TOPIC "r1/r6"
#define R7_TOPIC "r1/r7"
#define R8_TOPIC "r1/r8"
#define R9_TOPIC "r1/r9"
#define R10_TOPIC "r1/r10"
#define R11_TOPIC "r1/r11"
#define R12_TOPIC "r1/r12"
#define R13_TOPIC "r1/r13"
#define R14_TOPIC "r1/r14"
#define R15_TOPIC "r1/r15"
#define R16_TOPIC "r1/r16"
#define R17_TOPIC "r1/r17"
#define R18_TOPIC "r1/r18"
#define R19_TOPIC "r1/r19"
#define R20_TOPIC "r1/r20"
#define R21_TOPIC "r1/r21"

String topicArray[20]     = {  "r1/r1", "r1/r2", "r1/r3", "r1/r4",
                               "r1/r5", "r1/r6", "r1/r7", "r1/r8",
                               "r1/r9", "r1/r10", "r1/r11", "r1/r12",
                               "r1/r13", "r1/r14", "r1/r15", "r1/r16", 
                               "r1/r17", "r1/r18", "r1/r19", "r1/r20"
                            }; 
                            
static byte buttonArray[20]     = {  8,  9,  7,  6,
                                     3,  5,  2 , 59,
                                     57, 58, 56, 55,
                                     43, 54, 44, 45, 
                                     46, 47, 48, 49
                                  };   
                                  
byte stateArray[20]     = {  0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0, 
                             0, 0, 0, 0
                          };                             

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.print(topic);
  Serial.println("");

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  if (strcmp(topic,R1_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r1, HIGH);
      stateArray[0] = HIGH;
    } else {
      digitalWrite(r1, LOW);
      stateArray[0] = 0;
    }
  } else if (strcmp(topic,R2_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r2, HIGH);
      stateArray[1] = 1;
    } else {
      digitalWrite(r2, LOW);
      stateArray[1] = 0;
    }
  } else if (strcmp(topic,R3_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      stateArray[2] = HIGH;
      digitalWrite(r3, 1);
    } else {
      digitalWrite(r3, LOW);
      stateArray[2] = 0;
    }
  } else if (strcmp(topic,R4_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      stateArray[3] = 1;
      digitalWrite(r4, HIGH);
    } else {
      digitalWrite(r4, LOW);
      stateArray[3] = 0;
    }
  } else if (strcmp(topic,R5_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r5, HIGH);
      stateArray[4] = 1;
    } else {
      digitalWrite(r5, LOW);
      stateArray[4] = 0;
    }
  } else if (strcmp(topic,R6_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r6, HIGH);
      stateArray[5] = 1;
    } else {
      digitalWrite(r6, LOW);
      stateArray[5] = 0;
    }
  } else if (strcmp(topic,R7_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r7, HIGH);
      stateArray[6] = 1;
    } else {
      digitalWrite(r7, LOW);
      stateArray[6] = 0;
    }
  } else if (strcmp(topic,R8_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r8, HIGH);
      stateArray[7] = 1;
    } else {
      digitalWrite(r8, LOW);
      stateArray[7] = 0;
    }
  } else if (strcmp(topic,R9_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r9, HIGH);
      stateArray[8] = 1;
    } else {
      digitalWrite(r9, LOW);
      stateArray[8] = 0;
    }
  } else if (strcmp(topic,R10_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r10, HIGH);
      stateArray[9] = 1;
    } else {
      digitalWrite(r10, LOW);
      stateArray[9] = 0;
    }
  } else if (strcmp(topic,R11_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r11, HIGH);
      stateArray[10] = 1;
    } else {
      digitalWrite(r11, LOW);
      stateArray[10] = 0;
    }
  } else if (strcmp(topic,R12_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r12, HIGH);
      stateArray[11] = 1;
    } else {
      digitalWrite(r12, LOW);
      stateArray[11] = 0;
    }
  } else if (strcmp(topic,R13_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r13, HIGH);
      stateArray[12] = 1;
    } else {
      digitalWrite(r13, LOW);
      stateArray[12] = 0;
    }
  } else if (strcmp(topic,R14_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r14, HIGH);
      stateArray[13] = 1;
    } else {
      /* we got '0' -> on */
      digitalWrite(r14, LOW);
      stateArray[13] = 0;
    }
  } else if (strcmp(topic,R15_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r15, HIGH);
      stateArray[14] = 1;
    } else {
      digitalWrite(r15, LOW);
      stateArray[14] = 0;
    }
  } else if (strcmp(topic,R16_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r16, HIGH);
      stateArray[15] = 1;
    } else {
      digitalWrite(r16, LOW);
      stateArray[15] = 0;
    }
  } else if (strcmp(topic,R17_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r17, HIGH);
      stateArray[16] = 1;
    } else {
      digitalWrite(r17, LOW);
      stateArray[16] = 0;
    }
  } else if (strcmp(topic,R18_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r18, HIGH);
      stateArray[17] = 1;
    } else {
      digitalWrite(r18, LOW);
      stateArray[17] = 0;
    }
  } else if (strcmp(topic,R19_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r19, HIGH);
      stateArray[18] = 1;
    } else {
      digitalWrite(r19, LOW);
      stateArray[18] = 0;
    }
  } else if (strcmp(topic,R20_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r20, HIGH);
      stateArray[19] = HIGH;
    } else {
      digitalWrite(r20, LOW);
      stateArray[19] = 0;
    }
  } else if (strcmp(topic,R21_TOPIC)==0) {
    if ((char)payload[0] == '1') {
      digitalWrite(r21, HIGH);
    } else {
      digitalWrite(r21, LOW);
    }
  }
}

void mqttconnect() {
  /* Loop until reconnected */
  while (!mqttClient.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "Mega2560";
    /* connect now */
    if (mqttClient.connect(clientId.c_str(), serverUsername.c_str(), serverPassword.c_str())) {
      Serial.println("connected");
      /* subscribe topic */
      mqttClient.subscribe(R1_TOPIC);
      mqttClient.subscribe(R2_TOPIC);
      mqttClient.subscribe(R3_TOPIC);
      mqttClient.subscribe(R4_TOPIC);
      mqttClient.subscribe(R5_TOPIC);
      mqttClient.subscribe(R6_TOPIC);
      mqttClient.subscribe(R7_TOPIC);
      mqttClient.subscribe(R8_TOPIC);
      mqttClient.subscribe(R9_TOPIC);
      mqttClient.subscribe(R10_TOPIC);
      mqttClient.subscribe(R11_TOPIC);
      mqttClient.subscribe(R12_TOPIC);
      mqttClient.subscribe(R13_TOPIC);
      mqttClient.subscribe(R14_TOPIC);
      mqttClient.subscribe(R15_TOPIC);
      mqttClient.subscribe(R16_TOPIC);
      mqttClient.subscribe(R17_TOPIC);
      mqttClient.subscribe(R18_TOPIC);
      mqttClient.subscribe(R19_TOPIC);
      mqttClient.subscribe(R20_TOPIC);
      mqttClient.subscribe(R21_TOPIC);
    } else {
      Serial.print("failed, status code =");
      Serial.print(mqttClient.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
      mqttRetryAttempt++;
    }
  }
}

void setup() {
 Serial.begin(9600);
  while (!Serial) {};
  Serial.println(F("MQTT Arduino Demo"));
  Serial.println();

  // setup ethernet communication using DHCP
  if (Ethernet.begin(mac) == 0) {
    //Serial.println(F("Unable to configure Ethernet using DHCP"));
    for (;;);
  }

  Serial.println(F("Ethernet configured via DHCP"));
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
  Serial.println();
 //convert ip Array into String
  ip = String (Ethernet.localIP()[0]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[1]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[2]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[3]);
  //Serial.println(ip);

  // setup mqtt client
  mqttClient.setClient(ethClient);

// Setting input pull-ups
  for( byte i = 0; i < 20; i++)
  {
    pinMode(buttonArray[i], INPUT_PULLUP);
  }

// Setting relay outputs
  for( byte i = 0; i < 21; i++)
  {
    pinMode(relayArray[i], OUTPUT);
  }  

  /* configure the MQTT server with IPaddress and port */
  mqttClient.setServer(serverHostname, 1883);
  /* this receivedCallback function will be invoked
    when client received subscribed topic */
  mqttClient.setCallback(receivedCallback);
  Serial.println("Everything Setup");
}

void loop() {
 if (!mqttClient.connected()) {
    mqttconnect();
  }
  mqttClient.loop();

  byte i;
  for( i = 0; i < 20; i++) {
    processButtonDigital( i );
  }
}

void processButtonDigital( byte buttonId )
{
    int sensorReading = digitalRead( buttonArray[buttonId] );
    //Serial.print(buttonId, DEC);
    //Serial.print(": ");
    //Serial.println(sensorReading, DEC);
    
    if( sensorReading == 0 )  // Input pulled low to GND. Button pressed.
    {
      Serial.println( "Button pressed" );
      if( lastButtonState[buttonId] == 0 )   // The button was previously un-pressed
      {
        if((millis() - lastActivityTime) > DEBOUNCE_DELAY)  // Proceed if we haven't seen a recent event on this button
        {
          lastActivityTime = millis();
    
          lastButtonPressed = buttonId;
          Serial.print( "transition on ");
          Serial.print( buttonId, DEC );
          Serial.print(" (input ");
          Serial.print( buttonArray[buttonId] );
          Serial.println(")");
        
//          String messageString = String(buttonId + 1);
          String messageString = String(stateArray[buttonId] == 0 ? 1 : 0);
          messageString.toCharArray(messageBuffer, messageString.length()+1);
        
          //String topicString = "device/" + String(panelId) + "/button";
          String topicString = String(topicArray[buttonId]);
          topicString.toCharArray(topicBuffer, topicString.length()+1);
          
          mqttClient.publish(topicBuffer, messageBuffer);
        
//          mqttClient.publish("buttons", messageBuffer);
//          mqttClient.publish(topicString, messageString);
        }
      } else {
        // Transition off
        //digitalWrite(statusArray[buttonId-1], LOW);
        //digitalWrite(13, LOW);
      }
      lastButtonState[buttonId] = 1;
    }
    else {
      lastButtonState[buttonId] = 0;
    }
}
