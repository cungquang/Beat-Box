var socket = io.connect();

$(document).ready(function() {

    //Select beat
    select_noneButton();

    select_rock1Button();

    select_rock2Button();


    //Volume
    increase_volume();    

    decrease_volume();
    

    //Tempo
    increase_tempo();
    
    decrease_tempo();


    //Select play drum
    select_hitButton();

    select_snareButton();

    select_baseButton();


    //Terminate button
    select_terminateButton();

})



/*
#####################
#      HELPER       #
#####################
*/

//Select beat
function select_noneButton() {
    $('#none-button').click(function(event) {
        event.preventDefault();

        socket.emit('beat', 'noneButton');
    });
}

function select_rock1Button() {
    $('#rock1-button').click(function(event) {
        event.preventDefault();

        socket.emit('beat', 'rock1Button');
    });
}

function select_rock2Button() {
    $('#rock2-button').click(function(event) {
        event.preventDefault();

        socket.emit('beat', 'rock2Button');
    });
}


//Volume
function increase_volume() {
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

function decrease_volume() {
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


//Tempo
function increase_tempo() {
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

function decrease_tempo() {
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


//Select drum
function select_hitButton() {
    $('#hit-button').click(function(event) {
        event.preventDefault();

        socket.emit('drum', 'hitButton');
    });
}


function select_snareButton() {
    $('#snare-button').click(function(event) {
        event.preventDefault();

        socket.emit('drum', 'snareButton');
    });
}


function select_baseButton() {
    $('#base-button').click(function(event) {
        event.preventDefault();

        socket.emit('drum', 'baseButton');
    });
}


//Select termination
function select_terminateButton() {
    $('#terminate-button').click(function(event) {
        event.preventDefault();

        socket.emit('drum', 'terminateButton');
    });
}