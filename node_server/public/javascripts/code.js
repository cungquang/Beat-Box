const socketio = require('socket.io');

//Export function listen() -> listen to connection
exports.listen = function(server) {
    const io = new socketio.Server(server);
    
    //On connection 
    io.on('connection', function(socket) {

        //Select beat
        socket.on('beat', () => {

        });

        //Adjust volume
        socket.on('volume', () => {

        })

        //Adjust tempo
        socket.on('tempo', () => {

        });

        //Select play drum
        socket.on('hit-hat', () => {

        });

        socket.on('snare', () => {

        });

        socket.on('base', () => {

        });

        //Terminate button
        socket.on('terminate', () => {

        });
    });
};