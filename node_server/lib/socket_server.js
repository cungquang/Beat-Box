const socketio = require('socket.io');
const dgram = require('dgram');

const udpClient = dgram.createSocket('udp4');
const CLIENT_IP = '192.168.7.2';
const CLIENT_PORT = 12345;
udpClient.setMaxListeners(15);

//Export function listen() -> listen to connection
exports.listen = function(server) {
    // Create web socket listen on server
    const io = new socketio.Server(server);
    
    //On connection 
    io.on('connection', function(socket) {
        handle_beat(socket);
        handle_volume(socket);
        handle_tempo(socket);
        handle_drum(socket);
        handle_volume(socket);
        handle_terminate(socket);
    });
};


/*
#####################
#      HELPER       #
#####################
*/

//UDP Client code to send message - Source: follow ChatGPT
function sendToUDPServer(message) {
    return new Promise((resolve, reject) => {
        const buffer = Buffer.from(message);

        // Create a one-time listener
        function onResponse(response) {
            resolve(response.toString());
            // Remove the listener after it's triggered
            udpClient.removeListener('message', onResponse);
        }

        udpClient.once('message', onResponse);

        udpClient.send(buffer, 0, buffer.length, CLIENT_PORT, CLIENT_IP, function(err) {
            if (err) {
                reject(err);
            }
        });
    });
}


//Handle beat
function handle_beat(socket) {
    //socket.on(eventName, callback)
    //eventName: a string represent the name of the event to listen for
    //callback: a function to be executed when specified event occurs

    socket.on('beat', function(data) {
        message = 'beat';
        sendToUDPServer(`${message},${data}`);
    });
}


//Handle volume request from client
function handle_volume(socket) {
    socket.on('volume', async function(data) {
        try {
            const message = 'volume';
            const response = await sendToUDPServer(`${message},${data}`);
            const parts = response.split(',');

            //Send via websocket
            console.log(response);
            socket.emit(`${parts[0]}`, response);
        } catch(error) 
        {
        }
    });
}


//Handle tempo 
function handle_tempo(socket) {
    socket.on('tempo', async function(data) {
        try {
            const message = 'tempo';
            const response = await sendToUDPServer(`${message},${data}`);
            const parts = response.split(',');
            console.log(response);
            //Send via websocket
            socket.emit(`${parts[0]}`, response);
        } catch(error) 
        {
        }
    });
}


function handle_drum(socket) {
    socket.on('drum', function(data) {
        message = 'drum';
        sendToUDPServer(`${message},${data}`);
    });
}


function handle_terminate(socket) {

    socket.on('terminate', function(data) {
        message = 'terminate';
        sendToUDPServer(`${message},${data}`);
    });
}