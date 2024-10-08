# 💧 Plataforma IoT de Monitoramento da Qualidade da Água

## 🌟 Descrição do Projeto

Este projeto desenvolve uma plataforma IoT para monitoramento em tempo real da qualidade da água utilizando o microcontrolador ESP32, sensores especializados e integração com o MongoDB. Os dados capturados incluem o nível de pH, temperatura e sólidos dissolvidos na água. Eles são enviados para um banco de dados MongoDB e podem ser visualizados em uma interface web interativa.

🔗 **Objetivo**: Monitorar a qualidade da água em reservatórios, aquários e piscinas, exibindo informações essenciais para garantir que as condições ideais sejam mantidas.

---

## 🛠️ Funcionalidades

- **Monitoramento em Tempo Real**: Captação de dados de qualidade da água utilizando sensores de pH, temperatura e TDS (Total Dissolved Solids).
- **Interface Web**: Visualização dos dados em gráficos em tempo real, diretamente em uma interface web acessível de qualquer lugar.
- **Armazenamento de Dados**: Utilização de um banco de dados NoSQL (MongoDB) para armazenar as informações e permitir análise histórica.
- **LEDs Indicadores**: Indicação de qualidade da água com LEDs RGB acoplados ao sistema físico.
- **Display OLED**: Exibição local de informações importantes como pH, temperatura e TDS no próprio dispositivo.

---

## 📋 Componentes Utilizados

- **ESP32**: Microcontrolador para capturar os dados dos sensores e enviar para a plataforma.
- **Sensor de pH (Ph4502)**: Para medir o nível de acidez da água.
- **Sensor de Temperatura (DS18b20)**: Sensor à prova d'água que monitora a temperatura.
- **Sensor de Sólidos Dissolvidos (KS0429 TDS)**: Para medir a concentração de sólidos dissolvidos.
- **MongoDB**: Banco de dados NoSQL para armazenar as leituras dos sensores.
- **Power Bank**: Fonte de alimentação portátil.
- **Display OLED**: Para exibir as leituras em tempo real no dispositivo.
- **LEDs RGB**: Indicadores visuais que sinalizam se os parâmetros estão fora da faixa ideal.

---

## 🚀 Como Funciona

1. O **ESP32** captura as leituras dos sensores conectados (pH, temperatura, TDS).
2. Os dados são processados e enviados para o **MongoDB** via Wi-Fi.
3. A interface web exibe os dados recebidos em gráficos de fácil interpretação.
4. LEDs RGB mudam de cor conforme as leituras indicam boas ou más condições da água.
5. Um **display OLED** local exibe as leituras mais recentes.

---

## 💻 Tecnologias Usadas

- **ESP32**
- **MongoDB** (NoSQL)
- **HTML, CSS e JavaScript** para a interface web
- **Python** no backend para comunicação entre ESP32 e MongoDB

---

##
<a href="https://aqua-metrics.vercel.app/">aqua-metrics.vercel.app</a>
