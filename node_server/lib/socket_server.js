const socketio = require('socket.io');
const dgram = require('dgram');

const udpClient = dgram.createSocket('udp4');
const SERVER_ADDRESS = '192.168.7.2';
const SERVER_PORT = 12345;

//Export function listen() -> listen to connection
exports.listen = function(server) {
    // Create web socket listen on server
    const io = new socketio.Server(server);
    
    //On connection 
    io.on('connection', function(socket) {
        handle_beat(socket);
    });
};


/*
#####################
#      HELPER       #
#####################
*/

//UDP Client code to send message:
function sendToUDPServer(message) {
    const buffer = Buffer.from(message);
    udpClient.send(buffer, 0, buffer.length, SERVER_PORT, SERVER_ADDRESS, function(err) {
        if (err) {
        console.error('Error sending message to UDP server:', err);
        } else {
        console.log('Message sent to UDP server:', message);
        }
    });
}


//Handle beat
function handle_beat(socket) {
    //socket.on(eventName, callback)
    //eventName: a string represent the name of the event to listen for
    //callback: a function to be executed when specified event occurs

    socket.on('beat', function(data) {
        message = 'beat';
        sendToUDPServer(message);
    });
}


//Handle volume request from client
function handle_volume(socket) {

    socket.on('volume', function(data) {
        message = 'volume';
        sendToUDPServer(message);
    });
}


//Handle tempo 
function handle_tempo(socket) {
    socket.on('tempo', function(data) {
        message = 'hi volume';
        sendToUDPServer(message);
    });
}


function handle_drum(socket) {

    socket.on('drum', function(data) {
        message = 'drum';
        sendToUDPServer(message);
    });
}


function handle_terminate(socket) {

    socket.on('terminate', function(data) {
        message = 'teriminate';
        sendToUDPServer(message);
    });
}