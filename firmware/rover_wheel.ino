#include <Servo.h>

Servo sterzo;
const int pinENA = 9;
const int pinIN1 = 8;
const int pinIN2 = 7;
const int pinServo = 3;

void setup() {
  pinMode(pinENA, OUTPUT);
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  sterzo.attach(pinServo);
  sterzo.write(90); // Sterzo diritta
}

void loop() {
  // Avanza
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);
  analogWrite(pinENA, 200); // Velocità 0–255

  // Sterza a sinistra
  sterzo.write(60);
  delay(1000);

  // Sterza a destra
  sterzo.write(120);
  delay(1000);

  // Ritorna centro
  sterzo.write(90);
  delay(500);

  // Ferma
  analogWrite(pinENA, 0);
  delay(1000);
}
