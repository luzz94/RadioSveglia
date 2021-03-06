#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
#include <Si4703_Breakout.h>
#include <SoftwareSerial.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <RTClib.h>
#include <avr/pgmspace.h>

#define blPin 9
#define clockPin 2
#define rotaryPush 3
#define rotaryUp 4
#define rotaryDown 5
#define DEBUG false

DateTime now ; // label per l'rtc
RTC_DS1307 rtc; // label per l'rtc
Si4703_Breakout radio(12, A4, A5); // vengono impostati i pin per la radio
SoftwareSerial esp8266(A1, A0);

float channel = 950; // variabile per la frequenza radio

int randNumber = 0;
int volume = 5;
int menu = 0;
int index = 0;
int lastMovement = 0;
int longOrShort = 0;
int minOld = 0;
char buffer[10];

unsigned long timeToPush = 0;      // variabile per tenere conto del tempo tra una premuta e un altra
unsigned long timeToRot = 0;       // variabile per tenere conto del tempo tra una rotazione e un altra
unsigned long startRotaryUp = 0;
unsigned long startRotaryDown = 0;
unsigned int durationUp = 0;       // usate nel rotary, durata dell'impulso
unsigned int durationDown = 0;
unsigned long startPushMain = 0;
String ora = "";
String data = "";
String message = "";

boolean puntino = true;           // per il lampeggio del puntino dell'orologio
boolean isTouch = true;           // tiene conto se il rotarySW è stato toccato - per sapere se pressione lunga/corta
boolean refreshDisplay = true;    // tiene conto se il rotarySW è stato toccato - per aggiornare il display
boolean pushState = false;        // tiene conto se lo switch del rotary è stato premuto
boolean pushStateUp = false;
boolean pushStateDown = false;
boolean bkLight = true;
boolean changeMinute = true;

/**
  * Aggiugi delle variabili per l'ora della sveglia (che non mi sembrano esserci),
  * la frequenza che vuoi come radio, e il volume. Salvale nella "PROGMEM"
  */


const PROGMEM char day_1[]  = "Domenica";
const PROGMEM char day_2[]  = "Lunedi";
const PROGMEM char day_3[]  = "Martedi";
const PROGMEM char day_4[]  = "Mercoledi";
const PROGMEM char day_5[]  = "Giovedi";
const PROGMEM char day_6[]  = "Venerdi";
const PROGMEM char day_7[]  = "Sabato";

const char* const giorniDellaSettimana[7] PROGMEM = {day_1, day_2, day_3, day_4, day_5, day_6, day_7};

const PROGMEM char month_1[]  = "Gen";
const PROGMEM char month_2[]  = "Feb";
const PROGMEM char month_3[]  = "Mar";
const PROGMEM char month_4[]  = "Apr";
const PROGMEM char month_5[]  = "Mag";
const PROGMEM char month_6[]  = "Giu";
const PROGMEM char month_7[]  = "Lug";
const PROGMEM char month_8[]  = "Ago";
const PROGMEM char month_9[]  = "Set";
const PROGMEM char month_10[]  = "Ott";
const PROGMEM char month_11[]  = "Nov";
const PROGMEM char month_12[]  = "Dic";

const char* const mesiDiUnAnno[12] PROGMEM = {month_1, month_2, month_3, month_4, month_5, month_6, month_7, month_8, month_9, month_10, month_11, month_12};

const PROGMEM byte CursorMainMenu[] = {6, 20, 34};
const PROGMEM byte CursorSvegliaMenuX[] = {44, 62, 44, 44};
const PROGMEM byte CursorSvegliaMenuY[] = {15, 15, 29, 43};
const PROGMEM byte CursorSvegliaLenght[] = {11, 11, 29, 11};

const PROGMEM byte CursorDataOraMenuX[] = {30, 48, 30, 54, 30, 54};
const PROGMEM byte CursorDataOraMenuY[] = {15, 15, 29, 29, 43, 43};
const PROGMEM byte CursorDataOraLenght[] = {11, 11, 17, 11, 17, 23};



Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 6, 8, 7);



void setup()
{
  //writeInt(6,2);
  //writeInt(0,4);
  //writeInt(10,6);
  //writeInt(950,8);

  pinMode(clockPin, INPUT); // pin di interrupt
  pinMode(rotaryPush, INPUT);
  digitalWrite(rotaryPush, HIGH);
  pinMode(rotaryUp, INPUT);
  digitalWrite(rotaryUp, HIGH);
  pinMode(rotaryDown, INPUT);
  digitalWrite(rotaryDown, HIGH);
  attachInterrupt(0 , conteggio, RISING); // viene attivato ogni secondo
  randomSeed(analogRead(A2));
  Serial.begin(9600);
  esp8266.begin(9600);
  radioInit();    // inizializzo la radio
  displayInit(); // inizializzo il display
  rtcInit();    // inizializzo il modulo RTC
  espInit();
}

void loop()
{
  while (menu == -1) // sveglia con visualizzazione numero di sblocco
  {
    clockRTC();
    analogWrite(blPin, 255);
    if(randNumber == 0)randNumber = random(1000,9999);
    if(refreshDisplay && menu == -1)
    {
    display.clearDisplay();
    writeWord(BLACK, 2, 19, 17, randNumber);
    display.display();
    refreshDisplay = false;
    }
    stopWakeUp();

  }

  while (menu == 0)  // orologio
  {

    clockRTC();   // aggiorno le variabili contenenti data e ora
    backLight(); //  attivo la retroilluminazione se è stato premuto swMain
    if ((refreshDisplay || changeMinute) && menu == 0) // se è da aggiornare il display o è passato un minuto
    {
      display.clearDisplay();
      writeWord(BLACK, 2, 13, 5, ora);
      strcpy_P (buffer, (char *) pgm_read_word (&giorniDellaSettimana[now.dayOfTheWeek()]));
      if (String(buffer).length() == 6) writeWord(BLACK, 1, 25, 24, String(buffer));   // in base alla lunghezza del giorno
      if (String(buffer).length() == 7) writeWord(BLACK, 1, 22, 24, String(buffer));  //  lo sposto in modo da tenerlo sempre in centro
      if (String(buffer).length() == 8) writeWord(BLACK, 1, 19, 24, String(buffer));  //  lo sposto in modo da tenerlo sempre in centro
      if (String(buffer).length() == 9) writeWord(BLACK, 1, 16, 24, String(buffer)); //   per qualsiasi giorno

      if (now.day() < 10) writeWord(BLACK, 1, 13, 37, data); // ogni mese ha 3 caratteri e l'anno 4, l'unica cosa che puo cambiare è il giorno
      if (now.day() >= 10) writeWord(BLACK, 1, 8, 37, data); //  a seconda che sia a 1 o 2 cifre, lo sposto sempre in centro
      display.display();
      refreshDisplay = false;
      changeMinute = false;

    }

    if (!puntino)display.fillRect(40, 8, 5, 10, WHITE); // cancello il puntino
    if (puntino) writeWord(BLACK, 2 , 37, 5, ":");    // scrivo il puntino, senza cancellare il resto dello schermo
    display.display(); // visualzza

    clockManage(); // richiamo il controllo dei tasti

  }


  while (menu == 1) // menu principale
  {
    clockRTC();
    analogWrite(blPin, 255);
    if (refreshDisplay && menu == 1)
    {
      writeWord(BLACK, 1, 18, 6, "Radio");
      writeWord(BLACK, 1, 18, 20, "Sveglia");
      writeWord(BLACK, 1, 18, 34, "Data/Ora");
      display.fillRect(7, pgm_read_byte(&CursorMainMenu[0]), 5, 7, WHITE);
      display.fillRect(7, pgm_read_byte(&CursorMainMenu[1]), 5, 7, WHITE);
      display.fillRect(7, pgm_read_byte(&CursorMainMenu[2]), 5, 7, WHITE);
      writeWord(BLACK, 1, 7, pgm_read_byte(&CursorMainMenu[index]), ">");
      display.display();
      refreshDisplay = false;
    }

    mainMenuManage();

  }

  while (menu == 2)  // Impostazione Sveglia
  {
    clockRTC();
    analogWrite(blPin, 255);
    if (refreshDisplay && menu == 2)
    {
      display.clearDisplay();
      writeWord(BLACK, 1, 0, 6, "Ora");
      writeWord(BLACK, 1, 0, 20, "FM");
      writeWord(BLACK, 1, 0, 34, "Volume");
      /**
        * Qui al posto della stringa "06:00" usi "oraSveglia.toString() + minutiSveglia.toString();"
        * con oraSveglia e minutiSveglia le variabili nella PROGMEM che indicano
        * l'ora e i minuti a cui la sveglia deve suonare.
        *
        * Stesso discorso lo fai per la frequenza della radio e il volume della sveglia
        */
      writeWord(BLACK, 1, 44, 6, "06:00");
      writeWord(BLACK, 1, 44, 20, "95.00");
      writeWord(BLACK, 1, 44, 34, "15");
      display.drawRect(pgm_read_byte(&CursorSvegliaMenuX[index]), pgm_read_byte(&CursorSvegliaMenuY[index]), pgm_read_byte(&CursorSvegliaLenght[index]), 0, BLACK);
      display.display();
      refreshDisplay = false;
    }

    snoozeManage();
  }

  while (menu == 3) // Radio FM
  {
    clockRTC();
    analogWrite(blPin, 255);
    if (refreshDisplay  && menu == 3)
    {
      display.clearDisplay();
      channel = radio.getChannel();
      if (channel >= 1000)writeWord(BLACK, 2, 12, 12, channel / 10);
      if (channel < 1000)writeWord(BLACK, 2, 19, 12, channel / 10);
      writeWord(BLACK, 1, 16, 32, ("volume " + String(volume)));
      refreshDisplay = false;
    }
    radioManage();
  }
  while (menu == 4) // Impostazioni Ora/data
  {
    clockRTC();
    analogWrite(blPin, 255);
    if (refreshDisplay && menu == 4)
    {
      display.clearDisplay();
      writeWord(BLACK, 1, 1, 6, "Ora");
      writeWord(BLACK, 1, 1, 20, "Gio");
      writeWord(BLACK, 1, 1, 34, "M/A");
      writeWord(BLACK, 1, 30, 6, "06:00");
      writeWord(BLACK, 1, 30, 20, "Lun 02");
      writeWord(BLACK, 1, 30, 34, "Mar 2016");
      display.drawRect(pgm_read_byte(&CursorDataOraMenuX[index]), pgm_read_byte(&CursorDataOraMenuY[index]), pgm_read_byte(&CursorDataOraLenght[index]), 0, BLACK);
      display.display();
      refreshDisplay = false;
    }
     dateHourManage();
  }

  while (menu < -1 or menu > 4)
  {
    analogWrite(blPin, 255);
    display.fillRect(0, 0, display.width(), display.height(), BLACK);
    display.display();
  }

}
