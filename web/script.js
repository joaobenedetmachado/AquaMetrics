async function buscarUltimaLeitura() {
  try {
    const response = await fetch('https://aqua-metrics-api.vercel.app/documentos/ultimo');
    const data = await response.json();

    const ultimoDocumento = data.documento;

    console.log(ultimoDocumento)

    var phData = parseFloat(ultimoDocumento.ph);  // Use parseFloat para lidar com números decimais
    var tdsData = parseFloat(ultimoDocumento.tds);
    var tempData = parseFloat(ultimoDocumento.temp);
    console.log(phData, tdsData, tempData);

    var container = document.getElementById("obs");

    document.getElementById('data-area').textContent = `Data: ${ultimoDocumento.data}`
    document.getElementById('ph-value').textContent = phData;
    document.getElementById('tds-value').textContent = tdsData;
    document.getElementById('temp-value').textContent = tempData;

    if (phData < 7.2 && !document.getElementById("ph-warning")) {
      var newObs = document.createElement("p");
      newObs.textContent = "PH está muito baixo!";
      newObs.className = "p-text";
      newObs.id = "ph-warning";

      container.appendChild(newObs);
    }

    if (phData >= 7.2) {
      var phWarningElement = document.getElementById("ph-warning");
      if (phWarningElement) {
        phWarningElement.remove();
      }
    }

    if (phData > 7.6 && !document.getElementById("ph-warning")) {
      var newObs = document.createElement("p");
      newObs.textContent = "PH está muito alto!";
      newObs.className = "p-text";
      newObs.id = "ph-warning";

      container.appendChild(newObs);
    }

    if (tdsData < 500 && !document.getElementById("tds-warning-low")) {
      var newObs = document.createElement("p");
      newObs.textContent = "TDS está muito baixo!";
      newObs.className = "p-text";
      newObs.id = "tds-warning-low";

      container.appendChild(newObs);
    }

    if (tdsData >= 500 && tdsData <= 1500) {
      var tdsWarningLowElement = document.getElementById("tds-warning-low");
      if (tdsWarningLowElement) {
        tdsWarningLowElement.remove();
      }
    }

    if (tdsData > 1500 && !document.getElementById("tds-warning-high")) {
      var newObs = document.createElement("p");
      newObs.textContent = "TDS está muito alto!";
      newObs.className = "p-text";
      newObs.id = "tds-warning-high";

      container.appendChild(newObs);
    }

if (tempData < 26 && !document.getElementById("temp-warning-low")) {
  var newObs = document.createElement("p");
  newObs.textContent = "Temperatura está muito baixa!";
  newObs.className = "p-text";
  newObs.id = "temp-warning-low";

  container.appendChild(newObs);
}

if (tempData >= 26 && tempData <= 30) {
  var tempWarningLowElement = document.getElementById("temp-warning-low");
  if (tempWarningLowElement) {
    tempWarningLowElement.remove();
  }
}

if (tempData > 30 && !document.getElementById("temp-warning-high")) {
  var newObs = document.createElement("p");
  newObs.textContent = "Temperatura está muito alta!";
  newObs.className = "p-text";
  newObs.id = "temp-warning-high";

  container.appendChild(newObs);
}

  } catch (error) {
    console.error('Erro ao buscar dados da API:', error);
  }
}

document.addEventListener('DOMContentLoaded', () => {
  buscarUltimaLeitura();
  setInterval(buscarUltimaLeitura, 1000);
});
