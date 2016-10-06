void writeInt (unsigned int intero, unsigned int INTERVAL_ADDR)
{  // la chiamata accetta il valore long e l'indirizzo        
  // in cui salvare                         
  EEPROM.write(INTERVAL_ADDR, highByte(intero));
  EEPROM.write(INTERVAL_ADDR+1, lowByte(intero));
}

unsigned int readInt(unsigned int INTERVAL_ADDR)     // ammette l'indirizzo in cui
{                                                    // leggere il valore
  byte hiByte = EEPROM.read(INTERVAL_ADDR);
  byte loByte = EEPROM.read(INTERVAL_ADDR+1); 
  unsigned int intero = word(hiByte,loByte);
  return intero;  // e la ritorna...
}
