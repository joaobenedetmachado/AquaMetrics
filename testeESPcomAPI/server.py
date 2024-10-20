import requests
from datetime import datetime

# URL da API
url = "https://aquametrics-api.vercel.app/documentos"  # Substitua pela URL da sua API

# Enviando os dados via POST para a API
enviar = requests.post(url, json={
    "data" : "2021-10-07 10:00:00",
    "ph" : 7.4,
    "tds" : 1200.2,
    "temp" : 20.3
})

if enviar.status_code == 200:
    print("deu certo")
else:
    print(enviar.status_code)