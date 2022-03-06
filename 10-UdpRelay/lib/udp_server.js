"use strict";
/*
 * Respond to commands over a websocket to relay UDP commands to a local program
 */

var socketio = require('socket.io');
var io;

var dgram = require('dgram');

exports.listen = function(server) {
	io = socketio.listen(server);
	io.set('log level 1');

	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});
};

function handleCommand(socket) {
	// Pased string of comamnd to relay
	socket.on('beat-box', function(data) {
		console.log('command: ' + data);

		// Info for connecting to the local process via UDP
		var PORT = 8088;
		var HOST = '127.0.0.1';
		var buffer = new Buffer(data);

		var client = dgram.createSocket('udp4');
		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
			if (err) 
				throw err;
			console.log('UDP message sent to ' + HOST +':'+ PORT);
		});

		client.on('listening', function () {
			var address = client.address();
			console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		});
		// Handle an incoming message over the UDP from the local application.
		client.on('message', function (message, remote) {
			console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

			var reply = message.toString('utf8')

			if(data.localeCompare('vol_get') == 0 || 
			   data.localeCompare('vol_up') == 0 || 
			   data.localeCompare('vol_down') == 0){
				socket.emit('vol_reply', reply);
			}
			else if(data.localeCompare('tempo_get') == 0 || 
				    data.localeCompare('tempo_up') == 0 || 
					data.localeCompare('tempo_down') == 0){
				socket.emit('tempo_reply', reply);
			}
			else if(data.localeCompare('uptime') == 0){
				socket.emit('uptime_reply', reply);
			}
			
			client.close();

		});
		client.on("primer: stop", function() {
			console.log("closed");
		});
		client.on("UDP Client: error", function(err) {
			console.log("error: ",err);
		});
	});
};