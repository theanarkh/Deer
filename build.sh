set -e
cd deps/v8 && tools/dev/v8gen.py x64.release.sample 
../depot_tools/ninja -C out.gn/x64.release.sample v8_monolith
cd ../.. && make && ./Deer test/tcp/server.js
