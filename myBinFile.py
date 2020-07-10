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

dataZ0C0 = np.copy(dataTemplate)
dataZ0C1 = np.copy(dataTemplate)
dataZ0C1[58] = IO|J
dataZ1C0 = np.copy(dataTemplate)
dataZ1C0[66] = IO|J
dataZ1C1 = np.copy(dataTemplate)
dataZ1C1[58] = IO|J
dataZ1C1[66] = IO|J


data_8bit = np.zeros(size, dtype='uint8')
for i in range(0,127):
    data_8bit[i] = dataZ0C0[i] >> 8
    data_8bit[i+128] = dataZ0C0[i]

    data_8bit[i+256] = dataZ0C1[i] >> 8
    data_8bit[i+256+128] = dataZ0C1[i]

    data_8bit[i+512] = dataZ1C0[i] >> 8
    data_8bit[i+512+128] = dataZ1C0[i]

    data_8bit[i+768] = dataZ1C1[i] >> 8
    data_8bit[i+768+128] = dataZ1C1[i]
                
print(data_8bit)

f=open("binfile.bin","wb")
f.write(data_8bit)
f.close()
