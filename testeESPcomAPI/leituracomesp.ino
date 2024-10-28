#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HTTPClient.h>
#include <WiFi.h>  // Adiciona a biblioteca WiFi para ESP32

const char* ssid = "SATC IOT";
const char* password = "IOT2024@#";
const char* serverName = "https://aquametrics-api.vercel.app/documentos";

#define PH_PIN 35              // Pino analógico para o sensor de pH
#define TEMPERATURE_PIN 2      // Pino de entrada para o sensor de temperatura

const float VOLTAGE_REF = 3.3;    
const float OFFSET = 0.4; // Calibracao para a correcao do valor
const int SAMPLES = 10;     

OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature sensors(&oneWire);

// Função para ler o pH
float LeituraPH() {
  float adc_value = 0;
  
  for(int i = 0; i < SAMPLES; i++) {
    adc_value += analogRead(PH_PIN);
    delay(10);
  }
  adc_value = adc_value / SAMPLES;
  
  float voltage = (adc_value * VOLTAGE_REF) / 4095.0;
  float ph_value = 3.5 * voltage + OFFSET;
  
  return ph_value;
}

// Função para ler a temperatura
float LeituraTemperatura() {
  sensors.requestTemperatures();             // Solicita a leitura de temperatura do sensor
  return sensors.getTempCByIndex(0);         // Retorna a temperatura em Celsius
}

void setup() {
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // Define explicitamente o modo Station
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

  analogReadResolution(12);  // Configura a resolução da leitura analógica
  Serial.println("Sistema iniciado!");
  
  sensors.begin();  // Inicializa o sensor de temperatura
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconectando ao Wi-Fi...");
    WiFi.disconnect();
    WiFi.reconnect();
    delay(1000);
  }  

  float ph = LeituraPH();
  Serial.print("pH: ");
  Serial.println(ph, 2);

  float temperatura = LeituraTemperatura();
  Serial.print("Temperatura: ");
  Serial.println(temperatura, 2);

  delay(1000);

  if (WiFi.status() == WL_CONNECTED) {
    String dataHora = "2024-03-21 14:00:00";  

    HTTPClient http;
    http.setConnectTimeout(1000);
    http.setTimeout(1000);
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"data\": \"" + dataHora + "\", "
                        "\"ph\": " + String(ph, 2) + ", "
                        "\"tds\": 1200.2, "
                        "\"temp\": " + String(temperatura, 2) + "}";

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
    delay(1000); 
  }
}
