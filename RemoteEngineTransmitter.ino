#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN         
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
uint8_t button_pin = 2;
uint8_t button_pin_dwa = 3;
uint8_t button_pin_trzy = 4;
uint8_t button_pin_cztery = 5;
boolean button_state = 0;
boolean button_state_dwa = 0;
boolean button_state_trzy = 0;
boolean button_state_cztery = 0;

void setup() {
 Serial.begin(9600);
pinMode(7, INPUT);
pinMode(button_pin_dwa, INPUT);
pinMode(button_pin_trzy, INPUT);
pinMode(button_pin_cztery, INPUT);
pinMode(6, OUTPUT);
radio.begin();                  //Starting the Wireless communication
radio.openWritingPipe(address); //Setting the address where we will send the data
radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
radio.stopListening();          //This sets the module as transmitter
}
void loop()
{
button_state = digitalRead(7);
button_state_dwa = digitalRead(button_pin_dwa);
button_state_trzy = digitalRead(button_pin_trzy);
button_state_cztery = digitalRead(button_pin_cztery);

if(button_state == HIGH)
{
radio.write(&button_pin, sizeof(button_pin));                  //Sending the message to receiver
digitalWrite(6, HIGH); 
}

if(button_state_dwa == HIGH)
{
   digitalWrite(6, HIGH); 
radio.write(&button_pin_dwa, sizeof(button_pin_dwa));                      //Sending the message to receiver
}

if(button_state_trzy == HIGH)
{
digitalWrite(6, HIGH); 
radio.write(&button_pin_trzy, sizeof(button_pin_trzy));                     //Sending the message to receiver
}

if(button_state_cztery == HIGH)
{
 digitalWrite(6, HIGH); 
radio.write(&button_pin_cztery, sizeof(button_pin_cztery));                     //Sending the message to receiver
}


delay(7);
digitalWrite(6, LOW); 
}
