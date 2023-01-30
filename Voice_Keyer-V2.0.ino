// Arduino Nano Voice Keyer V2.1
//  DFPlayer Mini
// Stéphane F4BIT/N4BIT
// stephane@mangeolle.fr
// Copyleft@2023 for Hamradio

#include <Wire.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


// Pin des bouttons

int selectButton1 = 6; // Play 1
int selectButton2 = 5; // Play 2
int selectButton3 = 4; // Play 3
int selectButton4 = 3; // Play 4
const byte interruptPin = 2; // Bouton Stop
int pinbusy = 12; //Pin de detection d'audio sur DFPlayer

// Pin du relais

const int relay1 = 9; // Relais MASSE VOIX
const int relay2 = 8; // Relais VOIX
const int relay3 = 7; // Relais PTT

// Initialise le player MP3

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// Initialise les descriptions des messages

char message1[16] = "CQ CONTEST FR";
char message2[16] = "CQ CONTEST UK";
char message3[16] = "Message 3";
char message4[16] = "Message 4";

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

  pinMode(selectButton1, INPUT_PULLUP);
  pinMode(selectButton2, INPUT_PULLUP);
  pinMode(selectButton3, INPUT_PULLUP);
  pinMode(selectButton4, INPUT_PULLUP);
  pinMode(interruptPin, INPUT_PULLUP);

  // Initialise le relais

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);


  // Initialise le menu

  updateMenu();
}

void loop() {

  if (!digitalRead(pinbusy)) {
   while (!digitalRead(pinbusy))
        if (!digitalRead(interruptPin)) {
      stopAction();
    }
  }
  if (!digitalRead(selectButton1)) {
	menu=1;
    	PTTon();
	executeAction();
	delay(500);
	}
    while (!digitalRead(selectButton1));
  
  if (!digitalRead(selectButton2)) {
	menu=2;
    	PTTon();
	executeAction();
	delay(500);
	}
    while (!digitalRead(selectButton2));
	
  if (!digitalRead(selectButton3)) {
	menu=3;
    	PTTon();
	executeAction();
	delay(500);
	}
    while (!digitalRead(selectButton3));
	
	if (!digitalRead(selectButton4)) {
	menu=4;
    	PTTon();
	executeAction();
	delay(500);
	}
    while (!digitalRead(selectButton4));
	
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
  }
}

void action1() {
  Serial.println(message1);
  myDFPlayer.play(1);
}
void action2() {
  Serial.println(message2);
  myDFPlayer.play(2);
}
void action3() {
  Serial.println(message3);
  myDFPlayer.play(3);
}
void action4() {
  Serial.println(message4);
  myDFPlayer.play(4);
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
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
}

void PTToff() {
  Serial.println("PTT OFF");
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
}
