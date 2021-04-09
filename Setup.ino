//SETUP
#include <SPI.h>             
#include <LoRa.h>
#include <Wire.h>
#include <FirebaseESP32.h>
#include <TinyGPS++.h>
#include <axp20x.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>

//#define MASTER

#define SCK 5   // GPIO5  SCK
#define MISO 19 // GPIO19 MISO
#define MOSI 27 // GPIO27 MOSI
#define SS 18   // GPIO18 CS
#define RST 14  
#define DI00 26
#define BAND 915E6 

TinyGPSPlus gps;
HardwareSerial GPS(1);
AXP20X_Class axp;

typedef struct {
  double latitude;
  double longitude;
  
  double latBur;
  double lngBur;
  int IdBur;
  
  int indiceUV;
  
  float cO;
  float cO2;
  float tolueno;
  float nH4;
  float airIndex;

  double temperature;
  double pressure;
  double humidity;
  
}Data;

void setupLora() {
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);
  //LoRa.setSpreadingFactor(8);

  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (1);                      
  }
    Serial.println( "Lora Initialized");
    LoRa.enableCrc();
}

void setupGps(){
  Wire.begin(21, 22);
  if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) {
    Serial.println("AXP192 Begin PASS");
  } else {
    Serial.println("AXP192 Begin FAIL");
  }
  axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
  GPS.begin(9600, SERIAL_8N1, 34, 12);   //17-TX 18-RX
  
}
