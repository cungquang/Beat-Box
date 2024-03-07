const fs = require('fs');
const mime = require('mime-types');
const path = require('path');

// Construct response 200
const send200 = (response, filePath, fileContents) => {
    response.writeHead(200, {"Content-Type": mime.lookup(filePath)});
    response.end(fileContents);
};

// Construct response for 404
const send404 = (response) => {
    response.writeHead(404, {'Content-Type':'text/plain'});
    response.write('Error 404: resource not found');
    response.end();
};


const serverStatic = (response, absPath) => {
    if (fs.existsSync(absPath)) {
        fs.readFile(absPath, (err, data) => {
            if (err) {
                send404(response);
            } else {
                send200(response, absPath, data);
            }
        });
    } else {
        send404(response);
    }
};

module.exports = { serverStatic, send200, send404 };

