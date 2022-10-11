const {
    loader,
    process,
    console,
} = No.buildin;
function loaderNativeModule() {
    const modules = [
        {
            module: 'libs/module/index.js',
            name: 'module'
        },
        {
            module: 'libs/events/index.js',
            name: 'events'
        },
        {
            module: 'libs/process/index.js',
            name: 'process'
        },
        {
            module: 'libs/buffer/index.js',
            name: 'buffer',
            after: (exports) => {
                global.Buffer = exports;
            }
        },
        {
            module: 'libs/tcp/index.js',
            name: 'tcp'
        },
        {
            module: 'libs/http/index.js',
            name: 'http'
        },
    ];
    No.libs = {};
    for (let i = 0; i < modules.length; i++) {
        const module = {
            exports: {},
        };
        loader.compile(modules[i].module).call(null, loader.compile, module.exports, module);
        No.libs[modules[i].name] = module.exports;
        typeof modules[i].after === 'function' && modules[i].after(module.exports);
    }
}

function runMain() {
    No.libs.module.load(process.argv[1]);
}

loaderNativeModule();
runMain();