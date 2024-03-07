const http = require('http');
const helper = require('./lib/helper_server');

// Basic configuration
const PORT = 3042;

// Setup server
const server = http.createServer((request, response) => {
    let filePath;
    if (request.url === "/") {
        filePath = 'public/index.html';
    } else {
        filePath = 'public' + request.url;
    }

    const absPath = './' + filePath;
    helper.serverStatic(response, absPath);
});

server.listen(PORT, () => {
    console.log("Server listening on PORT " + PORT);  
});
