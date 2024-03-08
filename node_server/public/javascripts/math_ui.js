var socket = io.connect();
$(document).ready(function() {
    //Make the text-entry box has focus
    $('#send-command').focus();
    
    //Allow sending the form
    $('#send-form').submit(function() {
        readUserInput();
        return false;
    });

    //Read data come from server
    socket.on('daAnswer', function(result) {
        $('#messages').append(divMessage(result));
    });
})

function readUserInput() {
    var message = $('#send-command').val();

    //Display command in message list
    $('#messages').append(divMessage(message));
    
    //Process the command
    var errMsg = processCommand(message);
    if(errMsg) {
        $('#messages').append(divMessage(errMsg));
    }

    //Clear the command
    $('#send-command').val('')
}

function divMessage(inString) {
    return $('<div></div>').text(inString);
}

function processCommand(command) {
    var words = command.split(' ').filter(word => word !== " ");
    console.log(words);
    var operation = words[1];
    var message = false;

    switch(operation) {
        case '+':
            var request = {
                addend1: Number(words[0]),
                addend2: Number(words[2])
            };
            
            //trigger daAdd function
            socket.emit('daAdd', request);
            break;
        default:
            message = "Unrecognized command";
    }
    return message;
}