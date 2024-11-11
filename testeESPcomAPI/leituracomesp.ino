#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <time.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // largura da tela oled
#define SCREEN_HEIGHT 64  // altura da tela oled
#define OLED_RESET -1     // Reset pin (não é necessário para a maioria dos displays OLED)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // insere os valores na classe 

const char* ssid = "SATC IOT";
const char* password = "IOT2024@#";
const char* serverName = "https://aquametrics-api.vercel.app/documentos";

#define PH_PIN 35          // Pino analógico para o sensor de pH
#define TEMPERATURE_PIN 2  // Pino de entrada para o sensor de temperatura

const float OFFSET = 0.4;  //variavel pra calibrar (testar com a agua destilada e fazer a diferenca pra dar 7)
const int SAMPLES = 10; //quantidade de veses que ele vai dividir pra fazer a media da leitura

const int sensorPinTDS = 32;  // Pino do sensor TDS

OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature sensors(&oneWire);

// func pro PH  
float LeituraPH() {
  float adc_value = 0;
  for (int i = 0; i < SAMPLES; i++) {
    adc_value += analogRead(PH_PIN);
    delay(10);
  }
  adc_value = adc_value / SAMPLES;

  float voltage = (adc_value * 3.3) / 4095.0;
  float ph_value = 3.5 * voltage + OFFSET;

  return ph_value;
}

// ler a temperatura e retornar e tal
float LeituraTemperatura() {
  sensors.requestTemperatures();      // Solicita a leitura de temperatura do sensor
  return sensors.getTempCByIndex(0);  // Retorna a temperatura em Celsius
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // define o modo Station
  WiFi.begin(ssid, password); // colocar a rede e a senha

  Serial.print("Conectando a rede> ");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // tenta conectar por 10 segundos
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) { // se tiver conectado
    Serial.println("\nConectado à rede Wi-Fi!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else { // se nao mostra o erro no serial
    Serial.println("\nFalha na conexão Wi-Fi!");
  }

  analogReadResolution(12);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Endereço I2C padrão: 0x3C
    Serial.println(F("Erro ao inicializar o display OLED"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);               // Tamanho do texto
  display.setTextColor(SSD1306_WHITE);  // Cor do texto
 // Configura a resolução da leitura analógica
Serial.println("Sistema iniciado!");

sensors.begin();  // Inicializa o sensor de temperatura

// Configura o fuso horário para Brasília (UTC-3)
configTime((-3 * 3600), 0, "pool.ntp.org");
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


  int valorAnalogico = analogRead(sensorPinTDS);
  
  float tensao = valorAnalogico * (5.0 / 1024.0);

  float compensacaoFator = 1.0 + 0.02 * (temperatura - 25.0);
  float tdsValorTDS = (tensao / compensacaoFator) * 133.42;       
  
  Serial.print("TDS: ");
  Serial.print(tdsValorTDS);
  Serial.println(" ppm");
    
  delay(1000);

  if (WiFi.status() == WL_CONNECTED) {
    // Obtém a data e hora atual em Brasília
    time_t now;
    struct tm* localTime;
    time(&now);
    localTime = localtime(&now);

    char dataHora[20];
    sprintf(dataHora, "%04d-%02d-%02d %02d:%02d:%02d",
            localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
            localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    HTTPClient http;
    http.setConnectTimeout(1000);
    http.setTimeout(1000);
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"data\": \"" + String(dataHora) + "\", "
                     "\"ph\": " + String(ph, 2) + ", "
                     "\"tds\": " + String(tdsValorTDS) + ", "
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
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("pH: ");
  display.println(ph, 2);
  display.print("Temperatura: ");
  display.println(temperatura, 2);
  display.display();
  delay(1000);
}
