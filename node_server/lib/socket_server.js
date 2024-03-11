const socketio = require('socket.io');
const dgram = require('dgram');

const SERVER_ADDRESS = '192.168.7.2';
const SERVER_PORT = 12345;

//Export function listen() -> listen to connection
exports.listen = function(server) {
    // Create web socket listen on server
    const io = new socketio.Server(server);
    
    // Create a UDP server
    const server = dgram.createSocket('udp4');
    
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



//Handle beat
function handle_beat(socket) {
    //socket.on(eventName, callback)
    //eventName: a string represent the name of the event to listen for
    //callback: a function to be executed when specified event occurs

    socket.on('beat', function(data) {
        message = 'beat';
        // Send message to server
        client.send(message, SERVER_ADDRESS, SERVER_PORT);

        // Listen for response from server
        client.on('message', (msg, rinfo) => {
            socket.emit('beat', msg);
            //client.close();
        });
    });
}


//Handle volume request from client
function handle_volume(socket) {

    socket.on('volume', function(data) {
        message = 'volume';
        // Send message to server
        client.send(message, SERVER_ADDRESS, SERVER_PORT);

        // Listen for response from server
        client.on('message', (msg, rinfo) => {
            socket.emit('beat', msg);
        });
    });
}


//Handle tempo 
function handle_tempo(socket) {
    socket.on('tempo', function(data) {
        message = 'hi volume';
        // Send message to server
        client.send(message, SERVER_ADDRESS, SERVER_PORT);

        // Listen for response from server
        client.on('message', (msg, rinfo) => {
            socket.emit('beat', msg);
        });
    });
}


function handle_drum(socket) {

    socket.on('drum', function(data) {
        message = 'drum';
        // Send message to server
        client.send(message, SERVER_ADDRESS, SERVER_PORT);

        // Listen for response from server
        client.on('message', (msg, rinfo) => {
            socket.emit('beat', msg);
        });
    });
}


function handle_terminate(socket) {

    socket.on('terminate', function(data) {
        message = 'teriminate';
        // Send message to server
        client.send(message, SERVER_ADDRESS, SERVER_PORT);

        // Listen for response from server
        client.on('message', (msg, rinfo) => {
            socket.emit('beat', msg);
        });
    });
}