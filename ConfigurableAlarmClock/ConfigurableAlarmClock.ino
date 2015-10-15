#include <LiquidCrystal.h>
#include <Wire.h>
#include <IRremote.h>
#include "RTClib.h"

int RECV_PIN = 8;
int PIN_ALARM = 9;
int PIN_ALARM_STOP = 10;
int editIndex = 0;
bool editMode = false;
bool blinker = false;
String line1 = String("");
String line2 = String("");
char alarm[5] = {'-','-',':','-','-'};
int alarmInt[5];
String alarmHour = String("--");
String alarmMinute = String("--");
int alarmH;
int alarmM;
bool alarming = false;
int tasterstatus=0;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
RTC_DS1307 RTC;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(57600);
  pinMode (13, OUTPUT);
  pinMode (PIN_ALARM, OUTPUT);
  pinMode (PIN_ALARM_STOP, INPUT);
  lcd.begin(16, 2);
  Wire.begin();
  RTC.begin();
  irrecv.enableIRIn();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  tasterstatus=digitalRead(PIN_ALARM_STOP);
  if(alarming && tasterstatus == HIGH)
  {
    digitalWrite(PIN_ALARM, LOW);
    alarming = false;
  }
  
  if(editMode)
  {
    blinker = !blinker;
  }
  
  DateTime now = RTC.now();
  
  if(!alarming && alarmH == now.hour() && alarmM == now.minute() && now.second() <= 2)
  {
    digitalWrite(PIN_ALARM, HIGH);
    alarming = true;
  }
  
  line1 = String("");
  line2 = String("                ");
  
  line1 += "Time: ";
  line1 += now.hour();
  line1 += ":";
  line1 += now.minute();
  line1 += ":";
  line1 += now.second();

  if(!blinker)
  {
    line2 = String("");
    line2 += "Alarm: ";
    line2 += alarm[0];
    line2 += alarm[1];
    line2 += alarm[2];
    line2 += alarm[3];
    line2 += alarm[4];
  }

  
  
  if (irrecv.decode(&results))
  {
    if (results.value == 16748655)
    {
      if(editMode)
      {
        editMode = false;
        blinker = false;
        if(editIndex != 5)
        {
          alarm[0] = '-';
          alarm[1] = '-';
          alarm[3] = '-';
          alarm[4] = '-';
          alarmH = -1;
          alarmM = -1;
          alarmInt[0] = -1;
          alarmInt[1] = -1;
          alarmInt[3] = -1;
          alarmInt[4] = -1;
        }
        else
        {
          alarmH = alarmInt[0] * 10 + alarmInt[1];
          alarmM = alarmInt[3] * 10 + alarmInt[4];
        }
        editIndex = 0;
      }
      else
      {
        editMode = true;
      }
    }
    
    if(editMode && results.value == 16738455) //0
    {
      alarm[editIndex] ='0';
      alarmInt[editIndex] =0;
      editIndex++;

      if(editIndex == 2)
      {
        editIndex++;
      }
    }

    if(editMode && results.value == 16724175) //1
    {
      alarm[editIndex] ='1';
      alarmInt[editIndex] =1;
      editIndex++;

      if(editIndex == 2)
      {
        editIndex++;
      }
    }

    if(editMode && results.value == 16718055) //2
    {
      alarm[editIndex] ='2';
      alarmInt[editIndex] =2;
      editIndex++;

      if(editIndex == 2)
      {
        editIndex++;
      }
    }

    if(editMode && results.value == 16743045 && editIndex != 0) //3
    {
      alarm[editIndex] ='3';
      alarmInt[editIndex] =3;
      editIndex++;

      if(editIndex == 2)
      {
        editIndex++;
      }
    }

    if(editMode && results.value == 16716015 && editIndex != 0) //4
    {
      alarm[editIndex] ='4';
      alarmInt[editIndex] =5;
      editIndex++;

      if(editIndex == 2)
      {
        editIndex++;
      }
    }

    if(editMode && results.value == 16726215 && editIndex != 0) //5
    {
      alarm[editIndex] ='5';
      alarmInt[editIndex] =5;
      editIndex++;

      if(editIndex == 2)
      {
        editIndex++;
      }
    }

    if(editMode && results.value == 16734885 && editIndex != 0) //6
    {
      alarm[editIndex] ='6';
      alarmInt[editIndex] =6;
      editIndex++;

      if(editIndex == 2)
      {
        editIndex++;
      }
    }

    if(editMode && results.value == 16728765 && editIndex != 0) //7
    {
      alarm[editIndex] ='7';
      alarmInt[editIndex] =7;
      editIndex++;

      if(editIndex == 2)
      {
        editIndex++;
      }
    }

    if(editMode && results.value == 16730805 && editIndex != 0) //8
    {
      alarm[editIndex] ='8';
      alarmInt[editIndex] =8;
      editIndex++;

      if(editIndex == 2)
      {
        editIndex++;
      }
    }

    if(editMode && results.value == 16732845 && editIndex != 0) //9
    {
      alarm[editIndex] ='9';
      alarmInt[editIndex] =9;
      editIndex++;

      if(editIndex == 2)
      {
        editIndex++;
      }
    }
    
    irrecv.resume();
  }
  
  printDiplay();
  delay(500);
}

void printDiplay()
{
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

