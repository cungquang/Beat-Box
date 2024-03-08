var socket = io.connect();

$(document).ready(function() {
    //Select beat


    //Volume
    increaseVolume();    

    decreaseVolume();
    
    //Tempo
    increaseTempo();
    
    decreaseTempo();

    //Select play drum


    //Terminate button

})



/*
#####################
#      HELPER       #
#####################
*/


function increaseVolume() {
    //click add-vol button
    $('#add-vol').click(function(event) {
        event.preventDefault();
        var currVol = parseInt($('#vol-text').val());
        var maxVol = parseInt($('#vol-text').attr('max'));

        //Upadte value
        if(currVol < maxVol) {
            currVol += 1;
            $('#vol-text').val(currVol);
        }

        //Send data to server
        socket.emit('volume', currVol);
    });
}

function decreaseVolume() {
    $('#subtract-vol').click(function(event) {
        event.preventDefault();
        var currVol = parseInt($('#vol-text').val());
        var minVol = parseInt($('#vol-text').attr('min'));

        //Upadte value
        if(currVol > minVol) {
            currVol -= 1
            $('#vol-text').val(currVol);
        }

        //Send data to server
        socket.emit('volume', currVol);
    });
}

function increaseTempo() {
    //click add button
    $('#add-tempo').click(function(event) {
        event.preventDefault(); 
        var currTempo = parseInt($('#tempo-text').val());
        var maxTempo = parseInt($('#tempo-text').attr('max')); 

        //Update 
        if(currTempo < maxTempo) {
            currTempo += 1;
            $('#tempo-text').val(currTempo);
        }
        
        //Send data to server
        socket.emit('tempo', currTempo);
    });
}

function decreaseTempo() {
    //substract button click
    $('#subtract-tempo').click(function(event) {
        event.preventDefault(); 
        var currTempo = parseInt($('#tempo-text').val());
        var maxTempo = parseInt($('#tempo-text').attr('min')); 

        if(currTempo > maxTempo) {
            currTempo -= 1;
            $('#tempo-text').val(currTempo);
        }

        //Send to server
        socket.emit('tempo', currTempo);
    });
}