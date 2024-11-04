#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128      // Largura da tela OLED
#define SCREEN_HEIGHT 64      // Altura da tela OLED
#define OLED_RESET -1         // Reset pin (não é necessário para a maioria dos displays OLED)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PH_PIN 35             // Pino analógico para o sensor de pH
#define TEMPERATURE_PIN 4     // Pino de entrada para o sensor de temperatura

const float VOLTAGE_REF = 3.3;
const float OFFSET = 0.7;
const int SAMPLES = 10;

OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature sensors(&oneWire);

// Função para ler o pH
float LeituraPH() {
    float adc_value = 0;

    for (int i = 0; i < SAMPLES; i++) {
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
    sensors.requestTemperatures();           // Solicita a leitura de temperatura do sensor
    return sensors.getTempCByIndex(0);       // Retorna a temperatura em Celsius
}

void setup() {
    Serial.begin(115200);
    analogReadResolution(12);
    Serial.println("Sistema iniciado!");

    sensors.begin();  // Inicializa o sensor de temperatura

    // Inicializa o display OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Endereço I2C padrão: 0x3C
        Serial.println(F("Erro ao inicializar o display OLED"));
        for (;;);
    }
    display.clearDisplay();
    display.setTextSize(1);      // Tamanho do texto
    display.setTextColor(SSD1306_WHITE); // Cor do texto
}

void loop() {
    // Leitura do pH
    float ph = LeituraPH();
    Serial.print("pH: ");
    Serial.println(ph, 2);

    // Leitura da temperatura
    float temperatura = LeituraTemperatura();
    Serial.print("Temperatura: ");
    Serial.println(temperatura, 2);

    // Exibe o pH e a temperatura no display OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("pH: ");
    display.println(ph, 2);
    display.print("Temperatura: ");
    display.println(temperatura, 2);
    display.display();

    delay(1000);
}
