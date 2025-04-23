/*
  Rover STEM – Arduino obstacle‑avoiding car
  Corso STEM (PNRR) – docenti Annalisa Pazzi & Lisa Galvani
  =========================================================
  Librerie necessarie:
    • SR04   → https://github.com/maarten-pennings/SR04
    • Servo  → inclusa nell’IDE Arduino
    • (facoltativa) NewPing → https://github.com/livetronic/Arduino-NewPing

  Per installare una libreria: IDE ► Sketch ► Include Library ► Add .ZIP...

  Hardware
  --------
    – Arduino UNO (o Nano)
    – 2 × driver L298N
    – 4 × motori TT con riduttore
    – Sensore ultrasuoni HC‑SR04 su servo SG90
    – Alimentazione 6×AA per i motori (≈7,2 V) + 5 V USB per Arduino

  Wiring (riassunto a fine file) 
*/

#include <Servo.h>
#include "SR04.h"           // ↳ gestisce il sensore HC‑SR04
// #include <NewPing.h>     // ↳ non usata nel codice ma lasciata come riferimento

/* ----------------------- Pin definition ----------------------- */

// Servo
constexpr uint8_t SERVO_PIN = 10;

// Ultrasonic sensor
constexpr uint8_t TRIG_PIN  = A0;
constexpr uint8_t ECHO_PIN  = A1;
constexpr uint16_t MAX_DISTANCE_CM = 200;

// L298N sinistra (SX)
constexpr uint8_t SX_IN1 = 2;
constexpr uint8_t SX_IN2 = 3;
constexpr uint8_t SX_IN3 = 4;
constexpr uint8_t SX_IN4 = 5;

// L298N destra (DX)
constexpr uint8_t DX_IN1 = 6;
constexpr uint8_t DX_IN2 = 7;
constexpr uint8_t DX_IN3 = 8;
constexpr uint8_t DX_IN4 = 9;

/* ----------------------- Globals ----------------------- */

SR04  sr04(ECHO_PIN, TRIG_PIN);
Servo headServo;

bool   goesForward   = false;
long   distanceFront = 0;

/* Mapping logico dei motori
   (inverti pin1/pin2 se un motore gira al contrario)        */
/* Ruote sinistra */
uint8_t motorFrontLeftPin1  = SX_IN3;
uint8_t motorFrontLeftPin2  = SX_IN4;
uint8_t motorRearLeftPin1   = SX_IN1;
uint8_t motorRearLeftPin2   = SX_IN2;
/* Ruote destra */
uint8_t motorFrontRightPin1 = DX_IN1;
uint8_t motorFrontRightPin2 = DX_IN2;
uint8_t motorRearRightPin1  = DX_IN3;
uint8_t motorRearRightPin2  = DX_IN4;

/* ----------------------- Helper prototypes ----------------------- */
void moveForward();
void moveBackward();
void moveStop();
void turnLeft();
void turnRight();
long lookRight();
long lookLeft();

/* ----------------------- Setup ----------------------- */
void setup() {
  Serial.begin(9600);

  /* Servo inizializzato al centro (≈115° nel montaggio scelto) */
  headServo.attach(SERVO_PIN);
  headServo.write(115);
  delay(1000);

  /* Imposto i pin dei motori */
  for (uint8_t pin : {SX_IN1, SX_IN2, SX_IN3, SX_IN4,
                      DX_IN1, DX_IN2, DX_IN3, DX_IN4}) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
}

/* ----------------------- Main loop ----------------------- */
void loop() {
  int distanceRight = 0;
  int distanceLeft  = 0;

  distanceFront = sr04.Distance();   // misura frontale (cm)
  Serial.println(distanceFront);
  delay(40);                         // anti‑rimbalzo sensore

  if (distanceFront <= 25) {         // ostacolo a ≤25 cm
    moveStop();
    delay(100);

    moveBackward();                  // indietreggia
    delay(300);
    moveStop();
    delay(200);

    distanceRight = lookRight();     // valuta destra
    delay(200);
    distanceLeft  = lookLeft();      // valuta sinistra
    delay(200);

    if (distanceRight >= distanceLeft) {
      turnRight();
    } else {
      turnLeft();
    }
    moveStop();

  } else {
    moveForward();                   // via libera
  }
}

/* ----------------------- Ultrasuoni su servo ----------------------- */
long lookRight() {
  headServo.write(50);               // ruota servo a destra
  delay(500);
  long d = sr04.Distance();
  delay(100);
  headServo.write(115);              // torna al centro
  return d;
}

long lookLeft() {
  headServo.write(170);              // ruota servo a sinistra
  delay(500);
  long d = sr04.Distance();
  delay(100);
  headServo.write(115);              // torna al centro
  return d;
}

/* ----------------------- Controllo movimento ----------------------- */
void moveStop() {
  for (uint8_t pin : {motorFrontLeftPin1,  motorFrontLeftPin2,
                      motorRearLeftPin1,   motorRearLeftPin2,
                      motorFrontRightPin1, motorFrontRightPin2,
                      motorRearRightPin1,  motorRearRightPin2}) {
    digitalWrite(pin, LOW);
  }
  goesForward = false;
}

void moveForward() {
  if (goesForward) return;           // evita re‑impostazioni inutili
  goesForward = true;

  /* Ruote sinistra avanti */
  digitalWrite(motorFrontLeftPin1, LOW);
  digitalWrite(motorFrontLeftPin2, HIGH);
  digitalWrite(motorRearLeftPin1,  HIGH);
  digitalWrite(motorRearLeftPin2,  LOW);

  /* Ruote destra avanti */
  digitalWrite(motorFrontRightPin1, LOW);
  digitalWrite(motorFrontRightPin2, HIGH);
  digitalWrite(motorRearRightPin1,  HIGH);
  digitalWrite(motorRearRightPin2,  LOW);
}

void moveBackward() {
  goesForward = false;

  /* Ruote sinistra indietro */
  digitalWrite(motorFrontLeftPin1, HIGH);
  digitalWrite(motorFrontLeftPin2, LOW);
  digitalWrite(motorRearLeftPin1,  LOW);
  digitalWrite(motorRearLeftPin2,  HIGH);

  /* Ruote destra indietro */
  digitalWrite(motorFrontRightPin1, HIGH);
  digitalWrite(motorFrontRightPin2, LOW);
  digitalWrite(motorRearRightPin1,  LOW);
  digitalWrite(motorRearRightPin2,  HIGH);
}

void turnRight() {
  /* Sinistra avanti, destra indietro */
  digitalWrite(motorFrontLeftPin1, LOW);
  digitalWrite(motorFrontLeftPin2, HIGH);
  digitalWrite(motorRearLeftPin1,  HIGH);
  digitalWrite(motorRearLeftPin2,  LOW);

  digitalWrite(motorFrontRightPin1, HIGH);
  digitalWrite(motorFrontRightPin2, LOW);
  digitalWrite(motorRearRightPin1,  LOW);
  digitalWrite(motorRearRightPin2,  HIGH);

  delay(350);   // durata rotazione (ms)
}

void turnLeft() {
  /* Sinistra indietro, destra avanti */
  digitalWrite(motorFrontLeftPin1, HIGH);
  digitalWrite(motorFrontLeftPin2, LOW);
  digitalWrite(motorRearLeftPin1,  LOW);
  digitalWrite(motorRearLeftPin2,  HIGH);

  digitalWrite(motorFrontRightPin1, LOW);
  digitalWrite(motorFrontRightPin2, HIGH);
  digitalWrite(motorRearRightPin1,  HIGH);
  digitalWrite(motorRearRightPin2,  LOW);

  delay(350);  // durata rotazione (ms)
}

/* -------------------------------------------------------------------
   Pinout riepilogativo

   Servo SG90 ... D10
   HC‑SR04 ...... TRIG=A0, ECHO=A1

   L298N sinistra      L298N destra
   IN1 D2              IN1 D6
   IN2 D3              IN2 D7
   IN3 D4              IN3 D8
   IN4 D5              IN4 D9
------------------------------------------------------------------- */
