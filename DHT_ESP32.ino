#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Substitua pelas credenciais da sua rede
const char *ssid = "WIFI-SSID";
const char *password = "WIFI-PASSWORD";

// Definição de IP estático
IPAddress local_IP(192, 168, 1, 2);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

#define DHTPIN 4  // Pino digital conectado ao sensor DHT

// Descomente o tipo de sensor em uso:
#define DHTTYPE DHT11  // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// Pinos RGB
#define REDPIN 16    // Pino do LED vermelho
#define GREENPIN 17  // Pino do LED verde
#define BLUEPIN 5    // Pino do LED azul

// Pino do LDR
#define LDRPIN 34  // Pino analógico conectado ao LDR

// Cria o objeto AsyncWebServer na porta 80
AsyncWebServer server(80);

String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Falha ao ler o sensor DHT!");
    return "--";
  } else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Falha ao ler o sensor DHT!");
    return "--";
  } else {
    Serial.println(h);
    return String(h);
  }
}

String readLDR() {
  int ldrValue = analogRead(LDRPIN);
  Serial.println("Valor do LDR: " + String(ldrValue));
  return String(ldrValue);
}

String getDHTJson() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int ldrValue = analogRead(LDRPIN);

  if (isnan(t) || isnan(h)) {
    return "{\"erro\":\"Falha ao ler o sensor DHT!\"}";
  }

  String json = "{\"temperatura\":";
  json += String(t);
  json += ", \"umidade\":";
  json += String(h);
  json += ", \"luminosidade\":";
  json += String(ldrValue);
  json += "}";

  return json;
}

void setColor(int red, int green, int blue) {
  analogWrite(REDPIN, red);
  analogWrite(GREENPIN, green);
  analogWrite(BLUEPIN, blue);
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Servidor ESP32 DHT</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperatura</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Umidade</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
  <p>
    <i class="fas fa-sun" style="color:#ffcc00;"></i> 
    <span class="dht-labels">Luminosidade</span> 
    <span id="luminosidade">%LUMINOSIDADE%</span>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperatura", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/umidade", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("luminosidade").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/luminosidade", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Substitui os marcadores pelos valores do DHT e LDR
String processor(const String &var) {
  if (var == "TEMPERATURE") {
    return readDHTTemperature();
  } else if (var == "HUMIDITY") {
    return readDHTHumidity();
  } else if (var == "LUMINOSIDADE") {
    return readLDR();
  }
  return String();
}

void setup() {
  // Porta serial para fins de depuração
  Serial.begin(115200);

  dht.begin();

  // Inicializa os pinos RGB como saída
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  // Configuração do IP estático
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Falha ao configurar IP estático");
    setColor(255, 0, 0);  // Vermelho se falhar na configuração do IP
  }

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Tentando conectar ao Wifi..");
  while (WiFi.status() != WL_CONNECTED) {
    setColor(255, 0, 0);
    delay(500);
    setColor(0, 0, 0);
    delay(500);
  }

  // Verifique se o Wi-Fi foi conectado
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wifi Conectado com sucesso");
    setColor(0, 255, 0);  // Verde se conectado
    Serial.println("IP estático configurado com sucesso:");
    Serial.println(WiFi.localIP());
  }

  // Rota para a página inicial / página web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Rotas para temperatura, umidade e luminosidade
  server.on("/temperatura", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/umidade", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  server.on("/luminosidade", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readLDR().c_str());
  });

  // Rota para obter temperatura, umidade e luminosidade em formato JSON
  server.on("/dht", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = getDHTJson();
    request->send(200, "application/json", json);
  });

  // Inicia o servidor
  server.begin();
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conexao com Wifi perdida! tentando reconectar");
    while (WiFi.status() != WL_CONNECTED) {
      setColor(255, 0, 0);
      delay(500);
      setColor(0, 0, 0);
      delay(500);
    }
  }

  // Se conectado ao Wi-Fi, verifique a temperatura e a umidade
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Verifique se os valores são válidos
  if (!isnan(temperature) && !isnan(humidity)) {
    if (temperature >= 40 || humidity > 85) {
      // Condição mais grave, pisca rápido
      setColor(255, 255, 0);  // Amarelo (vermelho + verde)
      delay(100);
      setColor(0, 0, 0);  // Apaga o LED
      delay(100);
    } else if ((temperature >= 35 && temperature < 40) || (humidity > 75 && humidity <= 85)) {
      // Condição moderada, pisca em uma velocidade intermediária
      setColor(255, 255, 0);  // Amarelo (vermelho + verde)
      delay(250);
      setColor(0, 0, 0);  // Apaga o LED
      delay(250);
    } else if ((temperature >= 30 && temperature < 35) || (humidity > 70 && humidity <= 75)) {
      // Condição menos grave, pisca lentamente
      setColor(255, 255, 0);  // Amarelo (vermelho + verde)
      delay(500);
      setColor(0, 0, 0);  // Apaga o LED
      delay(500);
    } else {
      // Condição normal, LED verde estável
      setColor(0, 255, 0);  // Verde
    }
  } else {
    // Se não conseguir ler os sensores, LED azul estável para indicar erro
    setColor(0, 0, 255);  // Azul
    delay(500);
    setColor(0, 0, 0);  // Apaga o LED
    delay(500);
  }
}
