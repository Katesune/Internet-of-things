# -*- coding: utf-8 -*-
"""
Created on Sun May 30 18:35:29 2021

@author: Adm
"""

import zmq
import socket as sock
import serial
import time 
from datetime import datetime

def find_value(ans):
    
    first_index = 0
    sec_index = 0
    
    for i in range(len(ans)):
        if ans[i]=='h':
            first_index = i+2
        if ans[i]=='E':
            sec_index = i-1
    
    ph_value = ans [first_index:sec_index]
    
    enc_value = ans [sec_index+4:len(ans)]
    
    return ph_value, enc_value

def check(ph_value, enc_value):
    if len(ph_values>0):
        for i in range(len(ph_values)):
            if ph_value==ph_values[i] and enc_value==enc_values[i]:
                return False
    return True

port = 1883
context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.connect(f"tcp://roboforge.ru:{port}")

s = serial.Serial("COM5", baudrate=9600)
s.timeout = 2
id_ = b"prep"

ph_values = []
enc_values = []

while True:
    try:
        answer = s.readline().decode().strip()
        if answer:
            print(answer)
            
            value1, value2 = find_value(answer)
            print(value1, value2)
            
            motion = 0
            
            if "detected" in answer:
                motion = 1
            else:
                motion = 0
                
            dt = datetime.utcnow().timestamp()
            dt = int(dt).to_bytes(4, "big")
            sub = id_ + dt + motion.to_bytes(1, "big")
            sub += "Значение фоторезистора - " + value1 + " "
            sub += "Значение энкодера - " + value2 + " "
            
            if (check(value1, value2)):
                socket.send(sub)
    
            
    except KeyboardInterrupt:
        break
    
s.close()