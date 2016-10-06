
void rtcInit()
{
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  rtc.writeSqwPinMode(SquareWave1HZ);


}
void clockRTC() {
  
  now = rtc.now();
  if (minOld != now.minute())
  {
    minOld = now.minute();
    changeMinute = true;
    if(now.hour()== 6 && now.minute()== 0 && menu !=-1 )
    {
      index = 0;
      refreshDisplay = true;
      radio.setChannel(readInt(8));
      radio.setVolume(readInt(6));
      menu = -1;
    }
  }
  
  ora = "";
  if (now.hour() < 10)ora = ora + "0";
  ora = ora + String(now.hour()) + " ";
  if (now.minute() < 10)ora = ora + "0" ;
  ora = ora + String(now.minute());

  data = "";
  strcpy_P (buffer, (char *) pgm_read_word (&mesiDiUnAnno[now.month()-1]));
  data = (data + String(now.day()) + " " + buffer + " " + now.year());

  
}


unsigned int conteggio()
{
  puntino = !puntino;
}

