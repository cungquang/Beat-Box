const socketio = require('socket.io');
const dgram = require('dgram');

const udpClient = dgram.createSocket('udp4');
const CLIENT_IP = '192.168.7.2';
const CLIENT_PORT = 12345;
udpClient.setMaxListeners(25);
var io;             //Declare io as global variable


/*
#####################
#       MAIN        #
#####################
*/

////////////////////// Exported functions //////////////////////

exports.listen = function(server) {
    // Create web socket listen on server
    io = new socketio.Server(server);
    
    //On connection 
    io.on('connection', function(socket) {

        //Operation
        handle_beat(socket);
        handle_volume(socket);
        handle_tempo(socket);
        handle_drum(socket);

        //Terminate
        handle_terminate(socket);

        //Time & error
        handle_timer(socket);
        handle_error(socket);
    });
};

exports.send = function(msgCode, msgContent) {
    if(io) 
    {
        io.emit(msgCode, msgContent);
    }
    //socket was not up
    else{
        console.log('ERROR: socket was not initated.');
    }
}


/*
#####################
#      HELPER       #
#####################
*/

//UDP Client for all
function sendToUDPServer_reg(message) {
    const buffer = Buffer.from(message);

    // Send the message
    udpClient.send(buffer, 0, buffer.length, CLIENT_PORT, CLIENT_IP);
}


//UDP Client - Volume/Tempo - Source: follow ChatGPT
function sendToUDPServer_promise(message) {
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
        sendToUDPServer_reg(`${message},${data}`);
    });
}


//Handle volume
function handle_volume(socket) {
    socket.on('volume', async function(data) {
        try {
            const message = 'volume';
            const response = await sendToUDPServer_promise(`${message},${data}`);
            const parts = response.split(',');

            socket.emit(`${parts[0]}`, parts[1]);
        } catch(error) 
        {
            console.log("ERROR: ", error);
        }
    });
}


//Handle tempo 
function handle_tempo(socket) {
    socket.on('tempo', async function(data) {
        try {
            const message = 'tempo';
            const response = await sendToUDPServer_promise(`${message},${data}`);
            const parts = response.split(',');

            //Send via websocket
            socket.emit(`${parts[0]}`, parts[1]);
        } catch(error) 
        {  
            console.log("ERROR: ", error);
        }
    });
}


//Handle drum
function handle_drum(socket) {
    socket.on('drum', function(data) {
        message = 'drum';
        sendToUDPServer_reg(`${message},${data}`);
    });
}


//Handle terminate
function handle_terminate(socket) {

    socket.on('terminate', function(data) {
        message = 'terminate';
        sendToUDPServer_reg(`${message},${data}`);
    });
}


//Handle timer
function handle_timer(socket) {    
    //Every 1 second -> server will send to the socket => display timer & hide error box
    socket.on("timer", function(data) {
        // send update message
        socket.emit("timer",`${data}`);
    });
}

function handle_error(socket) {
    //Set timer - will be exipore after 5 s => trigger display error box
    var errorTimer = setTimeout(async function() {
        // Set interval to send message every 1 second - source: ChatGPT
        setInterval(() => {
            const message = "show_error,isAlive"; 
            udpServer.send(message, 0, message.length, CLIENT_PORT, SERVER_IP, (err) => {
                if (err) {
                    console.error('Error sending message:', err);
                } else {
                    console.log('Message sent successfully:', message);
                }
            });
        }, 1000);

        
        socket.emit("show_error","show");
    }, 5000);

    //Listen to 
    socket.on("show_error", async function(data) {

        clearTimeout(errorTimer);
        socket.emit("show_error",data);
    });
}