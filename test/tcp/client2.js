const {
    console,
} = No.buildin;
const {
    tcp
} = No.libs;
const socket = new tcp.Socket();
socket.connect('127.0.0.1', 8888);

socket.on('data', (data, len) => {
    console.log(`len: ${len}, data: ${Buffer.toString(data)}\n`);
});