#include <ArduinoMqttClient.h>
#include <WiFi101.h>

const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "vFD4C8D27C266AF8";     //device ID on Pushingbox for our Scenario

const char* MY_SSID = "Vineyard";
const char* MY_PWD =  "thelastwillbefirst";

int status = WL_IDLE_STATUS;
char server[] = "api.thingspeak.com"; // name address for Google (using DNS)

int pushButton = 1; // pushbutton connected to digital pin 2
int proximSensor = A0;

int proximReading = 0; // raw sensor val

// Variables will change:
int buttonState = 0;     // current state of the button
int lastButtonState = 0; // previous state of the button

int proxState = 0; // 
int lastProxState = 0;

WiFiClient wifiClient; //Instantiate WiFi object, can scope from here or Globally
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.hivemq.com";
int        port     = 1883;

void setup()
{
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD)
    {
        Serial.println("WiFi shield not present");
        // don't continue:
        while (true)
            ;
    }

    pinMode(pushButton, INPUT);
    pinMode(proximSensor, INPUT);

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(MY_SSID);
        //Connect to WPA/WPA2 network.Change this line if using open/WEP network
        status = WiFi.begin(MY_SSID, MY_PWD);

        // wait 2 seconds for connection:
        delay(2000);
    }

    Serial.println("Connected to wifi");
    printWifiStatus();

    //MQTT things
      Serial.println("Attempting to connect to the MQTT broker: ");
      Serial.println(broker);
    
      if (!mqttClient.connect(broker, port)) {
        Serial.println("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());
    
        while (1); //
      }
    
      Serial.println("You're connected to the MQTT broker!");
      Serial.println();
}

void loop()
{
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();

  buttonState = digitalRead(pushButton); // read the input pin
  proximReading = analogRead(proximSensor);
  proxState = proxEval(proximReading);
  

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
      // if the state has changed, increment the counter
      if (buttonState == HIGH) {
        //do stuff when tap turns on
        publishMqtt("washroomprojectdata",String(buttonState) + "_" + String(proxState));
        Serial.println("->button pressed");
        Serial.println("");
      }
      else {
        // do stuff when tap turns off
        publishMqtt("washroomprojectdata",String(buttonState) + "_" + String(proxState));
        Serial.println("<-button released");
        Serial.println("");
      }
      // Delay a little bit to avoid bouncing
      delay(100);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
  
  // compare proximity state to last state
  if (proxState != lastProxState) {
    if (proxState == HIGH) {
      //do stuff when someone walks in
      publishMqtt("washroomprojectdata",String(buttonState) + "_" + String(proxState));
      Serial.println("...someone's inside!");
      Serial.println("");
    }
    else {
      //do stuff when someone leaves
      publishMqtt("washroomprojectdata",String(buttonState) + "_" + String(proxState));
      publishMqtt("washroomproject","ON");
      Serial.println("...someone left!");
      Serial.println("");
    }
    // Delay a little bit to avoid bouncing
    delay(100);
  }
  lastProxState = proxState;
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void publishMqtt(String topic, String msg) {
  // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage(topic);
  mqttClient.print(msg);
  mqttClient.endMessage();

  // prints message
  Serial.print("Publishing to: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.print(msg);
  Serial.println();
} // sends msg to designated broker

int proxEval(int reading) {
  if (reading < 480){
    return 1;
  }
  else {
    return 0;
  }
} // turns raw sensor value into 0 or 1



