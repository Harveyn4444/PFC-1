#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;

File myFile;

//Chip select for the SD card
const int CS = 10; //Change 10 whichever pin is CS


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
  //Just checking to make sure the sensor is working
   if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
  //Just checking to make sure the sensor is working 
    if (! sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;
  }

  //Checking if the SD card is working
  if (SD.begin(CS))
  {
    Serial.println("SD card is present");
  }
  else
  {
    Serial.println("SD card missing or failure");
    while(1);
  }
  //Clears the SD card
/*
  if (SD.exists("SGP30.txt"))
  {
    Serial.println("Clearing file");
    SD.remove("SGP30.txt");
    Serial.print("Done");
    Serial.print(" ");
  }
*/
  myFile = SD.open("SGP30.txt", FILE_WRITE);
   if (myFile)
  {
    Serial.println("Writing to SGP30.txt");
    myFile.print("TVOC"); myFile.print(",");
    myFile.print("eCO2"); myFile.print(",");
    myFile.print("H2"); myFile.print(",");
    myFile.println("Ethanol");
    myFile.close();
  }
  else 
    {
      Serial.println("error opening SGP30.txt");
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
  Serial.println(counter);
  }

//Gets some base readings
if (counter == 20) {
    uint16_t TVOC_base, eCO2_base;
    if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      Serial.println("Failed to get baseline readings");
      return;
    }
    Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
    Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
  }


  //Prints to the serial monitor
  Serial.print("TVOC "); Serial.print(TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(eCO2); Serial.println(" ppm");
  Serial.print("Raw H2 "); Serial.print(H2); Serial.print(" \t");
  Serial.print("Raw Ethanol "); Serial.print(Ethanol); Serial.println("");

  myFile = SD.open("SGP30.txt", FILE_WRITE);
   if (myFile)
  {
    Serial.println("Writing to SGP30.txt");
    myFile.print(TVOC); myFile.print(",");
    myFile.print(eCO2); myFile.print(",");
    myFile.print(H2); myFile.print(",");
    myFile.println(Ethanol);
    myFile.close();
  }
  else 
    {
      Serial.println("error opening SGP30.txt");
    }

delay(500);
}
