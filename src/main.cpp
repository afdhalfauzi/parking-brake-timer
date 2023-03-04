#include <Arduino.h>
#include <LiquidCrystal.h>

// DIP_SWITCH_PIN
#define DIP_SW1 A2
#define DIP_SW2 A3
#define DIP_SW3 A4
#define DIP_SW4 A5

// IO_PIN
#define BRAKE_PIN A1
#define RESET_PIN A0
#define TIMER_PIN 11
#define ALARM_PIN 10
#define LAMP_PIN 9

const int rs = 4, rw = 3, en = 2, d4 = 8, d5 = 7, d6 = 6, d7 = 5;
LiquidCrystal lcd(rs, rw, en, d4, d5, d6, d7);

int buzzer_state;
int lamp_state;
int reset_state;

unsigned long current_millis = 0;
unsigned long previous_millis = 0;
unsigned long reset_btn_press_time = 0;
unsigned long blinking_time = 0;
long timer = 60;
int timer_number = 1;
bool reset_btn_pressed = false;

bool isBrake();
bool isReset();

void setup()
{
  lcd.begin(16, 2);

  pinMode(DIP_SW1, INPUT);
  pinMode(DIP_SW2, INPUT);
  pinMode(DIP_SW3, INPUT);
  pinMode(DIP_SW4, INPUT);
  pinMode(BRAKE_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(TIMER_PIN, OUTPUT);
  pinMode(ALARM_PIN, OUTPUT);
  pinMode(LAMP_PIN, OUTPUT);
}

void lcdPrint()
{
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.setCursor(2, 0);
  lcd.print(String(timer) + ":" + String(timer_number) + " ");

  lcd.setCursor(9, 0);
  lcd.print("Brake=");
  lcd.setCursor(15, 0);
  lcd.print(isBrake());

  lcd.setCursor(0, 1);
  lcd.print("Bz=");
  lcd.setCursor(3, 1);
  lcd.print(String(buzzer_state));

  lcd.setCursor(6, 1);
  lcd.print("Ld=");
  lcd.setCursor(9, 1);
  lcd.print(String(lamp_state));

  lcd.setCursor(12, 1);
  lcd.print("Rs=");
  lcd.setCursor(15, 1);
  lcd.print(isReset());
}

// void lampBlinking()
// {
//   lamp_state = 1;
//   lcdPrint();
//   for (int i = 0; i < 10; i++) // for 10sec
//   {
//     digitalWrite(LAMP_PIN, HIGH);
//     digitalWrite(TIMER_PIN, LOW);
//     delay(500);
//     digitalWrite(LAMP_PIN, LOW);
//     digitalWrite(TIMER_PIN, HIGH);
//     delay(500);
//   }
// }

void alarmBlinking()
{
  lamp_state = 1;
  lcdPrint();

  current_millis = millis();
  if (current_millis - previous_millis > 500 * 0.75) // blinking every 1sec
  {
    previous_millis = current_millis;
    if (millis() - blinking_time > 10000 * 0.75) // alarm blinking after 10sec
    {
      buzzer_state = 1;
      digitalWrite(ALARM_PIN, !digitalRead(ALARM_PIN));
    }
    digitalWrite(LAMP_PIN, !digitalRead(LAMP_PIN));
    digitalWrite(TIMER_PIN, !digitalRead(TIMER_PIN));
  }
}

bool isBrake()
{
  if (digitalRead(BRAKE_PIN) == 0)
    return 1;
  else
    return 0;
}

bool isReset()
{
  if (digitalRead(RESET_PIN) == 0)
    return 1;
  else
    return 0;
}

void readTimerInput()
{
  if (analogRead(DIP_SW4) == 0)
  {
    timer = 420;
    timer_number = 7;
  }
  else if (analogRead(DIP_SW3) == 0)
  {
    timer = 300;
    timer_number = 5;
  }
  else if (analogRead(DIP_SW2) == 0)
  {
    timer = 180;
    timer_number = 3;
  }
  else
  {
    timer = 60;
    timer_number = 1;
  }
}

void loop()
{
  readTimerInput();

  while (isBrake() && !isReset()) // true if brake is active
  {
    current_millis = millis();
    if (current_millis - previous_millis > 1000 * 0.75) // 1 second
    {
      previous_millis = current_millis;
      timer--;
    }
    if (timer <= 0)
    {
      blinking_time = millis();
      while (!isReset())
      {
        alarmBlinking();
      }
    }
    lcdPrint();
  }

  if (isReset()) // true if reset button is pressed
  {
    if (!reset_btn_pressed)
    {
      buzzer_state = 0;
      lamp_state = 0;
      digitalWrite(ALARM_PIN, LOW);
      digitalWrite(LAMP_PIN, LOW);
      digitalWrite(TIMER_PIN, LOW);
      reset_btn_pressed = true;
      reset_btn_press_time = millis();
    }
    if ((millis() - reset_btn_press_time) >= 5000 * 0.75) // reset button pressed > 5sec
    {
      reset_state = 0;
      while (reset_state == 0 || reset_state == 1)
      {
        if (!isReset())
        {
          reset_state = 1;
        }
        if (isReset() && reset_state == 1)
        {
          reset_state = 2;
          reset_btn_pressed = false;
        }
        alarmBlinking();
      }
    }
  }
  else
  {
    if (reset_btn_pressed)
    {
      buzzer_state = 0;
      lamp_state = 0;
      digitalWrite(ALARM_PIN, LOW);
      digitalWrite(LAMP_PIN, LOW);
      digitalWrite(TIMER_PIN, LOW);
    }
    reset_btn_pressed = false;
  }

  lcdPrint();
}