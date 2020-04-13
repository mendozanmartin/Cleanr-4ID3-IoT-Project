#include <WiFi101.h>
const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "vFD4C8D27C266AF8";     //device ID on Pushingbox for our Scenario

const char *MY_SSID = "Your Wifi SSID";
const char *MY_PWD = "Your Wifi Password";

int status = WL_IDLE_STATUS;
char server[] = "api.thingspeak.com"; // name address for Google (using DNS)

int pushButton = 1; // pushbutton connected to digital pin 2
int proximSensor = A0;
int buttonState = 0; // variable to store the read value
int proximReading = 0;

// Variables will change:
int buttonState = 0;     // current state of the button
int lastButtonState = 0; // previous state of the button

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

    pinMode(pushButton, INPUT); // sets the digital pin 2 as input
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
}

void loop()
{

    buttonState = digitalRead(pushButton); // read the input pin
    proximReading = analogRead(proximSensor);

    // if you get a connection, report back via serial:
    WiFiClient client; //Instantiate WiFi object, can scope from here or Globally

 
    // compare the buttonState to its previous state
    if (buttonState != lastButtonState)
    {
        // if the state has changed, increment the counter
        if (buttonState == HIGH)
        {
            // if the current state is HIGH then the button went from off to on:
            //Start or API service using our WiFi Client through PushingBox then relayed to Google
            if (client.connect(WEBSITE, 80))
            {
                client.print("GET /pushingbox?devid=" + devid + "&buttonState=" + (String)buttonState + "&proximReading" + (String)proximReading);

                // HTTP 1.1 provides a persistent connection, allowing multiple requests to be batched
                // or pipelined to an output buffer
                client.println(" HTTP/1.1");
                client.print("Host: ");
                client.println(WEBSITE);
                client.println("User-Agent: MKR1000/1.0");
                //client.println("Connection: close");//for MKR1000, unlike esp8266, do not close connection
                client.println();
                Serial.println("\nData Sent");
                client.stop();
            }

            delay(100000);
        }
        else
        {
            // if the current state is LOW then the button went from on to off:
            Serial.println("off");
        }
        // Delay a little bit to avoid bouncing
        delay(50);
    }
    // save the current state as the last state, for next time through the loop
    lastButtonState = buttonState;
}

void printWifiStatus()
{
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
