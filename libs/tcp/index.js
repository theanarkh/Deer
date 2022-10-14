const {
    socket,
    console,
} = No.buildin;
const { constant } = socket;
const { events } = No.libs;
class Server extends events {
    socket = null;
    connections = 0;
    constructor() {
        super();
    }
    listen(options = {}) {
        this.socket = new socket.Socket();  
        this.socket.bind(options.host, options.port);
        return this.socket.listen(512, (socket) => {
            const s = new Socket({socket});
            s.read();
            this.connections++;
            this.emit('connection', s);
        });
    }
}

class Socket extends events {
    constructor(options = {}) {
        super();
        this.socket = options.socket || new socket.Socket();
    }
    read() {
        this.socket.read((bytes, data) => {
            if (bytes > 0) {
                this.emit('data', data, bytes);
            } else {
                this.socket.close(() => {
                    this.emit('close');
                });
                if (bytes == 0) {
                    this.emit('end');
                }
            }
        });
    }
    write(data) {
        if (typeof data === 'string') {
            data = Buffer.from(data);
        }
        const writeRequest = new socket.WriteRequest();
        writeRequest.oncomplete = () => {
            console.log('done\n');
        };
        this.socket.write(data, writeRequest);
    }
    connect(ip, port) {
        const connectRequest = new socket.ConnectRequest();
        connectRequest.oncomplete = () => {
            console.log("connect successfully");
            this.read();
        }
        return this.socket.connect(ip, port, connectRequest);
    }
}

function createServer(...arg) {
    return new Server(...arg);
}

module.exports = {
    createServer,
    Server,
    Socket,
}