const express = require('express');
const fs = require('fs');
const app = express();
app.use(express.json());       // to support JSON-encoded bodies
app.use(express.urlencoded()); // to support URL-encoded bodies

app.post('/', (req, res) => {
    // console.log('Got body:', req.body);
    const logger = fs.createWriteStream('log.txt', {
        flags: 'a' // 'a' means appending (old data will be preserved)
    });

    for (let reading of req.body) {
        let nowDate = Date();
        let readingsString = `Date: ${nowDate}, Temperature: ${reading.temperature} , Building is: ${reading.buildingIs}.\n`;

        console.log(readingsString);
        logger.write(readingsString);
    }
    logger.end(); // close string
    res.end(); //send back empty page with status 200 (OK)
});

app.listen(4000, () => console.log(`Started server at port 4000!`));

// TO START LOCALLY RUN node .\webserver\index.js