const express = require("express");
const fs = require("fs");
const path = require("path");
const app = express();
app.use(express.json()); // to support JSON-encoded bodies

const sysVersion = "7.0";
const fileName = "v" + sysVersion + ".bin";

app.get("/", (req, res) => {
  console.log("Hello From /");
  res.sendStatus(200);
});

app.get("/version-check", (req, res) => {
  res.status(200).send(sysVersion);
});

app.get("/esp-update", (req, res) => {
  const file = path.resolve("public", fileName);
  res.download(file); //Return Target File
});

app.get("/logs", (req, res) => {
  const file = path.resolve("public", "logs.txt");
  res.download(file); //Return Target File
});

app.post("/logs", (req, res) => {
  const logger = fs.createWriteStream(path.resolve("public", "logs.txt"), {
    flags: "a", // 'a' means appending (old data will be preserved)
  });

  for (let reading of req.body.data) {
    logger.write(`${reading.split("\n")[0]}, ${reading.split("\n")[1]}\n`); // add the string to the file
  }
  logger.end(); // close string logger
  res.sendStatus(200);
});

app.post("/health-check", (req, res) => {
  const logger = fs.createWriteStream("health-check.txt", {
    flags: "a",
  });

  let nowDate = Date();
  logger.write(
    `Date: ${nowDate}, Connectivity availability: ${req.body.availability}%.\n`
  ); // add the string to the file
  logger.end();
  res.sendStatus(200);
});

app.listen(80, () => console.log(`Started server at port 80!`));

// TO START LOCALLY RUN npm install then npm start
