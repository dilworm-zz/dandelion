LUA_A=../../skynet/3rd/lua/liblua.a
INCLUDE_LUA=../../skynet/3rd/lua

all:
	#gcc -g -O2 -Wall -fPIC --shared -I../../skynet/3rd/lua lua-util.c -o util.so 
	clear
	g++ -g -pedantic -std=c++11 -pthread -Wall -fPIC --shared lua-logger.cpp -I../../3rd/spdlog/include -I$(INCLUDE_LUA) -o logger.so

push:
	#cp util.so ../../skynet/luaclib/
	#cp dandelion-logger.so ../../skynet/luaclib/

test:
	#lua test.lua
	#./logger_test
	lua test_logger.lua

clean:
	rm *.so


