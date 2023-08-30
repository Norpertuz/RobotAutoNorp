#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
long duration; // variable for the duration of sound wave travel
uint8_t distance; // variable for the distance measurement
long duration_two; // variable for the duration of sound wave travel
uint8_t distance_two; // variable for the distance measurement
long duration_three; // variable for the duration of sound wave travel
uint8_t distance_three; // variable for the distance measurement

 
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

 
unsigned short int Speed;
 
void Backward(void){
  analogWrite(MOTOR_EN_1_2, Speed);
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_dwa_EN_1_2, Speed);
  digitalWrite(MOTOR_dwa_IN1, HIGH);
  digitalWrite(MOTOR_dwa_IN2, LOW);
}
 
void Forward(void){
  analogWrite(MOTOR_EN_1_2, Speed);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  analogWrite(MOTOR_dwa_EN_1_2, Speed);
  digitalWrite(MOTOR_dwa_IN1, LOW);
  digitalWrite(MOTOR_dwa_IN2, HIGH);
}
 
void TurnRight(void){
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_dwa_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_dwa_IN2, HIGH);
}
  
void TurnLeft(void){
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_dwa_IN1, HIGH);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_dwa_IN2, LOW);
  
}

void Stop(void){
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_dwa_IN1, HIGH);
  digitalWrite(MOTOR_dwa_IN2, HIGH);
}

void MainSensorMeasure(void){
  digitalWrite(trigPin_two, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_two, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_two, LOW);
  duration = pulseIn(echoPin_two, HIGH);
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  if( distance < 15 && distance > 0)
  {
    Stop(); // stop any moving
    Serial.print(Speed);
    Serial.print("Main sensor triggered");
    Serial.print("Distance_one: ");
    Serial.print(distance);
    Serial.println(" cm");
    Backward(); //move backward
    delay(300);
    Stop();
    SecondarySensorMeasure(true);
    delay(50);
  }else{
    Forward(); 
    }
}

void SecondarySensorMeasure(bool additional_measurement){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration_two = pulseIn(echoPin, HIGH);
  distance_two = duration_two * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  if( distance_two < 15 && distance_two > 0)
  {
   Serial.print("Second Measurement: Obstacle Detection");
   Serial.print("Distance_two: ");
   Serial.print(distance_two);
   Serial.println(" cm");
   TurnLeft();
   delay(600);
   Stop();
  }
  else if(additional_measurement == true){
   TurnRight();
   delay(600);
   Stop();
    }
}

void AdditionalSensorMeasure(void){
 distance_three = analogRead(A5);
  //Serial.print(analogRead(A5));
  if( distance_three < 10 && distance_three > 0)
  {
  Serial.print("Additional measurement obstacle detection ");
  Serial.print("Distance_three: ");
  Serial.print(distance_three);
  Serial.println(" cm");
  }
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
  Stop();
  bool autopilot = digitalRead(A5);
  if(autopilot == false){ //autopilot off, manually controlled
    unsigned short int val = analogRead(A0);
    val = val /4;
    // Serial.println(val,DEC); //Prints as decimal, with line feed
    Speed=val; // Speed default
    analogWrite(MOTOR_EN_1_2, Speed);
    analogWrite(MOTOR_dwa_EN_1_2, Speed);

  if (radio.available())//Looking for the data.
   {
    char text[32] = "";//Saving the incoming data 
    uint8_t button_state = 0;
    radio.read(&button_state, sizeof(button_state));//Reading the data
    Serial.println(button_state);

  switch (button_state) {
    case 2:
      Backward(); 
      delay(50);
      Stop();
      break;
    case 3:
      Forward(); 
      delay(50);
      Stop();
      break;
    case 4:
      TurnLeft();
      delay(50);
      Stop();
      break;
    case 5:
      TurnRight();
      delay(50);
      Stop();
      break;
    }
   }
  }else{//autopilot on, autonomous functionality enabled
    Speed = 128;
    MainSensorMeasure();
    SecondarySensorMeasure(false);
  }
}// end of main loop
