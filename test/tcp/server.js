const {
    console,
} = No.buildin;
const server = No.libs.tcp.createServer();

const ret = server.listen({host: '127.0.0.1', port: 8888});
console.log(ret === 0 ? 'listen successfully\n' : 'listen error');
server.on('connection', (socket) => {
    console.log('connection\n');
    socket.on('data', (data, len) => {
        console.log(`len: ${len}, data: ${Buffer.toString(data)}\n`);
    });
    socket.on('close', (len, data) => {
       console.log('close');
    });
    socket.write("hello");
    socket.write("hello again");
});