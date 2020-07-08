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

//byte digits[] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47};

void setup() {
  // put your setup code here, to run once:
  pinMode(DATA, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(LATCH, OUTPUT);

  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  
  Serial.begin(57600);

  //Program the EEPROM with the contents of data array
  byte digits[] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B}; //digits 0 thru 9 on the seven segment display

  //writes the ones places
  Serial.println("Programming ones place");
  for(int value = 0; value <= 255; value++){
    writeEEPROM(value, digits[(value % 10)]);
  }

  //writes the tens place
  Serial.println("Programming tens place");
  for(int value = 0; value <= 255; value++){
    writeEEPROM(value + 256, digits[((value/10)%10)]);
  }

  //writes the hundreds place
  Serial.println("Programming hundreds place");
  for(int value = 0; value <= 255; value++){
    writeEEPROM(value + 512, digits[((value/100)%10)]);
  }

  //writes a blank
  Serial.println("Programming zeros");
  for(int value = 0; value <= 255; value++){
    writeEEPROM(value + 768, 0);
  }

  Serial.println("Programming ones place (twos complement)");
  for(int value = -128; value <= 127; value++){
    writeEEPROM((byte)value + 1024, digits[abs(value) % 10]);
  }

  Serial.println("Programming tens place (twos complement)");
  for(int value = -128; value <=127; value++){
    writeEEPROM((byte)value + 1280, digits[abs(value/10) % 10]);
  }

  Serial.println("Programming hundreds place (twos complement)");
  for(int value = -128; value <=127; value++){
    writeEEPROM((byte)value + 1536, digits[abs(value/100) % 10]);
  }

  //write a negative sign or nothing for positive
  Serial.println("Programming sign (twos complement)");
  for(int value = -128; value <=127; value++){
    if(value < 0){
      writeEEPROM((byte)value + 1792, 0x01);
    } else {
      writeEEPROM((byte)value + 1792, 0);
    }
  }
  printContents();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
