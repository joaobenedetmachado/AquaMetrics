#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "SATC IOT";
const char* password = "IOT2024@#";
const char* serverName = "https://aquametrics-api.vercel.app/documentos";

const int potPin = 34;  // Sensor pH
const int tempPin = 35; // Sensor temperatura
float ph;
float temperature;

// Função para fazer a média de leituras para reduzir ruído
float getAverageTemperature() {
  float sum = 0;
  const int numReadings = 10;
  
  for(int i = 0; i < numReadings; i++) {
    int tempValue = analogRead(tempPin);
    float voltage = tempValue * (3.3 / 4095.0); // Converte para voltagem
    // Para LM35: cada 10mV = 1°C
    sum += (voltage * 100.0); // Multiplica por 100 para converter voltage para celsius
    delay(10);
  }
  
  return sum / numReadings;
}

void setup() {
  Serial.begin(115200);
  
  // Configurar resolução do ADC (opcional, mas pode melhorar a precisão)
  analogSetWidth(12); // Define resolução de 12 bits
  analogSetAttenuation(ADC_11db); // Define atenuação para permitir leitura até 3.3V
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Conectando à rede Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado à rede Wi-Fi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Leitura do pH
    int phValue = analogRead(potPin);
    float phVoltage = phValue * (3.3 / 4095.0);
    ph = (3.3 * phVoltage);
    
    // Leitura da temperatura com média de valores
    temperature = getAverageTemperature();
    
    // Debug das leituras
    Serial.print("Temperatura Raw ADC: ");
    Serial.print(analogRead(tempPin));
    Serial.print(" | Temperatura °C: ");
    Serial.println(temperature);
    
    String dataHora = "2024-03-21 14:00:00";
    
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    
    String jsonPayload = "{\"data\": \"" + dataHora + "\", "
                        "\"ph\": " + String(ph, 2) + ", "
                        "\"tds\": 1200.2, "
                        "\"temp\": " + String(temperature, 2) + "}";
    
    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0) {
      Serial.println("Dados enviados com sucesso!");
      Serial.println("Temperatura: " + String(temperature, 2) + "°C");
    } else {
      Serial.print("Erro ao enviar os dados. Código: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  }
  
  delay(2000);
}
