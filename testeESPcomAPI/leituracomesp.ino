#include <WiFi.h>
#include <HTTPClient.h>

// Defina o nome da rede Wi-Fi e a senha
const char* ssid = "NOME_DA_SUA_REDE";  // Substitua pelo SSID da sua rede
const char* password = "SENHA_DA_SUA_REDE";  // Substitua pela senha da sua rede

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Wi-Fi conectado!");

  // Verifica se está conectado antes de enviar a requisição
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // URL da API
    http.begin("https://aqua-metrics-api.vercel.app/documentos");

    // Definindo o tipo de conteúdo
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"data\":\"2021-10-07 10:00:00\",\"ph\":\"7\",\"tds\":\"20\",\"temp\":\"20\"}";

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Resposta do servidor: " + response);
    } else {
      Serial.println("Erro na requisição: " + String(httpResponseCode));
    }

    // Finalizando a conexão
    http.end();
  } else {
    Serial.println("Erro ao conectar ao Wi-Fi");
  }
}

void loop() {
  // Não precisa de código no loop para este exemplo
}
