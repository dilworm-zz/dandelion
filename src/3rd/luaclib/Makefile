all:
	gcc -g -O2 -Wall -fPIC --shared -I../../skynet/3rd/lua lua-util.c -o util.so 

push:
	cp util.so ../../skynet/luaclib/

test:
	lua test.lua
clean:
	rm util.so


