const {
    console,
    tcp,
    socket,
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
            this.connections++;
            // const serverSocket = new ServerSocket({fd: clientFd});
            this.emit('connection', s);
        });
    }
}

class Socket extends events {
    constructor(options = {}) {
        super();
        this.socket = options.socket;
        this.socket.read((...args) => {
            this.emit('data', ...args);
        });
    }
    write(data) {
        if (typeof data === 'string') {
            data = Buffer.from(data);
        }
        this.socket.write(data);
    }
}

class ClientSocket extends Socket {
    constructor(options = {}) {
        super(options);
        const fd = tcp.socket(constant.domain.AF_INET, constant.type.SOCK_STREAM);
        tcp.bind(fd, '127.0.0.1', 18989);
    }
}

class ServerSocket extends Socket {
    constructor(options = {}) {
        super(options);
        this.fd = options.fd;
        this.read();
    }
    read() {
        const buffer = Buffer.alloc(1024);
        tcp.read(this.fd, buffer, 0, (status) => {
            if (status === 0) {
                this.emit('end');
            } else if (status > 0){
                this.emit('data', buffer);
                this.read();
            } else {
                this.emit('error', new Error('read socket error'));
            }
        })
    }
}

function createServer(...arg) {
    return new Server(...arg);
}

module.exports = {
    createServer,
    Server,
}