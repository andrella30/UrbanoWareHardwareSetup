#ifndef MASTER

#include <MQUnifiedsensor.h>
 
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MPU6050.h>

#define RatioMQ135CleanAir 3.6
#define MQ_analog 4

Adafruit_MPU6050 mpu;
Adafruit_BME280 bme;

MQUnifiedsensor MQ135("ESP-32", 5, 10, 36, "MQ-135");

byte localAddress = 0xFD; //FD(sensor01) FF(sensor02)
byte destination = 0xBB;
byte msgCount = 0;
long lastSendTime = 0;        
int interval = 2000;   
float AcZ[5]; 
int count;
int id = 0;

Data data;

void calibrateMQ135(){
  MQ135.setRegressionMethod(1);
  MQ135.init();
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++) {
    MQ135.update();
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
  }
  MQ135.setR0(calcR0/10);
  Serial.println("  done!.");
}

void sendMessage() {
  LoRa.beginPacket();
  LoRa.write(destination);             
  LoRa.write(localAddress);
  LoRa.write(msgCount);
  data = readData();
  LoRa.write((uint8_t*)&data, sizeof(data));
  LoRa.endPacket();
  msgCount++;
}

void airQuality(){
   MQ135.update();
   
   MQ135.setA(605.18); MQ135.setB(-3.937);
   data.cO = MQ135.readSensor();

   MQ135.setA(110.47); MQ135.setB(-2.862); 
   data.cO2 = MQ135.readSensor(); 
  
   MQ135.setA(44.947); MQ135.setB(-3.445); 
   data.tolueno = MQ135.readSensor(); 
  
   MQ135.setA(102.2 ); MQ135.setB(-2.473); 
   data.nH4 = MQ135.readSensor(); 
  
   data.airIndex = analogRead(36);
}

void indiceUv(){
  int leitura_porta = analogRead(4); 
  if (leitura_porta <= 10) {
     data.indiceUV = 0;
  } else if (leitura_porta > 10 && leitura_porta <= 46) {
     data.indiceUV = 1;
  } else if (leitura_porta > 46 && leitura_porta <= 65) {
     data.indiceUV = 2;
  } else if (leitura_porta > 65 && leitura_porta <= 83) {
     data.indiceUV = 3;
  } else if (leitura_porta > 83 && leitura_porta <= 103) {
     data.indiceUV = 4;
  } else if (leitura_porta > 103 && leitura_porta <= 124) {
     data.indiceUV = 5;
  } else if (leitura_porta > 124 && leitura_porta <= 142) {
     data.indiceUV = 6;
  } else if (leitura_porta > 142 && leitura_porta <= 162) {
     data.indiceUV = 7;
  } else if (leitura_porta > 162 && leitura_porta <= 180) {
     data.indiceUV = 8;
  } else if (leitura_porta > 180 && leitura_porta <= 200) {
    data.indiceUV = 9;
  } else if (leitura_porta > 200 && leitura_porta <= 221) {
    data.indiceUV = 10;
  } else {
    data.indiceUV = 11;
  } 
}

Data readData() {

  //Qualidade do Ar
  airQuality();
  Serial.print("Qualidade do ar: ");
  Serial.println(data.airIndex);
  Serial.println(data.cO);
  Serial.println(data.cO2);
  Serial.println(data.tolueno);
  Serial.println(data.nH4);

  //Indice de Radiação
  indiceUv();
  Serial.print("Indice UV: ");
  Serial.println(data.indiceUV);

  Localização do buraco
  Serial.print("ID buraco: ");
  Serial.println(data.IdBur);
  Serial.print("LatitudeBur: ");
  Serial.println(data.latBur, 8);
  Serial.print("LongitudeBur: ");
  Serial.println(data.lngBur, 8);

  data.temperature = bme.readTemperature();
  Serial.print("Temepratura: ");
  Serial.println(data.temperature);
  data.pressure = bme.readPressure();
  Serial.print("Pressão: ");
  Serial.println(data.pressure);
  data.humidity = bme.readHumidity();
  Serial.print("Umidade: ");
  Serial.println(data.humidity);

  //Lolcaização do nó
  data.latitude = gps.location.lat();
  data.longitude = gps.location.lng();
  Serial.print("Latitude: ");
  Serial.println(data.latitude, 8);
  Serial.print("Longitude: ");
  Serial.println(data.longitude, 8);
  
  smartDelay(3000);
  return data;
}
void setup() {
  Serial.begin(115200);

  setupLora();
  setupGps();
  calibrateMQ135();

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050");
  }

   if (!bme.begin(0x76)) {
    Serial.println("Failed to find BME280");
   }


  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }

}

void loop() {
   if (millis() - lastSendTime > interval) {
    sendMessage();
    lastSendTime = millis();            
    interval = random(2000) + 1000;   
  }

   sensors_event_t a, g, temp;
   mpu.getEvent(&a, &g, &temp);
   AcZ[count] = a.acceleration.z;
   
   delay(200);
   
   sensors_event_t a1, g1, temp1;
   mpu.getEvent(&a1, &g1, &temp1);   
   AcZ[count + 1] = a1.acceleration.z;


   if(abs(AcZ[count]) > abs((AcZ[count + 1] * 1.2 ))){
      id++;

      Serial.println("================");
      Serial.println(abs(AcZ[count])); Serial.print(" ");
      Serial.println(abs((AcZ[count + 1] * 1.8 ))); 
      data.IdBur = id;
      data.latBur = gps.location.lat();
      data.lngBur = gps.location.lng();

   }
      
   if(count > 4){
    count = 0;
   }

   count++;
   delay(100);    
}

static void smartDelay(unsigned long ms){
  unsigned long start = millis();
  do {
    while (GPS.available())
      gps.encode(GPS.read());
  } while (millis() - start < ms);
}

#endif
