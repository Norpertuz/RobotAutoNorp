// Basic sketch for trying out the L293D by www.Electropeak.com
// Running single DC motor by L293D
// 20/08/2018
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";


long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
long duration_two; // variable for the duration of sound wave travel
int distance_two; // variable for the distance measurement
long duration_three; // variable for the duration of sound wave travel
int distance_three; // variable for the distance measurement
bool autopilot = false;

 
#define MOTOR_EN_1_2  6
#define MOTOR_IN1     5
#define MOTOR_IN2     4

#define MOTOR_dwa_EN_1_2  6
#define MOTOR_dwa_IN1     8
#define MOTOR_dwa_IN2     7
 
#define slow 32
#define normal 128
#define fast 255

#define echoPin A2
#define echoPin_two A3
#define echoPin_three A1
#define trigPin 3 
#define trigPin_two 2
#define trigPin_three A4

 
int Speed; // brak sterowania predkoscia 
 
void Forward_Rev(void){
analogWrite(MOTOR_EN_1_2, Speed);
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  
   analogWrite(MOTOR_dwa_EN_1_2, Speed);
  digitalWrite(MOTOR_dwa_IN1, HIGH);
  digitalWrite(MOTOR_dwa_IN2, LOW);
}
 
void Backward_Rev(void){
analogWrite(MOTOR_EN_1_2, Speed);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);

  analogWrite(MOTOR_dwa_EN_1_2, Speed);
  digitalWrite(MOTOR_dwa_IN1, LOW);
  digitalWrite(MOTOR_dwa_IN2, HIGH);
}
 
void Skret_prawo(void){
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_dwa_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_dwa_IN2, HIGH);

  }
  
  void Skret_lewo(void){
    digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_dwa_IN1, HIGH);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_dwa_IN2, LOW);
  
}

void Brake(void){
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_dwa_IN1, HIGH);
  digitalWrite(MOTOR_dwa_IN2, HIGH);
}

 
void setup() {
  
 


  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver 
 
  pinMode(MOTOR_EN_1_2, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_dwa_EN_1_2, OUTPUT);
  pinMode(MOTOR_dwa_IN1, OUTPUT);
  pinMode(MOTOR_dwa_IN2, OUTPUT);

 pinMode(A0, INPUT); // Sets the trigPin as an OUTPUT
 pinMode(A5, INPUT); // Sets the trigPin as an OUTPUT
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
   pinMode(trigPin_two, OUTPUT); // Sets the trigPin as an OUTPUT
     pinMode(trigPin_three, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(echoPin_two, INPUT); // Sets the echoPin as an INPUT
  pinMode(echoPin_three, INPUT); // Sets the echoPin as an INPUT
  digitalWrite(2,LOW);
}
 
void loop() {
int test_autopil = digitalRead(A5);
  //Serial.println(digitalRead(A5));
if(test_autopil > 0 ){
  autopilot = true;
  }else{autopilot = false;}
if(autopilot == false){
   int val = analogRead(A0);
  val = val /4;
 // Serial.println(val,DEC); //Prints as decimal, with line feed
   Speed=val; // Speed default
  //  Serial.println(Speed);
   analogWrite(MOTOR_EN_1_2, Speed);
    analogWrite(MOTOR_dwa_EN_1_2, Speed);

if (radio.available())              //Looking for the data.
{
char text[32] = "";                 //Saving the incoming data 
uint8_t button_state = 0;
  radio.read(&button_state, sizeof(button_state));    //Reading the data
  Serial.println(button_state);

switch (button_state) {
  case 2:
   Forward_Rev(); 
    delay(50);
   Brake();
    break;
  case 3:
     Backward_Rev(); 
    delay(50);
    Brake();
    break;
   case 4:
    
     Skret_lewo();
    delay(50);
    Brake();
    break;
  case 5:
   
Skret_prawo();
    delay(50);
    Brake();
    break;
  }
  
}//autopilot wylaczony, sterowanie pilotem
  
              }else{
      
//przedni

 digitalWrite(trigPin_two, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_two, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_two, LOW);
  duration = pulseIn(echoPin_two, HIGH);
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

   if( distance < 10 && distance > 0)
  {

  Serial.print("Przedni czujnik wykryl ");

  Serial.print("Distance_one: ");
  Serial.print(distance);
  Serial.println(" cm");
      delay(50);
  }

/*
//prawy
 distance_three = analogRead(A5);
  //Serial.print(analogRead(A5));
  if( distance_three < 10 && distance_three > 0)
  {

  Serial.print("Prawy czujnik wykryl ");

   Serial.print("Distance_tthree: ");
  Serial.print(distance_three);
  Serial.println(" cm");
      delay(50);
  }
*/
//lewy
 digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration_two = pulseIn(echoPin, HIGH);
  distance_two = duration_two * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  if( distance_two < 10 && distance_two > 0)
  {

  Serial.print("Lewy czujnik wykryl ");

   Serial.print("Distance_two: ");
  Serial.print(distance_two);
  Serial.println(" cm");
      delay(50);
  }
    
    }//autopilot wlaczony
}
