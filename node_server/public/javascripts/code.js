var socket = io.connect();

var beat = [false, false, false];

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
            socket.emit('beat', 'noneButton,0');
        } else {
            $('#none-button').removeClass('button-active');
            socket.emit('beat', 'none-button,0');
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

            //Unable other button 
            beat[0] = false;
            $('#none-button').removeClass('button-active');
            beat[2] = false;
            $('#rock2-button').removeClass('button-active');


            //Send message
            socket.emit('beat', 'rock1Button,1');
        } else {
            $('#rock1-button').removeClass('button-active');
            socket.emit('beat', 'rock1Button,0');
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
            beat[1] = false;
            $('#rock1-button').removeClass('button-active');

            //Send message
            socket.emit('beat', 'rock2Button,2');
        } else {
            $('#rock2-button').removeClass('button-active');
            socket.emit('beat', 'rock2Button,0');
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
            //Send data to server
            socket.emit('volume',`up,${currVol}`);

            // Listen for response from server
            socket.on('volume_up', function(response) {
                var parts = response.split(',');
                if (parts.length === 2) {
                    currTempo = parseInt(parts[1]);

                    //Update on page
                    $('#vol-text').val(currVol);
                } 
                else {
                    console.error('Invalid response format:', response);
                }
            });
        }
    });
}

function decrease_volume() {
    $('#subtract-vol').click(function(event) {
        event.preventDefault();
        var currVol = parseInt($('#vol-text').val());
        var minVol = parseInt($('#vol-text').attr('min'));

        //Upadte value
        if(currVol > minVol) {
            //Send data to server
            socket.emit('volume',`down,${currVol}`);

            // Listen for response from server
            socket.on('volume_down', function(response) {
                var parts = response.split(',');
                if (parts.length === 2) {
                    currTempo = parseInt(parts[1]);

                    //Update on page
                    $('#vol-text').val(currVol);
                } 
                else {
                    console.error('Invalid response format:', response);
                }
            });
        }
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
            //Send data to server
            socket.emit('tempo',`up,${currTempo}`);

            // Listen for response from server
            socket.on('tempo_up', function(response) {
                var parts = response.split(',');
                if (parts.length === 2) {
                    currTempo = parseInt(parts[1]);

                    //Update on page
                    $('#tempo-text').val(currTempo);
                } 
                else {
                    console.error('Invalid response format:', response);
                }
            });
        }
    });
}

function decrease_tempo() {
    //substract button click
    $('#subtract-tempo').click(function(event) {
        event.preventDefault(); 
        var currTempo = parseInt($('#tempo-text').val());
        var minTempo = parseInt($('#tempo-text').attr('min')); 

        if(currTempo > minTempo) {
            //Send to server
            socket.emit('tempo',`down,${currTempo}`);

            // Listen for response from server
            socket.on('tempo_down', function(response) {
                var parts = response.split(',');
                if (parts.length === 2) {
                    currTempo = parseInt(parts[1]);
                    
                    //Update on page
                    $('#tempo-text').val(currTempo);
                } 
                else {
                    console.error('Invalid response format:', response);
                }
            });
        }
    });
}


//Select drum
function select_hithatButton() {
    $('#hithat-button').click(function(event) {
        event.preventDefault();

        //Send message each time user click
        socket.emit('drum', 'hit-hat,0');
    });
}


function select_snareButton() {
    $('#snare-button').click(function(event) {
        event.preventDefault();
        
        //Send message each time user click
        socket.emit('drum', 'snare,1');
    });
}


function select_baseButton() {
    $('#base-button').click(function(event) {
        event.preventDefault();

        //Send message each time user click
        socket.emit('drum','base,2');
    });
}


//Select termination
function select_terminateButton() {
    $('#terminate-button').click(function(event) {
        event.preventDefault();

        socket.emit('terminate','terminate,1');
    });
}