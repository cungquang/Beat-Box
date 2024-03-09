const socketio = require('socket.io');

//Export function listen() -> listen to connection
exports.listen = function(server) {
    const io = new socketio.Server(server);
    
    //On connection 
    io.on('connection', function(socket) {
        handle_volume(socket);
    });
};


/*
#####################
#      HELPER       #
#####################
*/

//Handle beat
function handle_beat(socket) {

}


//Handle volume request from client
function handle_volume(socket) {
    //socket.on(eventName, callback)
    //eventName: a string represent the name of the event to listen for
    //callback: a function to be executed when specified event occurs
    socket.on('volume', function(data) {
        var val1 = Number(data.addend1);
        var val2 = Number(data.addend2);
        console.log('Adding', + val1 + '+' + val2);

        var answer = doDaAddition(val1, val2);
        var message = val1 + '+' + val2 + '=' + answer;

        //build & send solution
        socket.emit('daAnswer', message);
    });
}


//Handle tempo 
function handle_tempo(socket) {

}


function handle_drum(socket) {

}


function handle_terminate(socket) {

}