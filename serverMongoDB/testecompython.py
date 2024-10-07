from pymongo import MongoClient

minhaSenha = '123'
client = MongoClient(f"mongodb+srv://joaoteste:{minhaSenha}@iotcluster.a70ey.mongodb.net/?retryWrites=true&w=majority&appName=IotCluster")

db = client["aqua"]
collection = db["aquacollections"]

date = ''
ph = ''
tds = ''
temp = ''

documento = {
    "data": date,
    "ph": ph,
    "tds": tds,
    "temp" : temp
}


resultado = collection.insert_one(documento)

print(f"Documento inserido com ID: {resultado.inserted_id}")


