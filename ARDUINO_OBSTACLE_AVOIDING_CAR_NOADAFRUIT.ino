

//ARDUINO OBSTACLE AVOIDING CAR//
// Before uploading the code you have to install the necessary library//
//AFMotor Library https://learn.adafruit.com/adafruit-motor-shield/library-install //
//NewPing Library https://github.com/livetronic/Arduino-NewPing// 
//Servo Library https://github.com/arduino-libraries/Servo.git //
// To Install the libraries go to sketch >> Include Library >> Add .ZIP File >> Select the Downloaded ZIP files From the Above links //


#include <SR04.h>
#include <NewPing.h>
#include <Servo.h> 


/*
 *                        ULTRASONIC SENSOR
 *                                |
 *                              SERVO
 *                                |
 *              L298N-SX <--- ARDUINO-1 ---> L298N DX
 *                |                                | 
 *              /   \                            /   \        
 *  DC MOTOR ANT    DC MOTOR POS     DC MOTOR ANT    DC MOTOR POS  
 */


//Arduino
/*
 * 0-
 * 1-
 * 2- L298N SX IN1
 * 3- L298N SX IN2
 * 4- L298N SX IN3
 * 5- L298N SX IN4
 * 6- L298N DX IN1
 * 7- L298N DX IN2
 * 8- L298N DX IN3
 * 9- L298N DX IN4
 * 10-SERVO
 * 11 /
 * 12 /
 * 13 /
 * A0-TRIGGER
 * A1-ECHO
 * 
 * 
 * 
 */


//Servo Motor
#define servopin 10

//Ultrasonic sensor
#define TRIG_PIN A0 
#define ECHO_PIN A1 

#define MAX_DISTANCE 200 
#define MAX_SPEED 190 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

// L298N SX
#define SXIN1 2
#define SXIN2 3
#define SXIN3 4
#define SXIN4 5

// L298N DX
#define DXIN1 6
#define DXIN2 7
#define DXIN3 8
#define DXIN4 9

/*Ultrasonic*/
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN); 

int motor1pin1 ;
int motor1pin2;

int motor2pin1 ;
int motor2pin2 ;

int motor3pin1 ;
int motor3pin2;

int motor4pin1 ;
int motor4pin2 ;

Servo myservo;   

boolean goesForward=false;
long distance;
int speedSet = 0;

void setup() {

  Serial.begin(9600);

/* Servo */
  myservo.attach(servopin);  
  myservo.write(115); 
  delay(2000);
 
 
/* Motors */

  //velocità
  pinMode(SXIN1, OUTPUT);
  pinMode(SXIN2, OUTPUT);
  pinMode(SXIN3, OUTPUT);
  pinMode(SXIN4, OUTPUT);
  pinMode(DXIN1, OUTPUT);
  pinMode(DXIN2, OUTPUT);
  pinMode(DXIN3, OUTPUT);
  pinMode(DXIN4, OUTPUT);


  //1 Anteriore SX - 1
   motor1pin1 = SXIN3;
   motor1pin2 = SXIN4;  

  //2 Posteriose SX - 2
   motor2pin1 = SXIN1;
   motor2pin2 = SXIN2;
   

  //3 Anteriore DX - 3 
  motor3pin1 = DXIN1;
  motor3pin2 = DXIN2;

  //4 Posteriore DX - 4
  motor4pin1 = DXIN3;
  motor4pin2 = DXIN4;
  
}

void loop() {
  
 int distanceR = 0;
 int distanceL =  0;
 distance = sr04.Distance();
 delay(40);
 Serial.println(distance);

  
  
 if(distance<=25)
 {
  Serial.println("Gestione ostacolo");
  Serial.println(distance);
  moveStop();
  delay(100);
  moveBackward();
  delay(300);
  moveStop();
  delay(200);
  distanceR = lookRight();
  delay(200);
  distanceL = lookLeft();
  delay(200);

  if(distanceR>=distanceL)
  {
    turnRight();
    moveStop();
  }else
  {
    turnLeft();
    moveStop();
  }
 }else
 {
  moveForward();
  //moveStop();
 }
 
}

long lookRight()
{
    myservo.write(50); 
    delay(500);
    long distance = sr04.Distance();
    delay(100);
    myservo.write(115); 
    return distance;
}

long lookLeft()
{
    myservo.write(170); 
    delay(500);
    long distance = sr04.Distance();
    delay(100);
    myservo.write(115); 
    return distance;
    delay(100);
}



void moveStop() {


  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, LOW);

  digitalWrite(motor4pin1, LOW);
  digitalWrite(motor4pin2, LOW);

  } 
  
void moveForward() {

 if(!goesForward)
  {
    goesForward=true;

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);

    digitalWrite(motor3pin1, LOW);
    digitalWrite(motor3pin2, HIGH);

    digitalWrite(motor4pin1, HIGH);
    digitalWrite(motor4pin2, LOW);
     
  
  }
}

void moveBackward() {
    goesForward=false;
 
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);

    digitalWrite(motor3pin1, HIGH);
    digitalWrite(motor3pin2, LOW);

    digitalWrite(motor4pin1, LOW);
    digitalWrite(motor4pin2, HIGH );
}  

void turnRight() {
  
    //1 e 2 Avanti, 3 e 4 indietro 

   digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);

    digitalWrite(motor3pin1, HIGH);
    digitalWrite(motor3pin2, LOW);

    digitalWrite(motor4pin1, LOW);
    digitalWrite(motor4pin2, HIGH );
      
  delay(3500);
  
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);

    digitalWrite(motor3pin1, LOW);
    digitalWrite(motor3pin2, HIGH);

    digitalWrite(motor4pin1, HIGH);
    digitalWrite(motor4pin2, LOW);
     
} 
 
void turnLeft() {

     //1 e 2 Indietri, 3 e 4 avanti 
    
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);

   digitalWrite(motor3pin1, LOW);
    digitalWrite(motor3pin2, HIGH);

    digitalWrite(motor4pin1, HIGH);
    digitalWrite(motor4pin2, LOW);
     
  delay(3500);
   digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);

    digitalWrite(motor3pin1, LOW);
    digitalWrite(motor3pin2, HIGH);

    digitalWrite(motor4pin1, HIGH);
    digitalWrite(motor4pin2, LOW);
}  
