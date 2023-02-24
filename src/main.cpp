#include <Arduino.h>
#include <LiquidCrystal.h>

// DIP_SWITCH_PIN
#define DIP_SW1 A5
#define DIP_SW2 A4
#define DIP_SW3 A3
#define DIP_SW4 A2

// LED_PIN
#define LED_PS A1
#define LED_RS A0
#define LED_TM 11
#define LED_AL 10
#define LED_LM 9

const int rs = 4, en = 2, d4 = 8, d5 = 7, d6 = 6, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

long timer = 0;

void setup()
{
  lcd.begin(16, 2);

  pinMode(DIP_SW1, INPUT_PULLUP);
  pinMode(DIP_SW2, INPUT_PULLUP);
  pinMode(DIP_SW3, INPUT_PULLUP);
  pinMode(DIP_SW4, INPUT_PULLUP);
  pinMode(LED_PS, INPUT);
  pinMode(LED_RS, INPUT);
  pinMode(LED_TM, OUTPUT);
  pinMode(LED_AL, OUTPUT);
  pinMode(LED_LM, OUTPUT);

  Serial.begin(9600);
}

void ledControl()
{
  digitalWrite(LED_TM, HIGH);
  digitalWrite(LED_AL, HIGH);
  digitalWrite(LED_LM, HIGH);
  Serial.println("HIGH");
  delay(1000);
  digitalWrite(LED_TM, LOW);
  digitalWrite(LED_AL, LOW);
  digitalWrite(LED_LM, LOW);
  Serial.println("LOW");
  delay(1000);
}

void readTimerInput()
{
  Serial.print("1: ");
  Serial.print(analogRead(DIP_SW1));
  Serial.print(" 2: ");
  Serial.print(analogRead(DIP_SW2));
  Serial.print(" 3: ");
  Serial.print(analogRead(DIP_SW3));
  Serial.print(" 4: ");
  Serial.print(analogRead(DIP_SW4));
  Serial.println();

  if (analogRead(DIP_SW1) == 0 && analogRead(DIP_SW2) != 0 && analogRead(DIP_SW3) != 0 && analogRead(DIP_SW4) != 0)
  {
    timer = 60000;
  }
  else if (analogRead(DIP_SW1) == 0 && analogRead(DIP_SW2) != 0 && analogRead(DIP_SW3) != 0 && analogRead(DIP_SW4) != 0)
  {
    timer = 180000;
  }
}

void lcdPrint()
{
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.setCursor(2, 0);

  lcd.setCursor(9, 0);
  lcd.print("PB:");
  lcd.setCursor(13, 0);
  lcd.print("+0-0");

  lcd.setCursor(0, 1);
  lcd.print("Bz:");
  lcd.setCursor(3, 1);
  lcd.print("0");

  lcd.setCursor(6, 1);
  lcd.print("Ld:");
  lcd.setCursor(9, 1);
  lcd.print("0");

  lcd.setCursor(12, 1);
  lcd.print("Rs:");
  lcd.setCursor(15, 1);
  lcd.print("0");
}

void loop()
{
  // ledControl();
  // readTimerInput();
  lcdPrint();
}