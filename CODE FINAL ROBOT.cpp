#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <GyverOLED.h>

GyverOLED<SSH1106_128x64> oled;

#define BTSerial Serial1                // On associe le BTSerial au Serial 1 de l'arduino (RX1 et TX1)

#define borneENA        53              // On associe la borne "ENA" du L298N à la pin D53 de l'arduino
#define borneIN1        32              // On associe la borne "IN1" du L298N à la pin D32 de l'arduino
#define borneIN2        31              // On associe la borne "IN2" du L298N à la pin D31 de l'arduino
#define borneIN3        50              // On associe la borne "IN3" du L298N à la pin D50 de l'arduino
#define borneIN4        49              // On associe la borne "IN4" du L298N à la pin D49 de l'arduino
#define borneENB        48              // On associe la borne "ENB" du L298N à la pin D48 de l'arduino

#define borneENAB       47              // On associe la borne "ENAB" du L298N à la pin D47 de l'arduino
#define borneIN1B       46              // On associe la borne "IN1B" du L298N à la pin D46 de l'arduino
#define borneIN2B       45              // On associe la borne "IN2B" du L298N à la pin D45 de l'arduino
#define borneIN3B       44              // On associe la borne "IN3B" du L298N à la pin D44 de l'arduino
#define borneIN4B       43              // On associe la borne "IN4B" du L298N à la pin D43 de l'arduino
#define borneENBB       42              // On associe la borne "ENBB" du L298N à la pin D42 de l'arduino

#define trigUV1         33              // On associe la borne "trigUV1" du HC-SR04(1) à la pin D33 de l'arduino
#define echoUV1         34              // On associe la borne "echoUV1" du HC-SR04(1) à la pin D34 de l'arduino
#define trigUV2         35              // On associe la borne "trigUV2" du HC-SR04(2) à la pin D35 de l'arduino
#define echoUV2         36              // On associe la borne "echoUV2" du HC-SR04(2) à la pin D36 de l'arduino
#define trigUV3         37              // On associe la borne "trigUV3" du HC-SR04(3) à la pin D37 de l'arduino
#define echoUV3         38              // On associe la borne "echoUV3" du HC-SR04(3) à la pin D38 de l'arduino

int mode = 2;                           // On initialise la valeur de la variable "mode" à 2 
int distanceCm_1;                       // Variable pour la distance en cm du premier capteur ultrasonique UV1
int distanceCm_2;                       // Variable pour la distance en cm du deuxieme capteur ultrasonique UV2
int distanceCm_3;                       // Variable pour la distance en cm du troisième capteur ultrasonique UV3

long duration_1;                        // Variable pour le temps de récupération de la sonde ultrasonique par le capteur ultrasonique UV1
long duration_2;                        // Variable pour le temps de récupération de la sonde ultrasonique par le capteur ultrasonique UV2
long duration_3;                        // Variable pour le temps de récupération de la sonde ultrasonique par le capteur ultrasonique UV3

void initialisation(){
  SPI.begin();                          // On initialise le SPI bus
  oled.init();                          // On initialise l'oled
  Serial.begin(9600);                   // On initialise le Serial
  BTSerial.begin(38400);                // On initialise le BTSerial
  digitalWrite(borneENA, LOW);          // On initialise la borneENA en état bas
  digitalWrite(borneENB, LOW);          // On initialise la borneENB en état bas
  digitalWrite(borneENAB, LOW);         // On initialise la borneENAB en état bas
  digitalWrite(borneENBB, LOW);         // On initialise la borneENBB en état bas
}

void configuration(){
  // On configure les pins en INPUT ou en OUTPUT
  pinMode(borneENA, OUTPUT);            // On configure la borneENA en sortie
  pinMode(borneIN1, OUTPUT);            // On configure la borneIN1 en sortie
  pinMode(borneIN2, OUTPUT);            // On configure la borneIN2 en sortie
  pinMode(borneIN3, OUTPUT);            // On configure la borneIN3 en sortie
  pinMode(borneIN4, OUTPUT);            // On configure la borneIN4 en sortie
  pinMode(borneENB, OUTPUT);            // On configure la borneENB en sortie
  pinMode(borneENAB, OUTPUT);           // On configure la borneENAB en sortie
  pinMode(borneIN1B, OUTPUT);           // On configure la borneIN1B en sortie
  pinMode(borneIN2B, OUTPUT);           // On configure la borneIN2B en sortie
  pinMode(borneIN3B, OUTPUT);           // On configure la borneIN3B en sortie
  pinMode(borneIN4B, OUTPUT);           // On configure la borneIN4B en sortie
  pinMode(borneENBB, OUTPUT);           // On configure la borneENBB en sortie
  pinMode(trigUV1, OUTPUT);             // On configure le trigUV1 en sortie
  pinMode(trigUV2, OUTPUT);             // On configure le trigUV2 en sortie
  pinMode(trigUV3, OUTPUT);             // On configure le trigUV3 en sortie
  pinMode(echoUV1, INPUT);              // On configure l'echoUV1 en entrée 
  pinMode(echoUV2, INPUT);              // On configure l'echoUV2 en entrée
  pinMode(echoUV3, INPUT);              // On configure l'echoUV3 en entrée
}

void moteurON(){
  digitalWrite(borneENA, HIGH);         // On alimente la borneENA du L298N(1)
  digitalWrite(borneENB, HIGH);         // On alimente la borneENB du L298N(1)
  digitalWrite(borneENAB, HIGH);        // On alimente la borneENAB du L298N(2)
  digitalWrite(borneENBB, HIGH);        // On alimente la borneENBB du L298N(2)
}

void moteurOFF(){
  digitalWrite(borneENA, LOW);          // On etteint la borneENA du L298N(1)
  digitalWrite(borneENB, LOW);          // On etteint la borneENB du L298N(1)
  digitalWrite(borneENAB, LOW);         // On etteint la borneENAB du L298N(2)
  digitalWrite(borneENBB, LOW);         // On etteint la borneENBB du L298N(2)
  oled.clear();                         // On clear tout ce qu'il pourrait être écrit sur l'écran oled
  oled.setCursorXY(0, 0);               // On setup le positionnement pour le début de l'écriture sur l'oled
  oled.print("Je stop !");              // On écrit "Je stop !" sur l'oled
  oled.update();                        // On met à jour l'écran oled avec les nouvelles instructions données
}

void setup() {
  initialisation();                     // On fait un appel à la fonction initialisation() pour initialiser tout le système avant d'agir sur les commandes
  configuration();                      // On fait un appel à la fonction configuration() pour configurer les entrées et sorties des bornes qu'on utilise

  oled.print("Hello robot");                // On écrit "Hello robot" sur l'oled
  oled.update();                            // On met à jour l'écran oled avec les nouvelles instructions données
  BTSerial.write("Connected to you\r\n");   // On envoi un texte via le module Bluetooth "Connected to you" (ce texte sera affiché sur le système de l'autre module Bluetooth qu'on utilise)
  delay(50);                                // On met un petit delay pour le transfert de donnée
}

void avancer(){
  oled.clear();                         // On clear tout ce qu'il pourrait être écrit sur l'écran oled
  oled.setCursorXY(0, 0);               // On setup le positionnement pour le début de l'écriture sur l'écran oled
  oled.print("Avancer");                // On écrit "Avancer" sur l'oled
  oled.update();                        // On met à jour l'écran oled avec les nouvelles instructions données
  digitalWrite(borneIN1, HIGH);         // On met la borneIN1 en état haut
  digitalWrite(borneIN2, LOW);          // On met la borneIN2 en état bas
  digitalWrite(borneIN3, HIGH);         // On met la borneIN3 en état haut
  digitalWrite(borneIN4, LOW);          // On met la borneIN4 en état bas
  digitalWrite(borneIN1B, LOW);         // On met la borneIN1B en état bas
  digitalWrite(borneIN2B, HIGH);        // On met la borneIN2B en état haut
  digitalWrite(borneIN3B, HIGH);        // On met la borneIN3B en état haut
  digitalWrite(borneIN4B, LOW);         // On met la borneIN4B en état bas
  moteurON();                           // On fait appel à la fonction moteur() pour alimenter nos moteurs
}

void reculer(){
  oled.clear();                         // On clear tout ce qu'il pourrait être écrit sur l'oled
  oled.setCursorXY(0, 0);               // On setup le positionnement pour le début de l'écriture sur l'oled
  oled.print("Reculer");                // On écrit "Reculer" sur l'oled
  oled.update();                        // On met à jour l'écran oled avec les nouvelles instructions données
  digitalWrite(borneIN1, LOW);          // On met la borneIN1 en état bas
  digitalWrite(borneIN2, HIGH);         // On met la borneIN2 en état haut
  digitalWrite(borneIN3, LOW);          // On met la borneIN3 en état bas
  digitalWrite(borneIN4, HIGH);         // On met la borneIN4 en état haut
  digitalWrite(borneIN1B, HIGH);        // On met la borneIN1B en état haut
  digitalWrite(borneIN2B, LOW);         // On met la borneIN2B en état bas
  digitalWrite(borneIN3B, LOW);         // On met la borneIN3B en état bas
  digitalWrite(borneIN4B, HIGH);        // On met la borneIN4B en état haut
  moteurON();                           // On fait appel à la fonction moteur() pour alimenter nos moteurs 
}

void droite(){
  oled.clear();                         // On clear tout ce qu'il pourrait être écrit sur l'oled
  oled.setCursorXY(0, 0);               // On setup le positionnement pour le début de l'écriture sur l'oled
  oled.print("Turn right");             // On écrit "Turn right" sur l'oled
  oled.update();                        // On met à jour l'écran oled avec les nouvelles instructions données
  digitalWrite(borneIN1, HIGH);         // On met la borneIN1 en état haut
  digitalWrite(borneIN2, LOW);          // On met la borneIN2 en état bas
  digitalWrite(borneIN3, HIGH);         // On met la borneIN3 en état haut
  digitalWrite(borneIN4, LOW);          // On met la borneIN4 en état bas
  digitalWrite(borneIN1B, HIGH);        // On met la borneIN1B en état haut
  digitalWrite(borneIN2B, LOW);         // On met la borneIN2B en état bas
  digitalWrite(borneIN3B, LOW);         // On met la borneIN3B en état bas
  digitalWrite(borneIN4B, HIGH);        // On met la borneIN4B en état haut
  moteurON();                           // On fait appel à la fonction moteur() pour alimenter nos moteurs 
}

void gauche(){
  oled.clear();                         // On clear tout ce qu'il pourrait être écrit sur l'oled
  oled.setCursorXY(0, 0);               // On setup le positionnement pour le début de l'écriture sur l'oled
  oled.print("Turn left");              // On écrit "Turn left" sur l'oled
  oled.update();                        // On met à jour l'écran oled avec les nouvelles instructions données
  digitalWrite(borneIN1, LOW);          // On met la borneIN1 en état bas
  digitalWrite(borneIN2, HIGH);         // On met la borneIN2 en état haut
  digitalWrite(borneIN3, LOW);          // On met la borneIN3 en état bas
  digitalWrite(borneIN4, HIGH);         // On met la borneIN4 en état haut
  digitalWrite(borneIN1B, LOW);         // On met la borneIN1B en état bas
  digitalWrite(borneIN2B, HIGH);        // On met la borneIN2B en état haut
  digitalWrite(borneIN3B, HIGH);        // On met la borneIN3B en état haut
  digitalWrite(borneIN4B, LOW);         // On met la borneIN4B en état bas
  moteurON();                           // On fait appel à la fonction moteur() pour alimenter nos moteurs 
}

void distance1(){
  digitalWrite(trigUV1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigUV1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigUV1, LOW);
  duration_1 = pulseIn(echoUV1, HIGH);
  distanceCm_1 = duration_1 * 0.034 / 2;
  Serial.print("distance 1 : ");
  Serial.println(distanceCm_1);
}

void distance2(){
  digitalWrite(trigUV2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigUV2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigUV2, LOW);
  duration_2 = pulseIn(echoUV2, HIGH);
  distanceCm_2 = duration_2 * 0.034 / 2;
  Serial.print("distance 2 : ");
  Serial.println(distanceCm_2);
}

void distance3(){
  digitalWrite(trigUV3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigUV3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigUV3, LOW);
  duration_3 = pulseIn(echoUV3, HIGH);
  distanceCm_3 = duration_3 * 0.034 / 2;
  Serial.print("distance 3 : ");
  Serial.println(distanceCm_3);
}

void distance(){
  distance1();
  distance2();
  distance3();
}

void loop() {
  String recu;
  while (BTSerial.available()){
    recu = BTSerial.readString();
    Serial.println(recu);

    if (recu == "Mode manuel !"){
      mode = 0;
      BTSerial.write("ok");
    }

    else if (recu == "Mode auto !"){
      mode = 1;
      BTSerial.write("non ok\r\n");
    }

    while (mode == 0){                  // Tant que mode est à 0, le mode manuel sera actif et nous allons contrôler le robot avec la manette
      recu = BTSerial.readString();     // Cette variable va prendre le texte envoyé par le Bluetooth 
      Serial.print("le message recu en mode manuel est : ");
      Serial.println(recu);

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

      if (recu == "stop"){
        Serial.print("j'arrete");
        moteurOFF();
        mode = 2;
      }
    }

    while (mode == 1){                  // Tant que mode est à 1, le mode automatique sera actif et le robot avancera tout seul selon les paramètres du programme
      recu = BTSerial.readString();     // Cette variable va prendre le texte envoyé par le Bluetooth
      distance();

      while (distanceCm_1 >= 60 and distanceCm_2 >= 60 and distanceCm_3 >= 60 and mode == 1){ 
        recu = BTSerial.readString();
        if (recu == "stop"){
          Serial.print("j'arrete");
          moteurOFF();
          mode = 2;
        }

        else{
          distance();
          avancer();
        }
      }
      
      while (distanceCm_2 < 30 and mode == 1){
        recu = BTSerial.readString();
        if (recu == "stop"){
          Serial.print("j'arrete");
          moteurOFF();
          mode = 2;
        }

        else{
          distance();
          moteurOFF();
          reculer();
          delay(500);
          droite();
        }
      }

      while ((distanceCm_1 < 60 or distanceCm_2 < 60 or distanceCm_3 < 60) and mode == 1){
        distance();
        moteurOFF();
        
        recu = BTSerial.readString();
        if (recu == "stop"){
          Serial.print("j'arrete");
          moteurOFF();
          mode = 2;
        }

        else{
          if (distanceCm_1 < 60){
            droite();
          }
          
          else if (distanceCm_3 < 60){
            gauche();
          }

          else if (distanceCm_2 < 60){
            distance();

            if ((distanceCm_1 and distanceCm_3) < 60){
              reculer();
              delay(100);
              droite();
            }

            moteurOFF();
          }
        }              
      }
    }
  }
  
  while(Serial.available()){
    recu = Serial.readString();
    BTSerial.write(Serial.read());
  }

  if (BTSerial.available()){
    Serial.write(BTSerial.read());
  }
}