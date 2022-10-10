mkdir -p deps && cd deps
if [ ! -d "depot_tools" ]; then
    git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
fi
export PATH=$PATH:`pwd`/depot_tools
gclient config https://github.com/v8/v8.git
gclient sync
cd v8 && tools/dev/v8gen.py x64.release.sample 
../depot_tools/ninja -C out.gn/x64.release.sample v8_monolith
cd ../.. && make && ./Deer test/tcp/server.js
