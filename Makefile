run: 
	g++ -g -Ideps/v8/include -Ideps/llhttp/build Deer.cc core/*.cc -o Deer -lv8_monolith -ldl -lllhttp -Llib -pthread -std=c++17 -DV8_COMPRESS_POINTERS -DV8_ENABLE_SANDBOX