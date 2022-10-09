const {
    console,
} = No.buildin;
const server = No.libs.tcp.createServer();

const ret = server.listen({host: '127.0.0.1', port: 8888});
console.log(ret);
server.on('connection', (socket) => {
    console.log('connection\n');
    socket.write("hello");
    socket.on('data', (len, data) => {
        console.log(`len: ${len}, data: ${Buffer.toString(data)}`);
    });
});