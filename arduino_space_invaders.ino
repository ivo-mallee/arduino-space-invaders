#include <Wire.h>
#include <Adafruit_SH1106.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

int tankX = 0;
int OLDtankX = 0;
const int tankY = 45;
byte bulletX[20];
int bulletY[20];
int AlienX[40] = {64};
byte AlienY[40] = {10};
int ENBULLETX = 255;
int ENBULLETY = 255;
byte alienAmount = 10;
int bulletcount = 0;
long lastTime = 200;
long TIME = 0;
int score = 0;
int offset = 0;
int x = 3;
int y = 10;
int o = 0;
byte lives = 3;
long lastattack = 0;
int EnemyCooldown = 3000;
byte MovementAmount = 1;
byte Direction = 1;
byte REFRESH =0;
int cooldown = 200;
int AMMO = 30;
int money = 200;
long LastRefuel =0;
const byte explosion_radius = 10;
boolean GAMESTATE = false;
byte MenuLevel = 0;
int CLOSTALIEN = 0;
int ATPD =1000;
int ATPDOLD =1000;
byte KILLCOUNT =0;
byte WAVE =0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.display();
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  display.clearDisplay();

  for (int i = 0; i < alienAmount; i++)
  {
    x = x + 9 + offset;

    AlienX[i] =  x;
    AlienY[i] = y;
    if (x > 126) {
      y = y + 10;
      x = 3;
    }

  }
  offset = 0;
  display.setTextSize(1);
  display.setTextColor(WHITE);
 if (digitalRead(6) == LOW) {CONSOLE();}

}
void loop() {
  // put your main code here, to run repeatedly:
  if (lives == 0) {GAMESTATE = true;}
  if (GAMESTATE == true) {gameover();}
  
  
  GetUserInput();
  movement();
  EnemyAttack();
  handlephysics();
  draw_to_screen();
  detection_collision();
  if (digitalRead(7) == LOW) {MENU();}
  
  CheckOnAliens();
  refuel();
}


void GetUserInput() {

  TIME = millis();
  if (digitalRead(6) == LOW && TIME - lastTime >= cooldown && AMMO > 0) {
    shootbullet();
    lastTime = millis();
    AMMO--;
  }
  tankX = map(analogRead(A0), 300, 724, 1, 128);


}

void handlephysics() {
  for (int i = 0; i < 10; i++) {
    if (bulletY[i] > -50) {
      bulletY[i]--;
    }
  }
  ENBULLETY=ENBULLETY+1;
  if (ENBULLETY > 49){ENBULLETY = 255; ENBULLETX = 255;}


}

void detection_collision() {
  for (int i = 0; i < 11; i++)
  {
    for (int A = 0; A < alienAmount; A++) {
      
      if (bulletX[i] >= AlienX[A] - 4 && bulletX[i] <= AlienX[A] + 4) {

        if (bulletY[i] >= AlienY[A] - 4 && bulletY[i] <= AlienY[A] + 4) {
          //display.fillCircle(AlienX[A], AlienY[A], 2, BLACK);
          AlienX[A] = 255;
          AlienY[A] = 255;
          bulletY[i] = -10;

          score = score + 10;
          KILLCOUNT++;
          money = money +5;
        }
      }
    }
  }
  if (ENBULLETX >= tankX - explosion_radius-4 && ENBULLETX <= tankX + explosion_radius-4) {
    if (ENBULLETY >= tankY - explosion_radius-4 && ENBULLETY <= tankY + explosion_radius-4)
    {
      
    
      
      
      
      for (byte K = 0; K <= explosion_radius+10; K++) 
      {
        display.fillCircle(ENBULLETX,ENBULLETY,K,WHITE);
        display.display();
        
       
       
       
       }
       delay(1000);
       display.clearDisplay();
      ENBULLETY = 500;
      ENBULLETX = 500;
      lives--;
      
    while (true) {
  if (lives != 255) {
  display.setTextSize(1);
  display.setCursor(1,30);
 
 
  display.print(lives);
  display.println(" lives remaining"); 
  display.display();
  delay(3000);
  lastattack = millis() - 2000;
  display.clearDisplay();
  }
  break;
      
      
      
      
      }

    
    
    
    }








  }






}

void draw_to_screen()
{
  display.fillRect(1, 1, 128, 40, BLACK);
  for (int i = 0; i < alienAmount; i++) {
    display.fillCircle(AlienX[i], AlienY[i], 2, WHITE);
  }
  for (int i = 0; i < 10; i++) {

    if (bulletY[i] > 2) {
      bulletY[i]--;
    }



    display.fillCircle( bulletX[i], bulletY[i] + 1, 2, BLACK);
    display.fillCircle( bulletX[i], bulletY[i], 1, WHITE);



  }

  display.fillCircle(ENBULLETX, ENBULLETY - 1, 1, BLACK);
  display.fillCircle(ENBULLETX, ENBULLETY, 1, WHITE);

  
  
  
  display.fillCircle(OLDtankX, tankY, 5, BLACK);

  OLDtankX = tankX;
  display.fillCircle(tankX, tankY, 3, WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.fillRect(0, 45, 128, 20, WHITE);
  display.fillRect(0, 45, 128, 20, BLACK);
  display.drawLine(0, 45, 128, 45, WHITE);
  display.drawLine(25, 45, 25, 64, WHITE); // line next to score
  display.drawLine(110, 45, 110, 64, WHITE); //line next to lives

  display.setCursor(0, 55);
  display.println(score);
  display.setCursor(113, 55);
  display.println(lives);
  display.setCursor(29, 55);
  display.println("AMMO:");
  display.setCursor(60, 55);
  display.println(AMMO);
  display.display();
}



void shootbullet() {
  bulletX[bulletcount] = tankX;
  bulletY[bulletcount] = tankY;
  bulletcount++;
  if (bulletcount > 10) {
    bulletcount = 0;
    display.fillCircle(OLDtankX, tankY, 5, BLACK);
    OLDtankX = tankX;
    display.fillCircle(tankX, tankY, 3, WHITE);
  }

}

void movement() {

  
    if (Direction == 0) {offset = offset - MovementAmount;}
    if (Direction == 1) {offset = offset + MovementAmount;}
    
    
    
    
    for (int i = 0; i <= alienAmount; i++) {
      if (AlienX[i] != 255) {
        int U = AlienX[i];
        //Serial.println(U);
        if (U >= 128) {Direction = 0;} 
        if (U <= -1) {Direction = 1;} 
        
        
        }
     
      
        
        
        
        
        
        
      
      
      
      
      }
    
    
    


  
  
  for (int i = 0; i < alienAmount; i++)
  {
    x = x + 9;

    if (AlienX[i] != 255) {
      AlienX[i] =  x;
    }
    if (AlienY[i] != 255) {
      AlienY[i] = y;
    }


  }
 
  x = offset + 15;
 

  
}

void EnemyAttack()
{

  if (TIME - lastattack >= EnemyCooldown) {
    ATPD = 1000;
    ATPDOLD = 1000;
     CLOSTALIEN = 400;
      for (int i =2; i <= alienAmount; i++) 
      { 
      ATPD = AlienX[i] - tankX;
      ATPD = abs(ATPD);
      if (ATPD < ATPDOLD && i != 10) {ATPDOLD = ATPD; CLOSTALIEN = i;
      }
      
      ENBULLETX = AlienX[CLOSTALIEN];
      ENBULLETY = AlienY[CLOSTALIEN];
     
        
      
      o++;
      if (o > 400) {
        break;
      }
    

    lastattack = TIME;
  }






}}
void CheckOnAliens() {

  if (KILLCOUNT >= alienAmount) {
    for (int i = 0; i < alienAmount; i++)
    {
      x = x + 9 + offset;

      AlienX[i] =  x;
      AlienY[i] = y;
    }
  
  AMMO = AMMO + 30;
  money = money +100;
  KILLCOUNT=0;
  WAVE++;
  Serial.println(WAVE);
  }
  o = 0;

}

void refuel() {
  if (TIME - LastRefuel > 3000) {
    AMMO++;
    LastRefuel = TIME;
    
    
    }
  
  
  
  }

void gameover() 
{
  display.setTextSize(1);
  display.setCursor(1,30);
  display.println("GAME OVER");
  display.print("SCORE: ");
  display.print(score);
  display.display();
  while (true){}
  
}

void MENU() {
  display.clearDisplay();
  delay(500);
  while (true) 
  {
    display.setCursor(5,10);
    display.println("BACK TO GAME");
    display.setCursor(85,10);
    display.print(money);
    display.print(" $");
    display.setCursor(5,20);
    display.print("upgrade GUN To") ;
    display.print(" ");
    display.print(cooldown-100);
    display.print("MS");
    display.setCursor(5,30);
    display.println("buy more lives");
    display.setCursor(5,40);
    display.println("buy more bullets");
    
    
    
    
    
    display.display();
    
   if (digitalRead(7) == LOW) {delay(1000); display.clearDisplay(); break; } 
    MenuLevel = map(analogRead(A0),300,724,0,4);
    switch (MenuLevel) {
      case 0: display.clearDisplay(); display.setCursor(1,10); display.println("|"); break;
      case 1: display.clearDisplay(); display.setCursor(1,20); display.println("|"); break;
      case 2: display.clearDisplay(); display.setCursor(1,30); display.println("|"); break;
      case 3: display.clearDisplay(); display.setCursor(1,40); display.println("|"); break;
      
      default:
      MenuLevel = 0;
      break;
      }

      if (digitalRead(6) == LOW && MenuLevel == 0) {delay(1000); break;}
      if (digitalRead(6) == LOW && MenuLevel == 1 && money > 99) {money = money - 100; cooldown = cooldown -100; if (cooldown<0) {cooldown = 100;} delay(500);}
      if (digitalRead(6) == LOW && MenuLevel == 2 && money > 199 && lives < 99) {money = money - 200; lives++; delay(500);} 
      if (digitalRead(6) == LOW && MenuLevel == 3 && money > 1 ) {money--; AMMO++; delay(10);}
    
    
    
    }
  }

void CONSOLE() 
{
  boolean EXIT = false;
   display.setCursor(45,1);
    display.println("CONSOLE");
    display.display();
    delay(3000);
  while(true) 
  {
   display.clearDisplay();
   display.setCursor(45,1);
    display.println("CONSOLE");
    
    MenuLevel = map(analogRead(A0),300,724,0,4);
    display.setCursor(10,15);
    display.print("Set");
    display.print(" Money ");
    


    
    display.print(999);
    display.setCursor(10,25);
    display.println("infinite lives");
    display.setCursor(10,35);
    display.println("ERASE EEPROM");
    display.setCursor(10,45);
    display.println("DONE");
    
    switch (MenuLevel) {
      case 0: display.setCursor(1,15); display.println(">"); break;
      case 1: display.setCursor(1,25); display.println(">"); break;
      case 2: display.setCursor(1,35); display.println(">"); break;
      case 3: display.setCursor(1,45); display.println(">"); break;
    }
        display.display();
    
  if (digitalRead(6) == LOW) {
    
    switch (MenuLevel) {
      case 0: money = 32767; break;
      case 1: lives = 255; break;
      case 2: break;
      case 3: EXIT=true; break;
    }





      
  }
    
    
    if (EXIT == true) {delay(2000); display.clearDisplay(); break;}
  

  
  
  
  
  }}







  
