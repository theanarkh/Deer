const net = require('net');
const socket = net.connect({port: 8888});
socket.on('connect', () => {
    console.log('connected');
});
socket.on('data', (data) => {
    console.log(data.toString());
    socket.write('world');
});