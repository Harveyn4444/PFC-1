#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

File myFile;

float pressure;
float temperature;
float altitude;
float humidity;

void setup() {
  Serial.begin(9600);

  bme.begin();
  if (!bme.begin());
  Serial.println("BME280 - SD Card");

  if (SD.begin(8))
  {
    Serial.println("SD card is present");
  }
  else
  {
    Serial.println("SD card missing or failure");
    while(1);
  }
//Used to clear SD card
/*
  if (SD.exists("BME.txt"))
  {
    Serial.println("Clearing file");
    SD.remove("BME.txt");
    Serial.print("Done");
    Serial.println(" ");
  }
*/
delay(500);
}

void loop() {
temperature = bme.readTemperature();
pressure = bme.readPressure() / 100;
altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
humidity = bme.readHumidity();


  Serial.print(temperature); Serial.println(" *C");
  Serial.print(pressure); Serial.println("hPa");
  Serial.print(altitude); Serial.println("m");
  Serial.print(humidity); Serial.println("%");

  myFile = SD.open("BME.txt", FILE_WRITE);
  if (myFile)
  {
  myFile.print(temperature); myFile.print(",");
  myFile.print(pressure); myFile.print(",");
  myFile.print(altitude); myFile.print(",");
  myFile.println(humidity);
  myFile.close();
  }
  else 
    {
      Serial.println("error opening BME.txt");
    }

delay(500);

}
