"use strict";
// Client-side interactions with the browser.

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
	
	window.setInterval(function() {sendCommand("vol_get")}, 1000);
	window.setInterval(function() {sendCommand("tempo_get")}, 1000);
	window.setInterval(function() {sendCommand("beat_get")}, 1000);
	window.setInterval(function() {sendCommand("uptime")}, 1000);
});

socket.on('connect', function() {	
	$('#error-box').hide();
	sendCommand("vol_get");
	sendCommand("tempo_get");
	sendCommand("beat_get");
});

socket.on('disconnect', function() {	
	$('#error-box').show();
	$('#error-text').text('Error with NodeJS server!');
	setTimeout(function(){
		$('#error-box').hide();
	}, 10000);
});

socket.on('reconnect', function() {	
	$('#error-box').hide();
});

socket.on('local_ok', function() {	
	$('#error-box').hide();
});

socket.on('local-error', function() {
	$('#error-box').show();
	$('#error-text').text('Problem with local app!');
	setTimeout(function(){
		$('#error-box').hide();
	}, 10000);
});

socket.on('vol_reply', function(result) {
	$('#volumeid').val(result);
});

socket.on('tempo_reply', function(result) {
	$('#tempoid').val(result);
});

socket.on('uptime_reply', function(result) {
	$('#status').text(result);
});

socket.on('mode_reply', function(result) {
	$('#modeid').text(result);
});

function sendCommand(message) {
	socket.emit('beat-box', message);
};



