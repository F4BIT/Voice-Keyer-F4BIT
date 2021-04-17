// Arduino Nano Voice Keyer V1.1
// LCD 16x2 + DFPlayer Mini
// Stéphane F4BIT/N4BIT
// stephane@mangeolle.fr
// Copyleft@2021 for Hamradio

#include <Wire.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
void printDetail(uint8_t type, int value);

// Pin des bouttons

int upButton = 6; // Bouton UP
int downButton = 5; // Bouton Down
int selectButton = 4; // Bouton Validation
const byte interruptPin = 3; // Bouton Stop
int pinbusy = 12; //Pin de detection d'audio sur DFPlayer

// Pin du relais

const int relay1 = 8; // Relais PTT

// Initialise le player MP3

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// Initialise les descriptions des messages

char message1[16] = "CQ CONTEST FR";
char message2[16] = "CQ CONTEST UK";
char message3[16] = "Message 3";
char message4[16] = "Message 4";
char message5[16] = "Message 5";
char message6[16] = "Message 6";

// Constantes divers

int menu = 1;

void setup() {

  mySoftwareSerial.begin(9600);
  Serial.begin(9600);

  pinMode(pinbusy, INPUT);

  // Initialise la carte DFPlayer Mini
  Serial.println();
  Serial.println("DFPlayer Mini");
  Serial.println("Initialisation du DFPlayer...");

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Impossible de démarrer:");
    Serial.println("1.Controler la connection!");
    Serial.println("2.Inserer une SD card!");
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500); // Timeout
  myDFPlayer.volume(25); // Niveau de volume
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL); // Type d equaliseur
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD); // Support des mp3

  // Initialise les boutons

  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  pinMode(interruptPin, INPUT_PULLUP);


  // Initialise le relais

  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, HIGH);

  // Message d'accueil sur le LCD

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   Voice Keyer");
  lcd.setCursor(0, 1);
  lcd.print("  v1.1 - F4BIT");
  delay(3000);

  // Initialise le menu

  updateMenu();
}

void loop() {
    
    if (!digitalRead(downButton)) {
      menu++;
      updateMenu();
      delay(500);
      while (!digitalRead(downButton));
    }
    if (!digitalRead(upButton)) {
      menu--;
      updateMenu();
      delay(500);
      while (!digitalRead(upButton));
    }
    if (!digitalRead(selectButton)) {
      PTTon();
      executeAction();
      delay(500);
      while (!digitalRead(selectButton));
    }    
     if (!digitalRead(pinbusy)) { 
         
      while (!digitalRead(pinbusy))
        if (!digitalRead(interruptPin)) {
      stopAction();
      delay(500);
        }
      while (!digitalRead(selectButton));
       
       stopAction();
  }
}

void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(message1);
      lcd.setCursor(0, 1);
      lcd.print("--->");
      break;
    case 2:
      lcd.clear();
      lcd.print(message2);
      lcd.setCursor(0, 1);
      lcd.print("--->");
      break;
    case 3:
      lcd.clear();
      lcd.print(">message3");
      lcd.setCursor(0, 1);
      lcd.print(" message4");
      break;
    case 4:
      lcd.clear();
      lcd.print(" message3");
      lcd.setCursor(0, 1);
      lcd.print(">message4");
      break;
    case 5:
      lcd.clear();
      lcd.print(">message5");
      lcd.setCursor(0, 1);
      lcd.print(" message6");
      break;
    case 6:
      lcd.clear();
      lcd.print(" message5");
      lcd.setCursor(0, 1);
      lcd.print(">message6");
      break;
    case 7:
      menu = 6;
      break;
  }
}

void executeAction() {
  switch (menu) {
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
    case 3:
      action3();
      break;
    case 4:
      action4();
      break;
    case 5:
      action5();
      break;
    case 6:
      action6();
      break;
  }
}

void action1() {
  Serial.println(message1);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">Executing");
  lcd.setCursor(0, 1);
  lcd.print(message1);
  myDFPlayer.play(1);
}
void action2() {
  Serial.println(message2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">Executing");
  lcd.setCursor(0, 1);
  lcd.print(message2);
  myDFPlayer.play(2);
}
void action3() {
  Serial.println(message3);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">Executing");
  lcd.setCursor(0, 1);
  lcd.print(message3);
  myDFPlayer.play(3);
}
void action4() {
  Serial.println(message4);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">Executing");
  lcd.setCursor(0, 1);
  lcd.print(message4);
  myDFPlayer.play(4);
}
void action5() {
  Serial.println(message5);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">Executing");
  lcd.setCursor(0, 1);
  lcd.print(message5);
  myDFPlayer.play(5);
}
void action6() {
  Serial.println(message6);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">Executing");
  lcd.setCursor(0, 1);
  lcd.print(message6);
  myDFPlayer.play(6);
}

void stopAction() {
  myDFPlayer.stop();
  PTToff();
    updateMenu();
  Serial.println("Message Stopper !");
}

void PTTon() {
  Serial.println("PTT ON");
  digitalWrite(relay1, LOW);
}

void PTToff() {
  Serial.println("PTT OFF");
  digitalWrite(relay1, HIGH);

}
