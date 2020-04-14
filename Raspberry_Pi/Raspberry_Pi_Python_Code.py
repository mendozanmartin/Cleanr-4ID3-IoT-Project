from gpiozero import LED
import paho.mqtt.client as mqtt
from time import sleep

outputPin = LED(4)

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("washroomproject")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    #print(msg.topic+" "+str(msg.payload))
    print(str(msg.payload))
    spray(str(msg.payload))

def spray(mqtt_msg):
    if (mqtt_msg == "b'ON'"):
        outputPin.on()
        sleep(1.5)
        outputPin.off()
        print("Freshener sprayed")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("broker.hivemq.com", 1883, 60)


# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()