void stopWakeUp ()
{
 String a = reciveMessage();
 if(a!="") Serial.println(a);
 if ((a != "") && a==String(randNumber))
 {
  radio.setVolume(0);
  refreshDisplay = true;
  randNumber = 0;
  menu = 0;
 }
 
}

void clockManage()
{
 int a = swLongOrShort();  
 if(a == 2)                 // se longTouch su swMain allora entro nel menu principale
 {
  refreshDisplay = true;   // aggiorno il display
  display.clearDisplay();
  menu = 1;              // passo al menu 1
 }
 
}

void mainMenuManage()
{

  int a = swLongOrShort();
  int b = rotary();
  if(a == 1)Serial.print("short");
  if(a == 2)Serial.print("Long");

  if(b == 1)
  {
    index++;
    if(index == 3) index = 0;  
  }

  if(b == -1)
  {
    index--;
    if(index == -1) index = 2;  
  }
  if(a == 1)
  {
    if(index == 0)
    {
      index = 0;
      refreshDisplay = true;
      radio.setVolume(volume);
      menu = 3;
      display.clearDisplay(); 
    }
    if(index == 1)
    {
      index = 0;
      menu = 2;
      refreshDisplay = true;
      display.clearDisplay();
    }
    if(index == 2)
    {
      index = 0;
      refreshDisplay = true;
      menu = 4;
      display.clearDisplay();
    }
  }
  if (a == 2)
  { 
    index = 0;  
    refreshDisplay = true;
    analogWrite(blPin, 0);
    menu = 0;
  }
}

void snoozeManage()
{
  int a = swLongOrShort();
  int b = rotary();
  
  if(b == 1)
  {
    index++;
    if(index == 4) index = 0;  
  }

  if(b == -1)
  {
    index--;
    if(index == -1) index = 3;  
  }
  
  if(a == 1)
  {
    dataModify();
    }
  
  if (a == 2)
  { 
    index = 0;  
    refreshDisplay = true;
    analogWrite(blPin, 0);
    menu = 0;
  }
}

void radioManage() // metodo usato nel menù radio, controlla lo stato dell'encoder ed esegue le modifiche
{
  int a = rotary();
  int b = swLongOrShort();
  
  if (b == 2 && (millis()-timeToRot) > 3000) // se è longpress e non ho girato l'encoder
  {
    refreshDisplay = true; // devo aggiornare il display
    menu = 0;             // torno al menu 0
    radio.setVolume(0);  // spengo la radio
  }
  if (switchTime() > 200)   // se è un shortPress (NB ho una funz dedicata ma l'avevo creata prima di farla, da ricontrollare)
  {
    if (a == 1)                // se ho girato a destra
    {
      volume++;
      if (volume == 16) volume = 15;
      radio.setVolume(volume);
    }
    if (a == -1)              // se ho girato a sinistra
    {
      volume--;
      if (volume == -1) volume = 0;
      radio.setVolume(volume);
    }
  }
  else
  {
    if (a == 1) radio.seekUp();      // se invece non sto premendo lo swMain e sto girando l'encoder, allora 
    if (a == -1) radio.seekDown();  // sinistra stazione giu, destra, stazione sopra
  }
}

void dateHourManage()
{
  int a = swLongOrShort();
  int b = rotary();
  
  if(b == 1)
  {
    index++;
    if(index == 6) index = 0;  
  }

  if(b == -1)
  {
    index--;
    if(index == -1) index = 5;  
  }
  
  if(a == 1)
  {
    if(index == 0)
    {
      
    }
    if(index == 1)
    {
     
    }
    if(index == 2)
    {

    }
    if(index == 3)
    {

    }
    if(index == 4)
    {

    }
    if(index == 5)
    {

    }
  }
  if (a == 2)
  { 
    index = 0;  
    refreshDisplay = true;
    analogWrite(blPin, 0);
    menu = 0;
  }
}

void dataModify()
{
 int a = swLongOrShort();
 if(a == 1) return;
  }

