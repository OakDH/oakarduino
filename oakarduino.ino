#include "Wire.h"

#define SOIL_SENS_WATER_VALUE 200
#define SOIL_SENS_DRY_VALUE 525

void setup() 
{
		Wire.begin();
		Serial.begin(9600);

		while(!Serial) delay(20);
}

void loop() 
{
		Wire.beginTransmission(0x5c);

		Wire.write(0x03);
		Wire.write(0x00);
		Wire.write(0x04);

		if (Wire.endTransmission() != 0) { Serial.println("OOPSIE"); return; }

		delayMicroseconds(1500);

		Wire.requestFrom(0x5c, 8);
		
		Wire.read(); Wire.read();

		float humidity = ((Wire.read() << 8) | Wire.read()) / 10.0f;
		float temperature = ((Wire.read() << 8) | Wire.read()) / 10.0f;
		
		Wire.read(); Wire.read();

		Serial.println("Meting Start");
		Serial.println(humidity);
		Serial.println(temperature);

		float sensor = static_cast<float>(analogRead(A0));

		Serial.println(1 - (sensor - SOIL_SENS_WATER_VALUE)/(SOIL_SENS_DRY_VALUE-SOIL_SENS_WATER_VALUE)); 

		delay(2000);
}

		

