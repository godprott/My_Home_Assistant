#!/usr/bin/python3

import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BCM)
GPIO.setup(23,GPIO.OUT)

msg=0
def on_message(client, userdata, message): #ham callback nhan tin nhan tu broker
    global msg
    msg=int(message.payload.decode("utf-8"))
    print("msg: ",str(msg))
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)
    #print("msg: ",str(msg))
    if (msg==0):
        GPIO.output(23,1) #TAT
        client.publish("control/light/return","0",retain=True) #tra ket qua ve cho server mqtt
        print("return 0")
    else:
        GPIO.output(23,0)   #BAT
        client.publish("control/light/return","1",retain=True) #rerain dc kich hoat
        print("return 1")
    
def on_disconnect(client, userdata, rc): #neu disconnect?
    if rc != 0:
        print("Disconnect khong mong muon")
        
def on_connect(client, userdata, flags, rc):
    if rc==0:
        print("connected OK Returned code=",rc)
        client.subscribe("control/light")
        GPIO.output(23,1) #TAT
        client.publish("control/light/return","0",retain=True)
    else:
        print("Bad connection Returned code=",rc)
        
        
broker_address="192.168.1.167"
client = mqtt.Client("camera_control_light")
client.on_connect=on_connect
client.on_disconnect=on_disconnect
client.on_message=on_message
client.username_pw_set(username="mymqtt", password="mymqtt")
client.connect(broker_address)


try:
        client.loop_forever() #can thiet de ham callback co tac dung, cx co tac dung khi disconnect se tu dong reconnect
except KeyboardInterrupt:
    pass
except:
    pass
finally:
        client.disconnect() # disconnect
        GPIO.cleanup()
