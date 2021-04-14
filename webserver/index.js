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
        logger.write(reading); // add the string to the file
    }
    logger.end(); // close string logger
    res.end(); //send back empty page with status 200 (OK)
});

app.listen(4000, () => console.log(`Started server at port 4000!`));

// TO START LOCALLY RUN node .\webserver\index.js