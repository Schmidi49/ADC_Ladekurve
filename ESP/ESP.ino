/* ESP8266 I2C master-slave communication, requires Arduno Core with I2C Slave Support */

#include <Wire.h>

#define SDA
#define MAX 100

uint16_t values[MAX];
bool ready

void setup()
{
	Wire.begin(0x08);                // join i2c bus with address #8
	Wire.onReceive(receiveEvent); // register event
	Serial.begin(9600); 

	for(int i=0;i<MAX;i++)
	{
		values[i]=0;
	}
}



void loop()
{

}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
     c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}
