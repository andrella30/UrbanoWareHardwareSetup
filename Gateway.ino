//ESP32 GATEWAY
#ifdef MASTER

byte localAddress = 0xBB;

#define Host "urbanoware-cb53e-default-rtdb.firebaseio.com/"
#define Senha_fire "COLap4r4hWSZ81imNfaFTErHQWOKqdtEJ9TyGjSk"

const char* usuario = "NET_2GA3798A";
const char* senha = "B3A3798A";

void setup() {
  Serial.begin(115200);
  setupLora();

  WiFi.begin(usuario, senha);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WIFI...");
 }

 Serial.println("WIFI Conectado");
 Firebase.begin(Host, Senha_fire);
}

void loop() {
   onReceive(LoRa.parsePacket());
}

void onReceive(int packetSize) {
  
  if (packetSize == 0) return;
   
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
   
   //byte incomingLength = LoRa.read();  
 
  String incoming = "";
  Data data;
  LoRa.readBytes((uint8_t*)&data, sizeof(data));
  
  
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
  
  if (recipient != localAddress && recipient != 0xBB) {
    Serial.println("This message is not for me.");
    return;                            
  }

  String sensor = "";
  
  if(String(sender, HEX) == "fd"){
    sensor = "sensor01";

    Firebase.setFloat("sensor01/latitude", data.latitude);
    Firebase.setFloat("sensor01/longitude",data.longitude);
    
    Firebase.setFloat("sensor01/temperature", data.temperature);
    Firebase.setFloat("sensor01/umidade", data.humidity);
    Firebase.setFloat("sensor01/pressao", data.pressure);
    
    Firebase.setFloat("sensor01/co", data.cO);
    Firebase.setFloat("sensor01/co2", data.cO2);
    Firebase.setFloat("sensor01/nh4", data.nH4);
    Firebase.setFloat("sensor01/tolueno", data.tolueno);
    Firebase.setFloat("sensor01/qualidadeAr", data.airIndex);

    Firebase.setFloat("sensor01/indiceUv", data.indiceUV);

    Firebase.setFloat("buracos/"+ String(data.IdBur) + "/latitudeBuracos", data.latBur);
    Firebase.setFloat("buracos/" +String(data.IdBur) +"/longitudeBuracos", data.lngBur);  
    
  }

  if(String(sender, HEX) == "ff"){
    sensor = "Sensor02";
    Firebase.setFloat("sensor02/temperature", data.temperature);

    Firebase.setFloat("sensor02/latitude", data.latitude);
    Firebase.setFloat("sensor02/longitude",data.longitude);
    
    Firebase.setFloat("sensor02/temperature", data.temperature);
    Firebase.setFloat("sensor02/umidade", data.humidity);
    Firebase.setFloat("sensor02/pressao", data.pressure);
    
    Firebase.setFloat("sensor02/co", data.cO);
    Firebase.setFloat("sensor02/co2", data.cO2);
    Firebase.setFloat("sensor02/nh4", data.nH4);
    Firebase.setFloat("sensor02/tolueno", data.tolueno);
    Firebase.setFloat("sensor02/qualidadeAr", data.airIndex);

    Firebase.setFloat("sensor02/indiceUv", data.indiceUV);

    Firebase.setFloat("buracos/"+ String(data.IdBur) + "/latitudeBuracos", data.latBur);
    Firebase.setFloat("buracos/" +String(data.IdBur) +"/longitudeBuracos", data.lngBur); 
  }
  
  Serial.println("Received from: " + sensor);
  Serial.println("Message ID: " + String(incomingMsgId));

  Serial.print("Latitude: ");
  Serial.println(data.latitude); 
  Serial.print("Longitude: ");
  Serial.println(data.longitude);
  
  Serial.print("Temperatura: ");
  Serial.println(data.temperature);
  Serial.print("Pressão: ");
  Serial.println(data.pressure);
  Serial.print("Umidade: ");
  Serial.println(data.humidity);

  Serial.print("Indice UV: ");
  Serial.println(data.indiceUV);

  Serial.print("Qualidade do ar: ");
  Serial.println(data.airIndex);
  Serial.print("CO: ");
  Serial.println(data.cO);
  Serial.print("CO2: ");
  Serial.println(data.cO2);
  Serial.print("Tolueno: ");
  Serial.println(data.tolueno);
  Serial.print("NH4: ");
  Serial.println(data.nH4);
  
  Serial.print("ID buraco: ");
  Serial.println(data.IdBur);
  Serial.print("LatitudeBur: ");
  Serial.println(data.latBur, 8);
  Serial.print("LongitudeBur: ");
  Serial.println(data.lngBur, 8);
    
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println("Packet Size: " + String(packetSize));

  Serial.println();
}

#endif
