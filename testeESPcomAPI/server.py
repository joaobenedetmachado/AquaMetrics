from flask import Flask, request
from pymongo import MongoClient

app = Flask(__name__)

client = MongoClient("mongodb+srv://joaoteste:123@iotcluster.a70ey.mongodb.net/?retryWrites=true&w=majority&appName=IotCluster")
db = client["aqua"]
collection = db["aquacollections"]

@app.route('/insertdata', methods=['POST'])
def insert_data():
    data = request.json
    collection.insert_one(data)
    return {"message": "Dados inseridos com sucesso!"}, 200

if __name__ == "__main__":
    app.run(debug=True)
