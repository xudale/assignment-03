const API_BASE = `http://127.0.0.1:8080/api`;

const modeEl = document.getElementById("mode");
const waterDepthEl = document.getElementById("waterDepth");
const valveEl = document.getElementById("valve");
const lastUpdateEl = document.getElementById("lastUpdate");
const chart = Highcharts.chart("chart", {
  chart: {
    type: "line",
    height: 280,
  },
  title: {
    text: "Real-time Water Depth Data",
  },
  xAxis: {
    visible: false,
  },
  yAxis: {
    title: {
      text: "Water Depth (cm)",
    },
    min: 0,
    max: 100,
  },
  series: [
    {
      name: "Water Depth",
      data: [],
    },
  ],
  credits: {
    enabled: false,
  },
});

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
  waterDepthEl.textContent = `${(data.waterDepth ?? 0).toFixed(2)} cm`;
  valveEl.textContent = `${data.valvePercentage ?? 0}%`;
  lastUpdateEl.textContent = data.lastUpdateMs
    ? new Date(data.lastUpdateMs).toLocaleTimeString()
    : "-";
  autoBtn.classList = "";
  manualBtn.classList = "";
  if (["AUTOMATIC", "MANUAL"].includes(data.mode)) {
    if (data.mode === "AUTOMATIC") {
      autoBtn.classList.add("primary");
      manualBtn.classList.add("secondary");
      sendValveBtn.classList.add("disabled");
    } else if (data.mode === "MANUAL") {
      manualBtn.classList.add("primary");
      autoBtn.classList.add("secondary");
      sendValveBtn.classList.remove("disabled");
    }
  } else {
    autoBtn.classList.add("disabled");
    manualBtn.classList.add("disabled");
    sendValveBtn.classList.add("disabled");
  }

  drawChart(data.history ?? []);
}

function setUnavailable() {
  modeEl.textContent = "NOT AVAILABLE";
  waterDepthEl.textContent = "-";
  valveEl.textContent = "-";
  lastUpdateEl.textContent = "-";
  autoBtn.classList = "";
  manualBtn.classList = "";
  sendValveBtn.classList = "";
  autoBtn.classList.add("disabled");
  manualBtn.classList.add("disabled");
  sendValveBtn.classList.add("disabled");
  drawChart([]);
}

function drawChart(history) {
  

  if (!history.length) {
    chart.series[0].setData([], true);
    chart.setTitle(null, { text: "No data available" });
    return;
  }

  const seriesData = history.map((item) => [
    item.timestamp ?? Date.now(),
    item.waterDepth ?? 0,
  ]);

  chart.series[0].setData(seriesData, true);
  chart.setTitle(null, { text: "" });
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

async function sendValve(event) {
  if (event.target.classList.contains("disabled")) return;
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

autoBtn.addEventListener("click", () => {
  if (autoBtn.classList.contains("disabled")) return;
  sendMode("AUTOMATIC")
});
manualBtn.addEventListener("click", () => {
  if (manualBtn.classList.contains("disabled")) return;
  sendMode("MANUAL");
});
sendValveBtn.addEventListener("click", sendValve);

fetchStatus();
setInterval(fetchStatus, 2000);
