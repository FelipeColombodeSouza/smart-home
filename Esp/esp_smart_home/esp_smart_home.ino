/* 

  Inclusão de bibliotecas para configuração do Esp na rede.

*/

#if defined(ESP32)
  #include <WiFi.h>
  #define btn_reset 4
  #define ledPin 5
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #define btn_reset D4
  #define ledPin D5
#endif

#include <PubSubClient.h>
#include <Wire.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>

/* 

  Setando pinos do Led para indicação de sucesso ou falha em configuração na rede Wi-Fi.

*/
const int ledBlue = 12;
const int ledGreen = 13;
const int ledRed = 15;

/* 

  Variáveis de conexão na rede Wi-Fi e no MQTT.

*/
const char* ssid = "ESP-Accesso";
const char* password = "";

const char* mqtt_server = "";
const char* comodo = "";

WiFiClient espClient;
PubSubClient client(espClient);

/* 

  Variáveis de utilização em geral.

*/
long lastMsg = 0;
char msg[50];
int value = 0;
String savedSSID = "";
String savedPassword = "";
String tempComodo = "";
String tempMqtt = "";
int contador = 0;
const int eepromAddress = 0;

/* 

  Criando WebServer na porta 80.

*/
AsyncWebServer server(80);

/* 

  Variável com um array literal de um HTML, que será exibido quando conectar no WebServer que o Esp irá criar.

*/
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
  <title>Configurar placa</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {
      font-family: Arial, Helvetica, sans-serif;
      text-align: center;
    }

    h1 {
      font-size: 1.8rem;
      color: white;
    }

    h2 {
      font-size: 1.5rem;
      font-weight: bold;
      color: #143642;
    }

    .topnav {
      overflow: hidden;
      background-color: #143642;
    }

    body {
      margin: 0;
    }

    .content {
      padding: 30px;
      max-width: 600px;
      margin: 0 auto;
    }

    .card {
      background-color: #F8F7F9;
      ;
      box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
      padding-top: 10px;
      padding-bottom: 20px;
    }

    .button {
      padding: 15px 50px;
      font-size: 24px;
      text-align: center;
      outline: none;
      color: #fff;
      background-color: #0f8b8d;
      border: none;
      border-radius: 5px;
      -webkit-touch-callout: none;
      -webkit-user-select: none;
      -khtml-user-select: none;
      -moz-user-select: none;
      -ms-user-select: none;
      user-select: none;
      -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
    }

    /*.button:hover {background-color: #0f8b8d}*/
    .button:active {
      background-color: #0f8b8d;
      box-shadow: 2 2px #CDCDCD;
      transform: translateY(2px);
    }

    .state {
      font-size: 1.5rem;
      color: #8c8c8c;
      font-weight: bold;
    }
  </style>
</head>

<body>
  <div class="topnav">
    <h1>Configuracao da placa</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Configuracao do WiFi</h2>
      <form method='post' action='/save'>
        <p>SSID <input type='text' name='ssid'></p>
        <p>Senha <input type='password' name='password'></p>
        <p>Qual o comodo? 
          <select name="comodo">
            <option value="cozinha">Cozinha</option>
            <option value="quarto">Quarto</option>
            <option value="sala">Sala</option>
            <option value="banheiro">Banheiro</option>            
            <option value="ventilador">Ventilador</option>
            <option value="fogao">Fogao</option>
            <option value="sacada">Sacada</option>
          </select>
        </p>
        <p>IP Servidor <input type='text' name='mqtt_server'></p>
        <input type='submit' class="button" value='Salvar'>
      </form>
    </div>
  </div>
</body>

</html>
)rawliteral";

/* 

  Função que carrega os valores salvos na EEPROM do Esp.

*/
void carregaEEPROM() {
  // Ler SSID e senha armazenados na EEPROM
  savedSSID = readEEPROMString(eepromAddress, 32);
  savedPassword = readEEPROMString(eepromAddress + 32, 64);
  tempComodo = readEEPROMString(eepromAddress + 64, 96);
  comodo = tempComodo.c_str();
  tempMqtt = readEEPROMString(eepromAddress + 96, 128);
  mqtt_server = tempMqtt.c_str();
  

  Serial.print("savedSSID: ");
  Serial.println(savedSSID);
  Serial.print("savedPassword: ");
  Serial.println(savedPassword);
  Serial.print("comodo: ");
  Serial.println(comodo);
  Serial.print("IP Servidor: ");
  Serial.println(mqtt_server);
}

/* 

  Função que cria um WebServer e configura as seguintes rotas:

  /     - Rota que irá exibir o literal do HTML, onde o usuário poderá colocar os valores de SSID, Senha do Wi-Fi, Comodo e Ip do servidor.
  /save - Rota que irá salvar os valores preenchidos anteriormente pelo usuário na EEPROM do Esp.

*/
void startWebServer() {
  criarRede();
  // Configurar o WebServer
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, 0);
  });

  server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
    // Salvar SSID e senha na EEPROM
    writeEEPROMString(eepromAddress, request->arg("ssid"), 32);
    writeEEPROMString(eepromAddress + 32, request->arg("password"), 64);
    writeEEPROMString(eepromAddress + 64, request->arg("comodo"), 96);
    writeEEPROMString(eepromAddress + 96, request->arg("mqtt_server"), 128);
    EEPROM.commit();

    request->send(200, "text/html", "Configuracoes salvas. O ESP32 sera reiniciado.");
    ESP.restart();
  });
}

/* 

  Função que inicializa o Wi-Fi e tenta conectar na rede e no MQTT:

*/
void inicializaWifi() {
  // Inicializar o WiFi
  conectarRede();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

/* 

  Função que conectar na rede Wi-Fi e informa se foi com sucesso ou houve uma falha, com indicação na cor do led RGB:

*/
void conectarRede() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(savedSSID);

  WiFi.begin(savedSSID, savedPassword);

  while (WiFi.status() != WL_CONNECTED && contador <= 3) {
    for (int i = 0; i < 5; i++) {
      digitalWrite(ledBlue, HIGH);
      delay(500);
      digitalWrite(ledBlue, LOW);
      delay(500);
    }    
    delay(5000);
    Serial.println("Tentando se conectar ao WiFi...");
    contador += 1;
  }

  if (WiFi.status() == WL_CONNECTED) { 
    digitalWrite(ledGreen, HIGH);
    Serial.println("WiFi conectado!");
    Serial.println("Endereco de IP:");
    Serial.println(WiFi.localIP());
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
  } else {
    digitalWrite(ledRed, HIGH);
    Serial.println("WiFi não conectado!");
  }
}

/* 

  Função que cria uma rede AP.

*/
void criarRede(){
  Serial.print("Configurando Access Point…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP Access Point: ");
  Serial.println(IP);
}

/* 

  Função inicializa todos os programas, EEPROM, pinos e cria o WebServer.

*/
void setup() {

  pinMode(btn_reset, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT); 

  Serial.begin(115200);
  EEPROM.begin(512);
  carregaEEPROM();

  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);

  if (savedSSID == "" || savedPassword == "" || tempMqtt == "") {
    digitalWrite(ledBlue, HIGH);
    startWebServer();
  } else { 
    inicializaWifi();
  }
  
  server.begin();  
}

/* 

  Função callback que retorna uma mensagem e rota do MQTT, onde ocorrerá a ação de ligar ou desligar algum sistema.

*/
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic).equals(tempComodo)) {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

/* 

  Função reconnect que conecta no servidor MQTT e publica a ação pretendida pelo usuário.

*/
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(comodo);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/* 

  Função loop que aguarda o pressionamento do botão de reset das configuração gravadas na EEPROM. Também verifica se está conectado no Wi-Fi e no servidor MQTT.

*/
void loop() {
  if (digitalRead(btn_reset) == LOW) {
    unsigned long startMillis = millis();
    while (digitalRead(btn_reset) == LOW && (millis() - startMillis) < 5000) {
      // Aguarda até que o botão seja liberado ou até que tenham passado 5 segundos
    }

    if ((millis() - startMillis) >= 5000) {
      xTaskCreate(taskReset, "TaskReset", 10000, NULL, 3, NULL);
      while (digitalRead(btn_reset) == LOW) {
        // Aguarda até que o botão seja liberado para evitar múltiplas ativações
      }
    }
    
  }

  if ((WiFi.status() == WL_CONNECTED) && (tempMqtt != "")) {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }

}

/* 

  Função que reseta os valores salvos na EEPROM, essa função grava valores vazios no lugar.

*/
void taskReset(void *pvParameters) {
  Serial.print("Funcao Reset ");
  writeEEPROMString(eepromAddress, "", 32);
  writeEEPROMString(eepromAddress + 32, "", 64);
  writeEEPROMString(eepromAddress + 64, "", 96);
  EEPROM.commit();
  ESP.restart();
}

/* 

  Função que lê os valores salvos na EEPROM.

*/
String readEEPROMString(int start, int length) {
  String value = "";
  for (int i = 0; i < length; i++) {
    char c = EEPROM.read(start + i);
    if (c == 0) {
      break;
    }
    value += c;
  }
  return value;
}

/* 

  Função que grava os valores na EEPROM.

*/
void writeEEPROMString(int start, const String& value, int length) {
  for (int i = 0; i < length; i++) {
    if (i < value.length()) {
      EEPROM.write(start + i, value[i]);
    } else {
      EEPROM.write(start + i, 0);
    }
  }
}