#include <Wire.h>
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;

uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}

int counter = 0;
float TVOC;
float eCO2;
float H2;
float Ethanol;


void setup() {
 Serial.begin(115200);
 delay(500);
 Serial.println("SGP30 test");

  if (! sgp.begin()){
    Serial.println("Sensor not found");
    while (1);
  }
  //This outputs the sensors chips unique serial number, not too sure if necessary
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);

   if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
    if (! sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;
  }
}

void loop() {
//Takes the measurements
TVOC = sgp.TVOC;
eCO2 = sgp.eCO2;
H2 = sgp.rawH2;
Ethanol = sgp.rawEthanol;

 //This warms the sensor up by taking 20 readings which will be indicated as test values
 //These will not be saved
 
 
 for (int counter = 0;counter < 20;counter++) {
  Serial.println("Warming up sensor");
  Serial.print("TVOC "); Serial.print(TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(eCO2); Serial.println(" ppm");
  Serial.print(counter);
  }
if (counter == 20) {
    uint16_t TVOC_base, eCO2_base;
    if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      Serial.println("Failed to get baseline readings");
      return;
    }
    Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
    Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
  }

  Serial.print("TVOC "); Serial.print(TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(eCO2); Serial.println(" ppm");
  Serial.print("Raw H2 "); Serial.print(H2); Serial.print(" \t");
  Serial.print("Raw Ethanol "); Serial.print(Ethanol); Serial.println("");


  
delay(500);
}
