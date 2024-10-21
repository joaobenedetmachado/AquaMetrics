#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "SATC IOT";
const char* password = "IOT2024@#";
const char* serverName = "https://aquametrics-api.vercel.app/documentos";

const int potPin = 34; // Pino para o sensor de pH
const int tempPin = 35; // Pino para o sensor de temperatura
float ph;
float temperature;

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
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconectando ao Wi-Fi...");
    WiFi.disconnect();
    WiFi.reconnect();
    delay(1000);
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    // Leitura dos sensores
    int phValue = analogRead(potPin); // Leitura do pH
    float phVoltage = phValue * (3.3 / 4095.0); // Converte leitura para voltagem
    ph = phVoltage; // Assumindo que a saída do sensor é diretamente a leitura do pH
    
    // Leitura da temperatura
    int tempValue = analogRead(tempPin);
    float tempVoltage = tempValue * (3.3 / 4095.0);
    // Para LM35: 10 mV/°C, ou seja, multiplicar a voltagem por 100
    temperature = tempVoltage * 100; 
    
    // Obter data e hora atual (você pode implementar NTP aqui)
    String dataHora = "2024-03-21 14:00:00";  
    
    HTTPClient http;
    
    // Configurar timeout mais longo
    http.setConnectTimeout(1000);
    http.setTimeout(1000);
    
    // Iniciar conexão com o servidor
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    
    // Criar payload JSON
    String jsonPayload = "{\"data\": \"" + dataHora + "\", "
                        "\"ph\": " + String(ph, 2) + ", "
                        "\"tds\": 1200.2, "
                        "\"temp\": " + String(temperature, 2) + "}";
    
    Serial.println("Enviando dados: " + jsonPayload);
    
    // Realizar POST
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
    
    delay(1000); // Espera 5 segundos antes da próxima leitura
  }
}
