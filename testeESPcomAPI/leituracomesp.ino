#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Configuração do Wi-Fi
const char* ssid = "SATC IOT";
const char* password = "IOT2024@#";
const char* serverName = "https://aquametrics-api.vercel.app/documentos";

// Pinos dos sensores
const int potPin = 34; // Pino para o sensor de pH
#define ONE_WIRE_BUS 35 // Pino para o sensor DS18B20

// Inicialização para o sensor de temperatura DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float ph;
float temperature;

void setup() {
  Serial.begin(115200);
  
  // Inicialização do sensor de temperatura
  sensors.begin();
  
  // Conexão Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Conectando à rede Wi-Fi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado à rede Wi-Fi!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha na conexão Wi-Fi!");
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconectando ao Wi-Fi...");
    WiFi.disconnect();
    WiFi.reconnect();
    delay(1000);
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    // Leitura do sensor de pH
    int phValue = analogRead(potPin);
    float phVoltage = phValue * (3.3 / 4095.0);
    ph = phVoltage; // Assumindo que a saída do sensor é diretamente a leitura do pH
    
    // Leitura do sensor DS18B20
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0); // Obtém a temperatura em °C
    
    if (temperature != DEVICE_DISCONNECTED_C) {
      Serial.print("Temperatura: ");
      Serial.println(temperature);
    } else {
      Serial.println("Falha ao ler temperatura");
    }
    
    // Obter data e hora atual (você pode implementar NTP aqui)
    String dataHora = "2024-03-21 14:00:00";  
    
    // Preparação e envio de dados via HTTP
    HTTPClient http;
    http.setConnectTimeout(1000);
    http.setTimeout(1000);
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    
    String jsonPayload = "{\"data\": \"" + dataHora + "\", "
                         "\"ph\": " + String(ph, 2) + ", "
                         "\"tds\": 1200.2, "
                         "\"temp\": " + String(temperature, 2) + "}";
    
    Serial.println("Enviando dados: " + jsonPayload);
    
    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Código HTTP: " + String(httpResponseCode));
      Serial.println("Resposta: " + response);
    } else {
      Serial.print("Erro na requisição HTTP: ");
      Serial.println(httpResponseCode);
      Serial.println("Erro: " + http.errorToString(httpResponseCode));
    }
    
    http.end();
    delay(1000); // Espera antes da próxima leitura
  }
}
