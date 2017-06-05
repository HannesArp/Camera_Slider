#include <TimerOne.h>
#include <EEPROM.h>

/////////_______LCD_______
#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd(9, 7, 6, 5, 4, 3);

/////////_______TASTER_______

#define T1 A0
#define T2 A1
#define T3 A2
#define T4 A4

/////////_______StepMotor_______
#define DIR 42
#define STEP 44
#define MS1 46
#define MS2 48
#define MS3 50
#define ENABLE 52
#define steps_per_mm 6
int speed_delay[]={1,3,5,7,9,11,13,15};

void fahr_mm(int mm, int speed_mm_sec, int richtung)
{
    digitalWrite(ENABLE, LOW);
    digitalWrite(DIR, richtung);
  for(int x=0;x<=(mm*steps_per_mm);x++)
  {
    digitalWrite(STEP, LOW);
    delay(speed_delay[speed_mm_sec]);
    digitalWrite(STEP, HIGH);
    delay(speed_delay[speed_mm_sec]);
  }
}

/////////_______AUSLÖSER_______
#define AUSLOSER1 14 
#define AUSLOSER2 15
void AUSLOSEN()
{
  
  digitalWrite(AUSLOSER1, HIGH);
  digitalWrite(AUSLOSER2, HIGH);
  delay(100);
  digitalWrite(AUSLOSER1, LOW);
  digitalWrite(AUSLOSER2, LOW);
}

/////////_______Variablen_______
int Anzahl_Bilder_addr = 1;
int Anzahl_Bilder2_addr = 2;
int Intervall_addr = 3;
int Verfahr_mm_addr = 4;
int Verfahr_Speed_addr = 5;
int Richtung_addr = 6;
int Ausloseverhalten_addr = 7;

int Anzahl_Bilder = 300;
int Intervall = 1;
int Verfahr_mm = 30;
int Verfahr_Speed=10; 
int Richtung = 0;
int Ausloseverhalten=0;


int enabel_menu=0;
int menuCunter =0;
int plus_minus =0;
int plus_minus_cunter=1;
int inervallcunter =0;

float verfahrzeit()
{
  float re= (Verfahr_mm*steps_per_mm)*speed_delay[Verfahr_Speed];
  re=re/1000;
  return re;
}

void save_settings()
{
  EEPROM.write(Anzahl_Bilder_addr, (Anzahl_Bilder/10));
  EEPROM.write(Intervall_addr,Intervall);
  EEPROM.write(Verfahr_mm_addr,Verfahr_mm);
  EEPROM.write(Verfahr_Speed_addr,Verfahr_Speed);
  EEPROM.write(Richtung_addr,Richtung);
  EEPROM.write(Ausloseverhalten_addr,Ausloseverhalten);
}
void read_settings()
{
    Anzahl_Bilder = EEPROM.read(Anzahl_Bilder_addr)*10;
    Intervall = EEPROM.read(Intervall_addr);;
    Verfahr_mm = EEPROM.read(Verfahr_mm_addr);;
    Verfahr_Speed=EEPROM.read(Verfahr_Speed_addr);; 
    Richtung = EEPROM.read(Richtung_addr);;
    Ausloseverhalten=EEPROM.read(Ausloseverhalten_addr);;
}


void start()
{
    inervallcunter=0;
    enabel_menu=1;
    lcd.setCursor(0,0);lcd.print("                ");
    lcd.setCursor(0,1);lcd.print("                ");
    Timer1.initialize(Intervall*1000000);
    Timer1.attachInterrupt(interrupt_timer);
}  

void stop_()
{
    enabel_menu=0;
    Timer1.stop();
}  

void interrupt_timer()
{
  if(inervallcunter<=Anzahl_Bilder)
  {
    fahr_mm(Verfahr_mm,Verfahr_Speed,Richtung);
    delay(10);
    AUSLOSEN();
    
    lcd.setCursor(0,0);lcd.print(">Slider Betrieb<");
    lcd.setCursor(3,1);lcd.print(" von ");
    lcd.setCursor(8,1);lcd.print(Anzahl_Bilder, DEC);
    lcd.setCursor(0,1);lcd.print(inervallcunter, DEC);
    inervallcunter++;
  }
  else
  {
    stop_();
    }
}

void setup() {
  // put your setup code here, to run once:
  /////////LCD-------
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(2, HIGH);
  digitalWrite(8, LOW);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("  CamSlider V1  ");
  lcd.setCursor(0,1);
  lcd.print("by hannes-arp.de");
  /////////LCD-------
  
  /////////Taster-------
  pinMode(T1, INPUT);
  pinMode(T2, INPUT);
  pinMode(T3, INPUT);
  pinMode(T4, INPUT);
  /////////Taster-------
  
  /////////Step Motor Treiber-------
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(DIR, LOW);
  /////////Step Motor Treiber-------

  
  /////////Auslöser-------
  pinMode(AUSLOSER1, OUTPUT);
  pinMode(AUSLOSER2, OUTPUT);
  /////////Auslöser-------

  
  /////////Clear Display-------
  delay(3000);
  lcd.setCursor(0,0);lcd.print("                ");
  lcd.setCursor(0,1);lcd.print("                ");
  read_settings();

}
void loop() {
  // put your main code here, to run repeatedly:
  
  while(digitalRead(T1) == 1)
  { 
    menuCunter++;
    if(menuCunter==8)menuCunter=0;
    while(digitalRead(T1) == 1);
    lcd.setCursor(0,1);lcd.print("                ");
  }
  
  while(digitalRead(T4) == 1)
  {
    menuCunter--;
    if(menuCunter==-1)menuCunter=7;
    while(digitalRead(T4) == 1);
    lcd.setCursor(0,1);lcd.print("                ");
  }

  if(digitalRead(T2) == 1)
  {
    plus_minus=1;
    plus_minus_cunter=1;
    delay(100);
    lcd.setCursor(0,1);lcd.print("                ");
  }
  if(digitalRead(T3) == 1)
  {
    plus_minus=1;
    plus_minus_cunter=-1;
    delay(100);
    lcd.setCursor(0,1);lcd.print("                ");
  }
  
  if(plus_minus==1)
  {
    switch(menuCunter)
    {
      case 0: if(plus_minus_cunter==1)start();
              if(plus_minus_cunter==-1)stop_();
              break;
      case 1: Anzahl_Bilder=Anzahl_Bilder+(plus_minus_cunter*10);
              if(Anzahl_Bilder==0)Anzahl_Bilder=1;
              break;
      case 2: Intervall=Intervall+plus_minus_cunter;
              if(Intervall==0)Intervall=1;
              break;
      case 3: Verfahr_mm=Verfahr_mm+plus_minus_cunter;
              if(Verfahr_mm==0)Verfahr_mm=1;
              break;
      case 4: Verfahr_Speed=Verfahr_Speed+plus_minus_cunter;
              if(Verfahr_Speed==-1)Verfahr_Speed=7;
              if(Verfahr_Speed==8)Verfahr_Speed=0;
              break;
      case 5: Richtung=Richtung+plus_minus_cunter;
              if(Richtung==-1)Verfahr_Speed=1;
              break;
      case 6: while(digitalRead(T2) == 1)
                {
                  digitalWrite(ENABLE, LOW);
                  digitalWrite(DIR, 0);
                  digitalWrite(STEP, LOW);
                  delay(speed_delay[Verfahr_Speed]);
                  digitalWrite(STEP, HIGH);
                  delay(speed_delay[Verfahr_Speed]);
                }
                while(digitalRead(T3) == 1)
                {
                  digitalWrite(ENABLE, LOW);
                  digitalWrite(DIR, 1);
                  digitalWrite(STEP, LOW);
                  delay(speed_delay[Verfahr_Speed]);
                  digitalWrite(STEP, HIGH);
                  delay(speed_delay[Verfahr_Speed]);
                }
              break;
      case 7: Ausloseverhalten=Ausloseverhalten+plus_minus_cunter;
              if(Ausloseverhalten==-1)Ausloseverhalten=2;
              break;
      default: break;
    }
    plus_minus=0;
  }

  if(enabel_menu==0)
  {
    switch(menuCunter)
    {
      case 0: lcd.setCursor(0,0);lcd.print("  Start   Stop  ");
              lcd.setCursor(0,1);lcd.print("A");
              lcd.setCursor(1,1);lcd.print(Anzahl_Bilder, DEC);
              lcd.setCursor(5,1);lcd.print("I");
              lcd.setCursor(6,1);lcd.print(Intervall, DEC);
              lcd.setCursor(8,1);lcd.print("R");
              lcd.setCursor(9,1);lcd.print(Richtung, DEC);
              lcd.setCursor(11,1);lcd.print("S");
              lcd.setCursor(12,1);lcd.print(Verfahr_Speed, DEC);
              break;
      case 1: lcd.setCursor(0,0);lcd.print("Anzahl Bilder   ");
              lcd.setCursor(0,1);lcd.print(Anzahl_Bilder, DEC);
              break;
      case 2: lcd.setCursor(0,0);lcd.print("Intervall in sec ");
              lcd.setCursor(0,1);lcd.print(Intervall, DEC);
              break;
      case 3: lcd.setCursor(0,0);lcd.print("Verfahr mm      ");
              lcd.setCursor(0,1);lcd.print(Verfahr_mm, DEC);
              lcd.setCursor(6,1);lcd.print(verfahrzeit(), DEC);
              lcd.setCursor(10,1);lcd.print("sec       ");
              break;
      case 4: lcd.setCursor(0,0);lcd.print("Verfahr Speed   ");
              lcd.setCursor(0,1);lcd.print(Verfahr_Speed, DEC);
              lcd.setCursor(6,1);lcd.print(verfahrzeit(), DEC);
              lcd.setCursor(10,1);lcd.print("sec       ");
              delay(100);
              break;
      case 5: lcd.setCursor(0,0);lcd.print("Richtung        ");
              lcd.setCursor(0,1);lcd.print(Richtung, DEC);
              break;
      case 6: lcd.setCursor(0,0);lcd.print("Handbetrieb     ");
              lcd.setCursor(0,1);lcd.print("<<<<<<   >>>>>>");
              break;
      case 7: lcd.setCursor(0,0);lcd.print("Ausloseverhalten");
              lcd.setCursor(0,1);lcd.print(Ausloseverhalten, DEC);
              break;
      default: break;
    }
  }
    save_settings();
  

}
