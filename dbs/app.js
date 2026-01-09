const API_BASE = "http://localhost:8080/api";

const modeEl = document.getElementById("mode");
const systemStateEl = document.getElementById("systemState");
const waterDepthEl = document.getElementById("waterDepth");
const valveEl = document.getElementById("valve");
const lastUpdateEl = document.getElementById("lastUpdate");
const chart = document.getElementById("chart");
const ctx = chart.getContext("2d");

const autoBtn = document.getElementById("autoBtn");
const manualBtn = document.getElementById("manualBtn");
const valveSlider = document.getElementById("valveSlider");
const valveValue = document.getElementById("valveValue");
const sendValveBtn = document.getElementById("sendValve");

valveSlider.addEventListener("input", () => {
  valveValue.textContent = `${valveSlider.value}%`;
});

async function fetchStatus() {
  try {
    const response = await fetch(`${API_BASE}/status`);
    if (!response.ok) {
      throw new Error("Failed to fetch status");
    }
    const data = await response.json();
    updateStatus(data);
  } catch (error) {
    setUnavailable();
  }
}

function updateStatus(data) {
  modeEl.textContent = data.mode ?? "-";
  systemStateEl.textContent = data.systemState ?? "-";
  waterDepthEl.textContent = `${(data.waterDepth ?? 0).toFixed(2)} cm`;
  valveEl.textContent = `${data.valvePercentage ?? 0}%`;
  lastUpdateEl.textContent = data.lastUpdateMs
    ? new Date(data.lastUpdateMs).toLocaleTimeString()
    : "-";

  drawChart(data.history ?? []);
}

function setUnavailable() {
  modeEl.textContent = "-";
  systemStateEl.textContent = "NOT AVAILABLE";
  waterDepthEl.textContent = "-";
  valveEl.textContent = "-";
  lastUpdateEl.textContent = "-";
  drawChart([]);
}

function drawChart(history) {
  ctx.clearRect(0, 0, chart.width, chart.height);
  ctx.fillStyle = "#f5f7fb";
  ctx.fillRect(0, 0, chart.width, chart.height);

  if (!history.length) {
    ctx.fillStyle = "#666";
    ctx.font = "16px sans-serif";
    ctx.fillText("No data available", 20, 30);
    return;
  }

  const padding = 40;
  const width = chart.width - padding * 2;
  const height = chart.height - padding * 2;

  const values = history.map((item) => item.waterDepth);
  const min = Math.min(...values);
  const max = Math.max(...values);
  const range = Math.max(max - min, 1);

  ctx.strokeStyle = "#1b5ff7";
  ctx.lineWidth = 2;
  ctx.beginPath();

  history.forEach((item, index) => {
    const x = padding + (index / (history.length - 1)) * width;
    const normalized = (item.waterDepth - min) / range;
    const y = padding + height - normalized * height;
    if (index === 0) {
      ctx.moveTo(x, y);
    } else {
      ctx.lineTo(x, y);
    }
  });

  ctx.stroke();

  ctx.fillStyle = "#333";
  ctx.font = "12px sans-serif";
  ctx.fillText(`${max.toFixed(1)} cm`, 10, padding);
  ctx.fillText(`${min.toFixed(1)} cm`, 10, padding + height);
}

async function sendMode(mode) {
  try {
    await fetch(`${API_BASE}/mode`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ mode }),
    });
  } catch (error) {
    console.error(error);
  }
}

async function sendValve() {
  try {
    await fetch(`${API_BASE}/valve`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ percentage: Number(valveSlider.value) }),
    });
  } catch (error) {
    console.error(error);
  }
}

autoBtn.addEventListener("click", () => sendMode("AUTOMATIC"));
manualBtn.addEventListener("click", () => sendMode("MANUAL"));
sendValveBtn.addEventListener("click", sendValve);

fetchStatus();
setInterval(fetchStatus, 2000);
