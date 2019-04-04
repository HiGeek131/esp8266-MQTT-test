import paho.mqtt.client as mqtt


mqtt_server = '192.168.31.200'


def on_connect(client, uerdata, flage, rc):
    print('Connected with result code' + str(rc))
    client.subscribe('chat')


def on_message(client, userdata, msg):
    print(msg.topic + ' :' + str(msg.payload))


if __name__ == '__main__':
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(mqtt_server, 1883, 60)
    client.loop_forever()
