/*
  VIBRA SENSE 2 Sensor Acquisition 

  This code reads raw digital values from the Vibra Sense 2 vibration sensor
  through I2C communication and sends the data to the computer via Serial USB.

  Output format:
  time_us,raw_value

  Hardware:
  - Arduino Nano ESP32
  - Vibra Sense 2 sensor with MCP3221 12-bit ADC
*/

#include <Wire.h>               // Library required for I2C communication
#define SENSOR_ADDR 0x4D        // I2C address of the sensor


void setup() {

  // Initialise I2C communication between the sensor and the Arduino
  Wire.begin();

  // Initialise Serial communication with the computer
  Serial.begin(9600);

  // Wait until the Serial communication is available
  while (!Serial);

}

/*
  FUNCTION TO READ THE SENSOR:
  Requests the current measurement from the vibration sensor via I2C and
  returns the corresponding 12-bit raw digital value.
*/

uint16_t readSensorRaw() {

  // Request 2 bytes from the sensor
  Wire.requestFrom((uint8_t)SENSOR_ADDR, (uint8_t)2);

  // Check if 2 bytes are available
  if (Wire.available() == 2) {

    // Read the most significant byte
    byte msb = Wire.read();

    // Read the least significant byte
    byte lsb = Wire.read();

    // Combine both bytes into a 16-bit number
    uint16_t value = ((uint16_t)msb << 8) | lsb;

    // Keep only the 12 bits corresponding to the ADC value
    value = value & 0x0FFF;

    // Return the raw ADC value
    return value;
  }

  // Return 0 if the sensor reading fails
  return 0;
}


void loop() {

  // Read the sensor value
  uint16_t rawValue = readSensorRaw();

  // Send timestamp and sensor value to the computer in CSV format
  Serial.print(micros());
  Serial.print(",");
  Serial.println(rawValue);

  // Small delay between readings to control the sampling rate
  delayMicroseconds(2500);  

}
