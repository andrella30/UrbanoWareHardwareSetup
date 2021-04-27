# UrbanoWareHardwareSetup
Código usado para configurar gateway e dispositivo de borda. 

## Pré-requisitos
Necessário ter o Esp32 LoRa e possuir instalado na máquina o Arduino IDE com a configuração para o Esp32 e as seguintes bibliotecas:
[SPI](https://www.arduino.cc/en/reference/SPI),
[LoRa](https://github.com/sandeepmistry/arduino-LoRa),
[Wire](https://www.arduino.cc/en/reference/wire),
[Firebase](https://drive.google.com/file/d/1kZo3UWYF_byA72UTHuT6KflZbYVa8h-2/view),
[Arduino Json](https://drive.google.com/file/d/1pei05r-xAxjebf6ZqcwENCdUQkb5vG4Z/view),
[Tiny GPS](https://github.com/mikalhart/TinyGPSPlus),
[AXP20X](https://github.com/lewisxhe/AXP202X_Library),
[MQUnifiedsensor](https://www.arduino.cc/reference/en/libraries/mqunifiedsensor/),
[Adafruit Sensor](https://github.com/adafruit/Adafruit_Sensor),
[Adafruit BME280](https://github.com/adafruit/Adafruit_BME280_Library/),
[Adafruit MPU6050](https://github.com/adafruit/Adafruit_MPU6050)

Possuir um [Esp32 LoRa TTGO Gps](https://www.saravati.com.br/modulo-wifi-esp32-com-suporte-de-bateria-gps-e-lora-915mhz-ttgo-t-beam) e os sensores [MPU6050]()
[BME280](https://cutt.ly/Ex6ZN2v), [Guva-S12SD](https://cutt.ly/bctAkLB) e [MQ-135](https://cutt.ly/Ex6ZN2v). 


## Configuração do Gateway

### Clone este repositório
$ git clone git@github.com:andrella30/UrbanoWareHardwareSetup.git
<p>Em seguida abra a pasta do projeto no Arduino IDE<p>

### Setup 
No arquivo Setup.ino, deixar descomentado a linha 12. Essa linha identifica quee queremos o arquivo de configuração para o Gateway.

### Base de dados
Nas linhas 6 e 7 temos as variaveis Host e Senha_fire. Essas variáveis conectam o esp32 com a base de dados do Firebase, caso queira usar uma base de dados pessoal é necessário mudar essas variáveis.

### Conectando a Internet
Nessa versão 1.0 do projeto é necessário que o gateway tenha uma conexão WiFi para se comunicar com a base de dados. A linha 9 e 10 temos as variáveis para configurar a rede Wifi.

### Comunicação com dispositivos de borda
Para que o Gateway possa receber mensagens dos dispositivos de borda é necessário inserir os endereços dos dispositivos de borda que serão utilizados no código. O arquivo gateway.ino já possui dois endereços.

### Esp32
Insira o esp32 na máquina. No Arduino IDE procure em qual porta ele se encontra e envie o código.   


## Configuração do Dispositivo de Borda

### Clone este repositório
$ git clone git@github.com:andrella30/UrbanoWareHardwareSetup.git
<p>Em seguida abra a pasta do projeto no Arduino IDE<p>

### Conectar os Sensores ao ESP32
Os sensores MPU6050 E BME280 utilizam de entradas I2C portanto vão utilizar as portar SDA (21) e SCL (22) do Esp32. Já os sensores Guva-S12SD e MQ-135 precisam ser conectados nas portas análogicas, portas 36  e 4 respectivamente.
<p> Após a montagem é necessário inserir o esp32 na máquina, procurar em qual porta ele se encontra e enviar o código. <p>



