const http = require('http');
const helper = require('./lib/helper_server');
const mathServer = require('./lib/math_server');

// Basic configuration
const PORT = 3042;

// Setup server
const server = http.createServer((request, response) => {
    let filePath;

    // If accessing root or directory, serve index.html
    if (request.url === "/" || request.url === "/public/") {
        filePath = 'public/index.html';
    } else {
        filePath = 'public' + request.url;
    }

    const absPath = './' + filePath;
    helper.serverStatic(response, absPath);
});

mathServer.listen(server);

server.listen(PORT, () => {
    console.log("Server listening on PORT " + PORT);  
});


