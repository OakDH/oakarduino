#include "Wire.h"

#define SOIL_SENS_PIN A0
#define SENS_ADDR 0x5c

//  Values defined through calibration. Might/will change per sensor.
#define SOIL_SENS_WATER_VALUE 200
#define SOIL_SENS_DRY_VALUE 525

struct measurements
{
	float temperature;
	float humidity;
	float soil_moisture;
};

// WARNING: Don't call function more than once per two seconds due to the warming up period
// of the humidity and temperature sensor.
measurements measure_sensors();

void setup() 
{
		Wire.begin();
		Serial.begin(9600);

		while(!Serial) delay(20);
}

void loop() 
{
	
}

measurements measure_sensors()
{
transmit:
	Wire.beginTransmission(SENS_ADDR);
	
	Wire.write(0x03); // Function code. 3 is the read function.
	Wire.write(0x00); // First address to read from.
	Wire.write(0x04); // Number of addresses to read.

	if (Wire.endTransmission() != 0) goto transmit; // We have to do this to warm the sensor up.

	delayMicroseconds(1500); // As instructed in the datasheet.

	Wire.requestFrom(SENS_ADDR, 8); // We request 8 bytes because of the metadata the sensor sends along.
									// The first two bytes of the response message will be the used function
									// code and the number of bytes sent respectively. The last two bytes
									// are the CRC code. This means that the actually requested bytes are the
									// middle four bytes.
	
	measurements to_return = {0}; // Create a new measurements structure and populate it with zeroes.

	Wire.read(); Wire.read(); // Read and do nothing with the first two bytes.

	to_return.humidity = ((Wire.read() << 8) | Wire.read()) / 10.0f;
	to_return.temperature = ((Wire.read() << 8) | Wire.read()) / 10.0f;

	Wire.read(); Wire.read(); // Read CRC and do nothing.

	float soil_sensor = static_cast<float>(analogRead(SOIL_SENS_PIN));

	to_return.soil_moiture = 1 - (soil_sensor - SOIL_SENS_WATER_VALUE) / (SOIL_SENS_DRY_VALUE - SOIL_SENS_WATER_VALUE));

	return to_return;
}

		

