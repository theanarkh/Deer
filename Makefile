run: 
	g++ -g -Iyour_path_for_v8_include Deer.cc core/*.cc -o Deer -lv8_monolith -ldl -Lyour_path_for_v8_lib/ -pthread -std=c++14 -DV8_COMPRESS_POINTERS