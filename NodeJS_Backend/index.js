const express = require("express"),
  http = require("http");

const app = express();
const server = http.createServer(app);
const bodyParser = require("body-parser");
const mqtt = require("mqtt");
const client = mqtt.connect("mqtt://broker.hivemq.com:1883");
const { GoogleSpreadsheet } = require("google-spreadsheet");
const cors = require("cors");

app.use(cors());

app.set("port", process.env.PORT || 5000);
app.use(bodyParser.json());

app.get("/getData", async (req, res, next) => {
  let filteredRows = [];
  const rows = await doc.sheetsByIndex[0].getRows();
  rows.forEach((element) => {
    filteredRows.push(element._rawData);
  });
  res.send(filteredRows.reverse());
});

app.get("/spray", async (req, res, next) => {
  client.publish("washroomproject", "ON");
  res.send({ message: "Spray turn on" });
});

const doc = new GoogleSpreadsheet(
  "1TO0qY-qTo4NHYeVOW8jdj-9jWP1LfeAZTGLBGI1VH44"
);

async function authenticate() {
  let filteredRows = [];
  await doc.useServiceAccountAuth(
    require("./timetowashyourhands-659a9829d7cf.json")
  );
  await doc.loadInfo();
  const rows = await doc.sheetsByIndex[0].getRows();
  rows.forEach((element) => {
    filteredRows.push(element._rawData);
  });
  console.log(filteredRows);
}

server.listen(app.get("port"), function () {
  console.log("listening on *:5000");
});

client.on("message", function (topic, message) {
  console.log("Message: " + message.toString());
  if (topic === "washroomprojectdata") {
    appendToSpreadsheet(message).catch(console.error);
  }
});

client.on("connect", function () {
  client.subscribe("washroomproject", function (err) {
    console.log(err);
  });

  client.subscribe("washroomprojectdata", function (err) {
    console.log(err);
  });
});

async function appendToSpreadsheet(message) {
  const splitMessage = message.toString().split("_");
  const date = new Date().toLocaleString("en-US", {
    timeZone: "America/Toronto",
  });
  const occupancy = splitMessage[1] == 1 ? "occupied" : "unoccupied";
  const sheet = await doc.sheetsByIndex[0];
  sheet.addRow([date, splitMessage[0], splitMessage[1], occupancy]);
}

authenticate().catch(console.error);
