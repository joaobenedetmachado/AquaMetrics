async function buscarUltimaLeitura() {
  try {
      const response = await fetch('https://aqua-metrics-api.vercel.app/documentos/ultimo');
      const data = await response.json();

      const ultimoDocumento = data.documento;

      console.log(ultimoDocumento)

      document.getElementById('data-area').textContent = `Data: ${ultimoDocumento.data}`
      document.getElementById('ph-value').textContent = ultimoDocumento.ph;
      document.getElementById('tds-value').textContent = ultimoDocumento.tds;
      document.getElementById('temp-value').textContent = ultimoDocumento.temp;

  } catch (error) {
      console.error('erro ao buscar dados da API:', error);
  }
}

document.addEventListener('DOMContentLoaded', () => {
  buscarUltimaLeitura(); 
  setInterval(buscarUltimaLeitura, 1000);  
});
