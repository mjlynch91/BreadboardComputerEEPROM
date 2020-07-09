#this is my own attempt to make a bin file so that I can program my EEPROM.
#Im doing this because I'm having problems with the Ben Eater code.
#The idea is to write the EEPROM data to a bin file and program the EEPROM using a universal
#EEPROM programmer that I bought off Amazon (the same one Ben Eater uses in his 6502 series

import numpy as np

size = 2048

HLT =0b1000000000000000
MI = 0b0100000000000000
RI = 0b0010000000000000
RO = 0b0001000000000000
IO = 0b0000100000000000
II = 0b0000010000000000
AI = 0b0000001000000000
AO = 0b0000000100000000
EO = 0b0000000010000000
SU = 0b0000000001000000
BI = 0b0000000000100000
OI = 0b0000000000010000
CE = 0b0000000000001000
CO = 0b0000000000000100
J =  0b0000000000000010
FI = 0b0000000000000001

dataTemplate = np.array([   MI|CO, RO|II|CE, 0,     0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, IO|MI, RO|AI, 0,              0, 0, 0,\
                            MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI|FI,       0, 0, 0,\
                            MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI|SU|FI,    0, 0, 0,\
                            MI|CO, RO|II|CE, IO|MI, AO|RI, 0,              0, 0, 0,\
                            MI|CO, RO|II|CE, IO|AI, 0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, IO|J,  0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, 0,     0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, 0,     0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, 0,     0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, 0,     0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, 0,     0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, 0,     0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, 0,     0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, AO|OI, 0,     0,              0, 0, 0,\
                            MI|CO, RO|II|CE, HLT,   0,     0,              0, 0, 0], dtype = 'uint16')

data = np.zeros(size, dtype='uint8')
for i in range(len(dataTemplate)):
    #flags ZF = 0 CF = 0
        data[i]=dataTemplate[i] >> 8
        data[i+128]=dataTemplate[i]
    #flags ZF = 0 CF = 1
        data[i+256]=dataTemplate[i] >> 8
        data[i+256+128]=dataTemplate[i]
    #flags ZF = 1 CF = 0
        data[i+512]=dataTemplate[i] >> 8
        data[i+512+128]=dataTemplate[i]
    #flags ZF = 1 CF = 1
        data[i+768]=dataTemplate[i] >> 8
        data[i+768+128]=dataTemplate[i]
print(data)

f=open("binfile.bin","wb")
f.write(data)
f.close()
