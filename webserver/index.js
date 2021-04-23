const express = require("express");
const fs = require("fs");
const path = require("path");
const app = express();
app.use(express.json()); // to support JSON-encoded bodies
// app.use(express.urlencoded()); // to support URL-encoded bodies

app.get("/", (req, res) => {
  const file = path.resolve("public", "v6.0.bin");
  res.download(file); //Return Target File
});

app.get("/esp-update", (req, res) => {
  const file = path.resolve("public", "v6.0.bin");
  res.download(file); //Return Target File
});

app.post("/", (req, res) => {
  // console.log("Got body:", req.body);

  const logger = fs.createWriteStream("log.txt", {
    flags: "a", // 'a' means appending (old data will be preserved)
  });

  // console.log(req.body.data)

  for (let reading of req.body.data) {
    logger.write(`${reading.split("\n")[0]}, ${reading.split("\n")[1]}\n`); // add the string to the file
  }
  logger.end(); // close string logger
  res.sendStatus(200);
});

app.listen(80, () => console.log(`Started server at port 80!`));

// TO START LOCALLY RUN npm install then npm start
