# -*- coding: utf-8 -*-
"""
Created on Thu Mar 24 02:11:28 2022

@author: JackC
"""

from serial import Serial as ser

Freq0=14850000
Freq1=148500000/1.001
Freq2=0

Freq0=int(Freq0)
Freq1=int(Freq1)
Freq2=int(Freq2)

sername="COM14"

def encode(Freq0,Freq1,Freq2):
    cmd=[0 for i in range(16)]
    cmd[0]=0xAA
    cmd[15]=0x55
    
    cmd[1]=(Freq0>>24)&0xFF
    cmd[2]=(Freq0>>16)&0xFF
    cmd[3]=(Freq0>>8)&0xFF
    cmd[4]=Freq0&0xFF
    
    cmd[5]=(Freq1>>24)&0xFF
    cmd[6]=(Freq1>>16)&0xFF
    cmd[7]=(Freq1>>8)&0xFF
    cmd[8]=Freq1&0xFF
    
    cmd[9]=(Freq2>>24)&0xFF
    cmd[10]=(Freq2>>16)&0xFF
    cmd[11]=(Freq2>>8)&0xFF
    cmd[12]=Freq2&0xFF
    
    CHK = 0;
    for i in range(13):	
        CHK ^= cmd[i];
    cmd[13]=CHK;
    
    CHK = 0
    for i in range(14):
        CHK += cmd[i];
    cmd[14]=(0-(CHK&0xFF))&0xFF;
    
    #print(cmd)
    
    #lin = ['%02X' % i for i in cmd]
    #print(" ".join(lin))
        
    cmd=bytes(cmd)

    return cmd

def decode(b):
    Actual0=int.from_bytes(b[1:5],byteorder='big',signed=False)
    Actual1=int.from_bytes(b[5:9],byteorder='big',signed=False)
    Actual2=int.from_bytes(b[9:13],byteorder='big',signed=False)
    
    return (Actual0,Actual1,Actual2)

cmd=encode(Freq0,Freq1,Freq2)

with ser(sername,1000000,timeout=0.1) as s:
    s.write(cmd)
    b=s.read(16)

actual = decode(b)

print("Target is Freq0:{0}, Freq1:{1}, Freq2:{2}".
      format(Freq0,Freq1,Freq2))
print("Actual is Freq0:{0}, Freq1:{1}, Freq2:{2}".
      format(actual[0],actual[1],actual[2]))
