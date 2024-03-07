const http = require('http');
const fs = require('fs');
const mime = require('mime-types');

// Basic configuration
const PORT = 3042;

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

// Setup server
const server = http.createServer((request, response) => {
    let filePath;
    if (request.url === "/") {
        filePath = 'public/index.html';
    } else {
        filePath = 'public' + request.url;
    }

    const absPath = './' + filePath;
    serverStatic(response, absPath);
});

server.listen(PORT, () => {
    console.log("Server listening on PORT " + PORT);  
});
