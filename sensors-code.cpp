int pushButton = 1; // pushbutton connected to digital pin 2
int proximSensor = A0;
int buttonStatus = 0; // variable to store the read value
int proximReading = 0;

void setup()
{
    pinMode(pushButton, INPUT); // sets the digital pin 2 as input
    pinMode(proximSensor, INPUT);
}

void loop()
{
    buttonStatus = digitalRead(pushButton); // read the input pin
    proximReading = analogRead(proximSensor);
    Serial.println(buttonStatus);
    Serial.println(proximReading);
}