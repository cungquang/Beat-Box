var socket = io.connect();

var beat = [false, false, false];
var drum = [false, false, false];

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
    select_hithatButton();

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
        beat[0] = !beat[0];
        $('#none-button').toggleClass('button-active');

        if(beat[0]) {
            //Other button must be unpressed
            for(let i = 1; i < beat.length; i++){
                beat[i] = false;
                $(`#rock${i}-button`).removeClass('button-active');
            }

            //none-button get pressed -> send to 
            socket.emit('beat', 'noneButton');
        }
    });
}

function select_rock1Button() {
    $('#rock1-button').click(function(event) {
        event.preventDefault();
        beat[1] = !beat[1];

        //the beat is turned on
        if(beat[1]){
            $('#rock1-button').toggleClass('button-active');

            //Remove 
            beat[0] = false;
            $('#none-button').removeClass('button-active');

            //Send message
            socket.emit('beat', 'rock1Button');
        } else {
            $('#rock1-button').removeClass('button-active');
        }
    });
}

function select_rock2Button() {
    $('#rock2-button').click(function(event) {
        event.preventDefault();
        beat[2] = !beat[2];

        //the beat is turned on
        if(beat[2]){
            $('#rock2-button').toggleClass('button-active');

            //Remove 
            beat[0] = false;
            $('#none-button').removeClass('button-active');

            //Send message
            socket.emit('beat', 'rock1Button');
        } else {
            $('#rock2-button').removeClass('button-active');
        }
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
function select_hithatButton() {
    $('#hithat-button').click(function(event) {
        event.preventDefault();
        drum[0] = !drum[0];
        $('#hithat-button').toggleClass('button-active');
        
        if(drum[0]){
            socket.emit('drum', 'hitButton');
        }
    });
}


function select_snareButton() {
    $('#snare-button').click(function(event) {
        event.preventDefault();
        drum[1] = !drum[1];
        $('#snare-button').toggleClass('button-active');

        if(drum[1]){
            socket.emit('drum', 'snareButton');
        }
    });
}


function select_baseButton() {
    $('#base-button').click(function(event) {
        event.preventDefault();
        drum[2] = !drum[2];
        $('#base-button').toggleClass('button-active');

        if(drum[2]){
            socket.emit('drum', 'baseButton');
        }
    });
}


//Select termination
function select_terminateButton() {
    $('#terminate-button').click(function(event) {
        event.preventDefault();

        socket.emit('drum', 'terminateButton');
    });
}