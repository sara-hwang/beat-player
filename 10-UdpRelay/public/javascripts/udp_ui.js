"use strict";
// Client-side interactions with the browser.

let nIntervId;

// Make connection to server when web page is fully loaded.
var socket = io.connect();

$(document).ready(function() {
	$('#volumeUp').click(function(){
		sendCommand("vol_up");
	});
	$('#volumeDown').click(function(){
		sendCommand("vol_down");
	});
	$('#tempoUp').click(function(){
		sendCommand("tempo_up");
	});
	$('#tempoDown').click(function(){
		sendCommand("tempo_down");
	});
	$('#queueBass').click(function(){
		sendCommand("bass");
	});
	$('#queueSnare').click(function(){
		sendCommand("snare");
	});
	$('#queueHihat').click(function(){
		sendCommand("hihat");
	});
	$('#modeRock').click(function(){
		sendCommand("rock");
	});
	$('#modeMine').click(function(){
		sendCommand("mine");
	});
	$('#modeNone').click(function(){
		sendCommand("none");
	});
	$('#btnStop').click(function(){
		sendCommand("stop");
	});
});

socket.on('connect', function() {	
	// start_interval();
	sendCommand("vol_get");
	sendCommand("tempo_get");
});

// function start_interval() {
// 	// check if already an interval has been set up
// 	if (!nIntervId) {
// 		nIntervId = setInterval(function(){
// 			sendCommand("vol_get");
// 		}, 1000);
// 	}
// }

socket.on('vol_reply', function(result) {
	$('#volumeid').val(result);
});

socket.on('tempo_reply', function(tempo_result) {
	$('#tempoid').val(tempo_result);
});

function sendCommand(message) {
	socket.emit('beat-box', message);
};



