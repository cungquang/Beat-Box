const SocketIo = require('socket.io');
var iosocket;

const handleCommand = (socket) => {
    //socket.on(eventName, callback)
    //eventName: a string represent the name of the event to listen for
    //callback: a function to be executed when specified event occurs
    socket.on('daAdd', function(data) {
        var val1 = Number(data.addend1);
        var val2 = Number(data.addend2);

        console.log('Adding', + val1 + '+' + val2);

        var answer = doDaAddition(val1, val2);
        var message = val1 + '+' + val2 + '=' + answer;

        //send solution
        socket.emit('daAnswer', message);
    })
}

const doDaAddition = (x, y) => {
    return x + y;
}

exports.listen = function(server) {
    iosocket = SocketIo.listen(server);
    iosocket.sockets.on('connection', function(socket) {
        handleCommand(socket);
    });
};