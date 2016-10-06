void espInit()
{
  delay(1000);
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWJAP_DEF=\"ReteCasa\",\"\"\r\n",4000,DEBUG); // configure as access point
  sendData("AT+CWMODE=3\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,9090\r\n",1000,DEBUG); // turn on server on port 80
  
}

String reciveMessage()
{
  String message = "";
  while (esp8266.available())            // se arriva un carattere
  {
    char ch = esp8266.read();               // salvo il carattere
    message = message + String(ch);   // e lo aggiungo alla stringa (convertendolo in stringa)
  }
  int startIndex = message.indexOf(":");    // trovo l'inizio del messaggio
  int finishIndex = message.lastIndexOf("\n", message.length() - 2);   // trovo il penultimo newline
  if (message != "" && startIndex != -1)  // se la stringa non è vuota e contiene il messaggio
  {
    return message.substring(startIndex + 1, finishIndex); // ricavo il messaggio dalla stringa
  }
  else return ""; 
}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {      
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }    
    return response;
}
