const express = require("express");
const fs = require("fs");
const app = express();
app.use(express.json()); // to support JSON-encoded bodies
// app.use(express.urlencoded()); // to support URL-encoded bodies

app.get("/", (req, res) => {
    console.log("Got a get");
    res.sendStatus(200);
});

app.post("/", (req, res) => {
    // console.log("Got body:", req.body);

    const logger = fs.createWriteStream("log.txt", {
        flags: "a", // 'a' means appending (old data will be preserved)
    });

    // console.log(req.body.data)

    for (let reading of req.body.data) {
        logger.write(`${reading.split('\n')[0]}, ${reading.split('\n')[1]}\n`); // add the string to the file
    }
    logger.end(); // close string logger
    res.sendStatus(200);
});

app.listen(4000, () => console.log(`Started server at port 4000!`));

// TO START LOCALLY RUN npm install then npm start
