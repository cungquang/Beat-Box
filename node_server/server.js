const http = require('http');
const dgram = require('dgram');
const fs = require('fs');
const mime = require('mime-types');
const path = require('path');
const socketServer = require('./lib/socket_server');

//HTTP address: http://192.168.6.2:8088
//UDP address: 192:168.6.2:8087
const SERVER_IP = '192.168.7.2';
const HTTP_PORT = 8088;
const UDP_PORT = 8087;


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

// UDP listen on the port

udpServer.on('listening', () => {
    const address = udpServer.address();
    console.log(`UDP server listening on ${address.address}:${address.port}`);
});


// UPD send message

udpServer.on('message', function(message, remote) {
    // Convert buffer to string if necessary
    if (Buffer.isBuffer(message)) {
        message = message.toString('utf8'); // Assuming UTF-8 encoding
    }
    
    //print message
    console.log('original message: ' + message);

    //Split message by deliminiter
    const msgParts = message.split(',');
    socketServer.send(msgParts[0], msgParts[1]);
})

//UDP on error

udpServer.on('error', function(err){
    console.log("ERROR (UDP):\n" + err.stack);
})

//////////////////////// Configure Server ////////////////////////

//Open the socket listen on the same server PORT
socketServer.listen(httpServer);

// Binding on UDP port
udpServer.bind(UDP_PORT, SERVER_IP);

// Configuration
httpServer.listen(HTTP_PORT, function() {
    console.log("Server listening on PORT " + HTTP_PORT);  
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