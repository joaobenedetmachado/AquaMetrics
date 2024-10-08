import requests
from datetime import datetime

# URL da API
url = "https://aqua-metrics-api.vercel.app/documentos"  # Substitua pela URL da sua API

# Enviando os dados via POST para a API
enviar = requests.post(url, json={
    "data" : "2021-10-07 10:00:00",
    "ph" : "7",
    "tds" : "20",
    "temp" : "20"
})

if enviar.status_code == 200:
    print("deu certo")
else:
    print("deu errado")