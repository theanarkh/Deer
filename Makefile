run: 
	g++ -g -Ideps/v8/include Deer.cc core/*.cc -o Deer -lv8_monolith -ldl -Ldeps/v8/out.gn/x64.release.sample/obj -pthread -std=c++17 -DV8_COMPRESS_POINTERS -DV8_ENABLE_SANDBOX