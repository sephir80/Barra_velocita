#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <splash.h>
#include <string.h>
#include <stdio.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define OLED_RESET -1
#define DISTANCEBETWEENSENSOR 400
#define SEMAPHORE_NULL 0
#define SEMAPHORE_START 1
#define SEMAPHORE_STOP 2


int ledPin1 = 1;
int ledPin2 = 2;
int ledPin3 = 3;

int temp;
int tempstop;
float velR;
bool bTimeStarted;
unsigned long watchTime;


Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,OLED_RESET);


void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC,SCREEN_ADDRESS);
  display.display();
  display.clearDisplay();
  drawSchema();
  pinMode(ledPin1,  OUTPUT);
  pinMode(ledPin2,  OUTPUT);
  pinMode(ledPin3,  OUTPUT);
  semaphore(SEMAPHORE_NULL);
}

 

void loop() {
  int valuea = analogRead(A0);
  int valueb = analogRead(A1);

  
  if ((valuea > 900)&&(bTimeStarted==false))
  {
    watchTime=millis();

    drawSchema(); 
    bTimeStarted=true;
    semaphore(SEMAPHORE_START);   
  }
  if ((valueb > 900)&&(bTimeStarted==true))
  {
    watchTime=millis()-watchTime;
    semaphore(SEMAPHORE_STOP);
    velR=velRobot (watchTime,DISTANCEBETWEENSENSOR);
    Serial.println(velR);
    drawSchema();
    writeText(watchTime,velR);    
    bTimeStarted=false;
  }

}

void drawSchema()
{
  display.clearDisplay();
  display.drawLine(0, 31,128,31 ,SSD1306_WHITE);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1,5);
  display.println(F("TIME="));
  display.setCursor(2,45);
  display.println(F("VEL="));
  display.display(); 
}

void writeText(unsigned long time,float speed)
{
  char c[20];
  sprintf(c,"%05d",time);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(65,5);
  display.println(F(c));
  display.setCursor(55,40);
  display.println(speed);
  display.display();
 
}

float velRobot(unsigned long time,int distance)
{
  float v;
  float t;

  t=time/1000.00;
  v=(distance/t);
  Serial.print("time normal=");
  Serial.print(time);
  Serial.println();  
  Serial.print("time float=");
  Serial.println(t);
  Serial.print("v=");
  Serial.println(v);
  return v;
}

void semaphore(int state)
{
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2,  LOW);
  digitalWrite(ledPin3,  LOW);
  switch (state)
  {
    case 0:
      digitalWrite(ledPin2,HIGH);
      break;
    case 1:
      digitalWrite(ledPin3,HIGH);
      break;
    case 2:
      digitalWrite(ledPin1,HIGH);
      break;
  }
}
