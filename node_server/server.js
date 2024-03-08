const http = require('http');
const fs = require('fs');
const mime = require('mime-types');
const path = require('path');
const mathServer = require('./lib/math_server');

// Setup server
const server = http.createServer((request, response) => {
    var filePath = false;
    
    // If accessing root or directory, serve index.html
    if(request.url == '/'){
        filePath = 'public/index.html';
    } else {
        filePath = 'public' + request.url;
    }

    var absPath = './' + filePath;
    serverStatic(response, absPath);
});

mathServer.listen(server);

// Basic configuration
const PORT = 3042;
server.listen(PORT, function() {
    console.log("Server listening on PORT " + PORT);  
});

function send404(response) {
    response.writeHead(404, {'Content-Type':'text/plain'});
    response.write('Error 404: resource not found');
    response.end();
}

function sendFile(response, filePath, fileContents) {
    response.writeHead(200, 
        {"Content-Type": mime.lookup(path.basename(filePath))});
    response.end(fileContents);
}

function serverStatic(response, absPath) {
    fs.exists(absPath, function(exists) {
        if(exists) {
            fs.readFile(absPath, function(err, data) {
                if(err) {
                    send404(response);
                } else{
                    sendFile(response, absPath, data);
                }
            })
        } else {
            send404(response);
        }
    })
};