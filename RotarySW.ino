int rotary()
{
  int encoder = 0;
  if (!(digitalRead(rotaryUp) == LOW && digitalRead(rotaryDown) == LOW)) // solo se non sono entrambi a zero
  {
    if (digitalRead(rotaryUp) == LOW)                          // controllo se è andato a zero
    {
      timeToRot = millis();
      if (startRotaryUp == 0) startRotaryUp = millis();        // se non avevo già cominciato a tenere il tempo
    }
    if (digitalRead(rotaryDown) == LOW)                      // controllo se è andato a zero
    {
      timeToRot = millis();
      if (startRotaryDown == 0) startRotaryDown = millis();  // se non avevo già cominciato a tenere il tempo
    }

    if (digitalRead(rotaryUp) == HIGH && startRotaryUp != 0 && durationUp == 0) durationUp = millis() - startRotaryUp;           // se ho registrato uno start, e l'uscita torna alta, e per la prima volta -> tempo finale
    if (digitalRead(rotaryDown) == HIGH && startRotaryDown != 0 && durationUp == 0) durationDown = millis() - startRotaryDown;  // se ho registrato uno start, e l'uscita torna alta e per la prima volta -> tempo finale
  }



  if ((durationUp <= 20 && durationUp != 0) || (durationDown <= 20 && durationDown != 0)) // se la durata di uno dei due impulsi è minore di 30 millis
  {                                                                                      // allora scarto la misura
    startRotaryUp = 0; 
    startRotaryDown = 0;
    durationUp = 0;
    durationDown = 0;
  }

  if((durationUp >= 20 && durationUp != 0) || (durationDown >= 20 && durationDown != 0)) 
  {
    if(startRotaryUp < startRotaryDown)  // se rotaryUp è switchata prima di rotaryDown -> movimento a destra
    {
     encoder = 1;
    }
    if(startRotaryUp > startRotaryDown) // se rotaryDown è switchata prima di rotaryUp -> movimento a sinistra
    {
     encoder = -1;
    }
    isTouch = true;           // l'encoder è stato toccato
    refreshDisplay = true;   // il display è da refreshare
    startRotaryUp = 0;      //  azzero le variabili per una nuova misura 
    startRotaryDown = 0;
    durationUp = 0;
    durationDown = 0;  
  }
  
  return encoder;
}


unsigned long switchTime() // ritorna il tempo in cui il pulsante rimane premuto
{
  if (digitalRead(rotaryPush) == HIGH && pushState == LOW)
  {
    delay(2);
    if (digitalRead(rotaryPush) == HIGH && pushState == LOW)
    {
      pushState = !pushState;
      startPushMain = millis();

    }
  }
  if (digitalRead(rotaryPush) == LOW && pushState == HIGH)
  {
    delay(2);
    if (digitalRead(rotaryPush) == LOW && pushState == HIGH)
    {
      isTouch = true;            // allora per forza è stato toccato
      refreshDisplay = true;
      bkLight = true;
      pushState = !pushState;
      startPushMain = millis();
      timeToPush = millis();    // e aggiorno il momento in cui l'ho toccato
    }
  }

  if (pushState == LOW)
  {
    return (millis() - startPushMain);
  }
  else return 0;
}


int swLongOrShort()
{
  switchTime();
  if ((millis() - timeToPush < 500) && pushState == HIGH && isTouch == true)
  {
    isTouch = false;
    return 1;
  }
  if (switchTime() > 3000 && ((millis() - timeToPush) > 3000) && isTouch == true)
  {
    isTouch = false;
    return 2;
  }
  else return 0;
}


