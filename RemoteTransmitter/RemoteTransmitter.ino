#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN         
const byte address[6] = "00001";//Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
uint8_t button_pin = 2;
uint8_t button_pin_second = 3;
uint8_t button_pin_third = 4;
uint8_t button_pin_fourth = 5;
boolean button_state = 0;
boolean button_state_second = 0;
boolean button_state_third = 0;
boolean button_state_fourth = 0;

void setup() {
Serial.begin(9600);
pinMode(7, INPUT);
pinMode(button_pin_second, INPUT);
pinMode(button_pin_third, INPUT);
pinMode(button_pin_fourth, INPUT);
pinMode(6, OUTPUT);
radio.begin();                  //Starting the Wireless communication
radio.openWritingPipe(address); //Setting the address where we will send the data
radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
radio.stopListening();          //This sets the module as transmitter
}
void loop()
{
button_state = digitalRead(7);
button_state_second = digitalRead(button_pin_second);
button_state_third = digitalRead(button_pin_third);
button_state_fourth = digitalRead(button_pin_fourth);
if(button_state == HIGH)
{
radio.write(&button_pin, sizeof(button_pin));//Sending the message to receiver
digitalWrite(6, HIGH); 
}

if(button_state_second == HIGH)
{
digitalWrite(6, HIGH); 
radio.write(&button_pin_second, sizeof(button_pin_second));//Sending the message to receiver
}

if(button_state_third == HIGH)
{
digitalWrite(6, HIGH); 
radio.write(&button_pin_third, sizeof(button_pin_third));//Sending the message to receiver
}

if(button_state_fourth == HIGH)
{
digitalWrite(6, HIGH); 
radio.write(&button_pin_fourth, sizeof(button_pin_fourth));//Sending the message to receiver
}

delay(5);
digitalWrite(6, LOW); 
}
