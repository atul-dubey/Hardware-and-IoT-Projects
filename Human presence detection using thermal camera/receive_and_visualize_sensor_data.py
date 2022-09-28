# Import required packages
import random
import json
import numpy as np
import time
from paho.mqtt import client as mqtt_client
import matplotlib.pyplot as plt

# Tell the MQTT broker address and port
broker = 'localhost'
#broker = 'broker.emqx.io'
port = 1883

# Tell the topic on which the sensor data is being sent
topic = "esp32/mlx"

# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 100)}'
# username = 'emqx'
# password = 'public'

# Function to connect to the MQTT broker
def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    # client.username_pw_set(username, password) # Use this line if the broker required authentication
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

# Function to subscribe to topic, decode the sensor data, and visualize it
def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        topic=msg.topic
        m_decode=str(msg.payload.decode("utf-8","ignore"))
        print("data Received type",type(m_decode))
        print("data Received",m_decode)
        print("Converting from Json to Object")
        m_in=json.loads(m_decode) #decode json data
        print(type(m_in))
        print("sensor reading = ",m_in["values"])
        arr = np.array(m_in["values"])
        newarr = arr.reshape(24, 32)
        im = plt.imshow(newarr)
        plt.show(block=False)
        plt.pause(3)
        

    client.subscribe(topic)
    client.on_message = on_message

# Function to run all the above
def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()

# Finally, call the run function
if __name__ == '__main__':
    run()
