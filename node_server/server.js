const http = require('http');
const dgram = require('dgram');
const fs = require('fs');
const mime = require('mime-types');
const path = require('path');
const socketServer = require('./lib/socket_server');

//Host address: http://192.168.6.2:8088/

/*
#####################
#       MAIN        #
#####################
*/


////////////////////// Create a Http server //////////////////////

const httpServer = http.createServer((request, response) => {
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


/////////////////////// Create a UDP server ///////////////////////

const udpServer = dgram.createSocket('udp4');

udpServer.on('message', function(message, remote) {
    
})


//////////////////////// Configure Server ////////////////////////

//Open the socket listen on the same server PORT
socketServer.listen(httpServer);

// Configuration
const PORT = 8088;
httpServer.listen(PORT, function() {
    console.log("Server listening on PORT " + PORT);  
});


/*
#####################
#      HELPER       #
#####################
*/
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