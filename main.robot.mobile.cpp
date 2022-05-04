#Année 2021-2022  
#Groupe2
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <GyverOLED.h>
#define BTSerial Serial1
GyverOLED<SSH1106_128x64> oled;

#define borneENA        53      // On associe la borne "ENA" du L298N à la pin D53 de l'arduino
#define borneIN1        52      // On associe la borne "IN1" du L298N à la pin D52 de l'arduino
#define borneIN2        51      // On associe la borne "IN2" du L298N à la pin D51 de l'arduino
#define borneIN3        50      // On associe la borne "IN3" du L298N à la pin D50 de l'arduino
#define borneIN4        49      // On associe la borne "IN4" du L298N à la pin D49 de l'arduino
#define borneENB        48      // On associe la borne "ENB" du L298N à la pin D48 de l'arduino

#define borneENAB        47      // On associe la borne "ENAB" du L298N à la pin D47 de l'arduino
#define borneIN1B        46      // On associe la borne "IN1B" du L298N à la pin D46 de l'arduino
#define borneIN2B        45      // On associe la borne "IN2B" du L298N à la pin D45 de l'arduino
#define borneIN3B        44      // On associe la borne "IN3B" du L298N à la pin D44 de l'arduino
#define borneIN4B        43      // On associe la borne "IN4B" du L298N à la pin D43 de l'arduino
#define borneENBB        42      // On associe la borne "ENBB" du L298N à la pin D42 de l'arduino

int mode = 2;

String x = "";
String y = "";

void setup() {
  SPI.begin();
  oled.init();

  // Configuration de toutes les pins de l'Arduino en "sortie" (car elles attaquent les entrées du module L298N)
  pinMode(borneENA, OUTPUT);
  pinMode(borneIN1, OUTPUT);
  pinMode(borneIN2, OUTPUT);
  pinMode(borneIN3, OUTPUT);
  pinMode(borneIN4, OUTPUT);
  pinMode(borneENB, OUTPUT);
  pinMode(borneENAB, OUTPUT);
  pinMode(borneIN1B, OUTPUT);
  pinMode(borneIN2B, OUTPUT);
  pinMode(borneIN3B, OUTPUT);
  pinMode(borneIN4B, OUTPUT);
  pinMode(borneENBB, OUTPUT);

  Serial.begin(9600);
  Serial.print("Enter AT commands: ");
  BTSerial.begin(38400);
  BTSerial.write("connected to you\r\n");
  delay(50);
}

void lancerRotationMoteur() {
  digitalWrite(borneENA, HIGH);       // Active l'alimentation du moteur 1
  digitalWrite(borneENB, HIGH);
  digitalWrite(borneENAB, HIGH);
  digitalWrite(borneENBB, HIGH);
  delay(2000);                        // et attend 2 secondes
  
  digitalWrite(borneENA, LOW);        // Désactive l'alimentation du moteur 1
  digitalWrite(borneENB, LOW);
  digitalWrite(borneENAB, LOW);
  digitalWrite(borneENBB, LOW);
  delay(1000);                        // et attend 1 seconde
}

void reculer(){
  oled.clear();
  oled.setCursorXY(0, 0);
  oled.print("reculer");
  oled.update();
  digitalWrite(borneIN1, LOW);
  digitalWrite(borneIN2, HIGH);
  digitalWrite(borneIN3, LOW);
  digitalWrite(borneIN4, HIGH);
  digitalWrite(borneIN1B, LOW);
  digitalWrite(borneIN2B, HIGH);
  digitalWrite(borneIN3B, LOW);
  digitalWrite(borneIN4B, HIGH);
  lancerRotationMoteur();
}

void avancer(){
  oled.clear();
  oled.setCursorXY(0, 0);
  oled.print("avancer");
  oled.update();
  digitalWrite(borneIN1, HIGH);
  digitalWrite(borneIN2, LOW);
  digitalWrite(borneIN3, HIGH);
  digitalWrite(borneIN4, LOW);
  digitalWrite(borneIN1B, HIGH);
  digitalWrite(borneIN2B, LOW);
  digitalWrite(borneIN3B, HIGH);
  digitalWrite(borneIN4B, LOW);
  lancerRotationMoteur();
}

void gauche(){
  oled.clear();
  oled.setCursorXY(0, 0);
  oled.print("turn left");
  oled.update();
}

void droite(){
  oled.clear();
  oled.setCursorXY(0, 0);
  oled.print("turn right");
  oled.update();
}

void loop() {
  String recu;
  while(BTSerial.available()){
    recu = BTSerial.readString();
    Serial.println(recu);

    if (recu == "Mode manuel !"){
      mode = 0;
      BTSerial.write("ok");
    }

    else if (recu == "Mode auto !"){
      mode = 1;
      BTSerial.write("Mode auto actif \r\n");
    }

    while (mode == 0){
      recu = BTSerial.readString();
      if (recu == "Mode auto !"){
        mode = 1;
      }
      if (recu == "avancer"){
        avancer();
      }
      if (recu == "reculer"){
        reculer();
      }
      if (recu == "droite"){
        droite();
      }
      if (recu == "gauche"){
        gauche();
      }
      
    }
  }

  while(Serial.available()){
    recu = Serial.readString();
    BTSerial.write(Serial.read());
  }

  if (BTSerial.available())
    Serial.write(BTSerial.read());

  if (Serial.available())
    BTSerial.write(Serial.read());
}
