#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Configurações da rede Wi-Fi
const char* ssid = "NOME_DA_SUA_REDE";
const char* password = "SENHA_DA_SUA_REDE";

// URL da sua API (que vai se comunicar com o MongoDB)
String apiEndpoint = "http://api-seu-dominio.com/insertdata"; // Coloque a URL da API que irá interagir com o MongoDB

// Variáveis para os dados a serem enviados
String date = "2024-10-07";
float ph = 7.2;
int tds = 300;
float temp = 25.5;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(apiEndpoint);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> doc;
    doc["data"] = date;
    doc["ph"] = ph;
    doc["tds"] = tds;
    doc["temp"] = temp;

    String jsonData;
    serializeJson(doc, jsonData);

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Dados enviados com sucesso: " + response);
    } else {
      Serial.println("Erro ao enviar dados. Código HTTP: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi desconectado");
  }

  delay(10000);
}
