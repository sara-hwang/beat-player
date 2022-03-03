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
	
	socket.on('commandReply', function(result) {
		var newDiv = $('<div></div>').text(result);
		$('#messages').append(newDiv);
		$('#messages').scrollTop($('#messages').prop('scrollHeight'));
	});
	
});

function sendCommand(message) {
	socket.emit('prime', message);
};