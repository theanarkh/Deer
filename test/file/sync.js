const {
    fs,
    console,
    util
} = No.buildin;
const fd = fs.openSync('./test/file/1.txt');
console.log(fd);
while(1) {}