#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h> // RFID
#include <SoftwareSerial.h>
#include <GyverOLED.h>
GyverOLED<SSH1106_128x64> oled;
SoftwareSerial BTSerial(2, 3);

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN); 

// Tableau contentent l'ID
byte nuidPICC[4];

const int analogX = A1;
const int analogY = A0;
const int bouton = 4;

int valuex = 0;
int valuey = 0;
int butonstate = 0;

String x = "";
String y = "";

int mode = 2;

void setup() {
  	pinMode (analogX, INPUT);
  	pinMode (analogY, INPUT);
	pinMode (bouton, INPUT);
  	valuex = analogRead(analogX);
  	valuey = analogRead(analogY);

  	Serial.begin(9600); // Init RS232
  	SPI.begin(); // Init SPI bus
  	rfid.PCD_Init(); // Init MFRC522
	oled.init(); // Init oled

  	Serial.begin(9600);
  	Serial.print("Enter AT commands:\r\n");
  	BTSerial.begin(38400);
  	BTSerial.write("connected to you\r\n");
	delay(50);
}

void id(){
	// Affichage de l'ID 
	String UID = "";
	for (byte i = 0; i < rfid.uid.size; i++){
		UID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
		UID.concat(String(rfid.uid.uidByte[i], HEX));
	}
	UID.toUpperCase();
	Serial.println(UID);
	if (UID.substring(1) == "D9 8F 83 9C"){
		BTSerial.write("Mode auto !");
	}
	else {
		BTSerial.write("Mode auto !");
	}
}

void newid(){
	if ( !rfid.PICC_IsNewCardPresent())
	    return;
	if ( !rfid.PICC_ReadCardSerial())
		return;
	
	for (byte i = 0; i < 4; i++) {	// Enregistrer l'ID du badge (4 octets) 
		nuidPICC[i] = rfid.uid.uidByte[i];
	}
	id();
	// Re-Init RFID
	rfid.PICC_HaltA(); // Halt PICC
	rfid.PCD_StopCrypto1(); // Stop encryption on PCD

}

void reculer(){
	BTSerial.write("reculer");
	oled.setCursorXY(0, 0);
	oled.print("reculer");
	oled.update();
}

void avancer(){
	BTSerial.write("avancer");
	oled.setCursorXY(0, 0);
	oled.print("avancer");
	oled.update();
}

void gauche(){
	BTSerial.write("gauche");
	oled.setCursorXY(0, 0);
	oled.print("tourner a gauche");
	oled.update();
}

void droite(){
	BTSerial.write("droite");
	oled.setCursorXY(0, 0);
	oled.print("tourner a droite");
	oled.update();
}

void stop(){
	butonstate = digitalRead(bouton);

	if (butonstate == LOW){
		Serial.print("le bouton est actif");
		BTSerial.write("stop");
		mode = 2;
		oled.clear();
		oled.setCursorXY(0, 0);
		oled.print("je suis en arret !");
		oled.update();
	}
}

void loop(){
	String recu;
	while(BTSerial.available()){
      	recu = BTSerial.readString();
      	Serial.println(recu);
		
		if (recu == "ok"){
			mode = 0;
			Serial.println("je recois");
		}

		if (recu == "non ok"){
			mode = 1;
			Serial.println("je recois");
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
	
	newid();
	
	stop();

	while (mode == 0){
		valuex = analogRead(analogX);
		valuey = analogRead(analogY);
		stop();

		if (valuex < 150){
			droite();
		}

		if (valuex > 850){
			gauche();
		}

		if (valuey > 850){
			reculer();
		}

		if (valuey < 150){
			avancer();
		}
		delay(2500);
		oled.clear();
	}
delay(50);
}