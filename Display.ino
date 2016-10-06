
void displayInit()
{
  pinMode(blPin, OUTPUT); // pin per la retroilluminazione
  display.begin();
  display.setContrast(45);
  display.clearDisplay();
  display.display();
}

void backLight()
{
  if (bkLight)
  {
    analogWrite(blPin, 255);
    if ((millis() - timeToPush) > 10000)
    {
      bkLight = false;
      analogWrite(blPin, 10);
    }
  }
}

void writeWord(int color, int dim, int x, int y, float text )  // colore - dimTesto - coordinate - testo
{
  display.setTextSize(dim);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.print(text, 1);
  display.display();
}
void writeWord(int color, int dim, int x, int y, int16_t text )  // colore - dimTesto - coordinate - testo
{
  display.setTextSize(dim);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.print(text);
  display.display();
}

void writeWord(int color, int dim, int x, int y, String text )  // colore - dimTesto - coordinate - testo
{
  display.setTextSize(dim);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.print(text);
  // display.drawPixel(42, 24, BLACK); // pixel centrale
  display.display();
}


