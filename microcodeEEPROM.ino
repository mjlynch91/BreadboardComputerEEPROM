#define DATA 2
#define CLK 3
#define LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

void setAddress(int address, bool outputEnable) {
  shiftOut(DATA, CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80)); //puts the first three bits of our 11-bit address into the upper register
  shiftOut(DATA, CLK, MSBFIRST, address);
  
  digitalWrite(LATCH, LOW);
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
}

byte readEEPROM(int address){
  setAddress(address, /*outputEnable*/ true);
  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1){
    pinMode(pin, INPUT); //input on the Arduino would mean output on the EEPROM
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}

void writeEEPROM(int address, byte data){
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++){
    pinMode(pin, OUTPUT); //output on the Arduino would mean input on the EEPROM
    digitalWrite(pin, data & 1); //ANDing gets only the LSB
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10); //give the EEPROM enough time to write
}

void printContents(){
  for (int base = 0; base <= 255; base += 16){
    byte data[16];
    for (int offset = 0; offset <=15; offset += 1){
      data[offset] = readEEPROM(base + offset);
    };
    char buf[80];
    sprintf(buf, "%03x: %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x", base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8],\
    data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
    Serial.println(buf);
  }
}

#define HLT 0b1000000000000000
#define MI  0b0100000000000000
#define RI  0b0010000000000000
#define RO  0b0001000000000000
#define IO  0b0000100000000000
#define II  0b0000010000000000
#define AI  0b0000001000000000
#define AO  0b0000000100000000
#define EO  0b0000000010000000
#define SU  0b0000000001000000
#define BI  0b0000000000100000
#define OI  0b0000000000010000
#define CE  0b0000000000001000
#define CO  0b0000000000000100
#define J   0b0000000000000010

uint16_t data[] = {
  MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0,  // 0000 : NOP - No Operation
  MI|CO, RO|II|CE, IO|MI, RO|AI, 0,           0, 0, 0,  // 0001 : LDA - Load a value from RAM into A register
  MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI,       0, 0, 0,  // 0010 : ADD - Add a value from RAM to whatever is in A register and store the result in A register
  MI|CO, RO|II|CE, IO|MI, RO|BI, EO|AI|SU,    0, 0, 0,  // 0011 : SUB - Subtract a value from RAM to whatever is in A register and store the result in A register
  MI|CO, RO|II|CE, IO|MI, AO|RI, 0,           0, 0, 0,  // 0100 : STA - Store whatever is in the A register into RAM
  MI|CO, RO|II|CE, IO|AI, 0,     0,           0, 0, 0,  // 0101 : LDI - Load an immediate value (value not stored in RAM, up to 4 bits) into A register
  MI|CO, RO|II|CE, IO|J,  0,     0,           0, 0, 0,  // 0110 : JMP - Jump to memory address
  MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0,  // 0111
  MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0,  // 1000
  MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0,  // 1001
  MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0,  // 1010
  MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0,  // 1011
  MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0,  // 1100
  MI|CO, RO|II|CE, 0,     0,     0,           0, 0, 0,  // 1101
  MI|CO, RO|II|CE, AO|OI, 0,     0,           0, 0, 0,  // 1110 : OUT
  MI|CO, RO|II|CE, HLT,   0,     0,           0, 0, 0   // 1111 : HLT
  };

void setup() {
  // put your setup code here, to run once:
  pinMode(DATA, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(LATCH, OUTPUT);

  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  
  Serial.begin(57600);

  //Erase EEPROM
  Serial.print("Erasing EEPROM");
  for(int address = 0; address <= 2047; address ++){
    writeEEPROM(address, 0xff);
    if(address % 64 == 0){
      Serial.print(".");
    }
  }
  Serial.println("done erasing");
  
  //Program the EEPROM with the contents of data array
  Serial.print("Programming EEPROM");
  for(int address = 0; address < sizeof(data)/sizeof(data[0]); address++){    //for the left EEPROM
    writeEEPROM(address, data[address] >> 8);
    if(address % 64 == 0){
      Serial.print(".");
      }
    }
  for(int address = 0; address < sizeof(data)/sizeof(data[0]); address++){    //for the right EEPROM
  writeEEPROM(address + 128, data[address]);
    if(address % 64 == 0){
      Serial.print(".");
    }
  }
  
  Serial.println(" done.");
  printContents();

  Serial.println("Now safe to eject");
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
