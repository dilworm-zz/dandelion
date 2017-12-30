all:
	#gcc -g -O2 -Wall -fPIC --shared -I../../skynet/3rd/lua lua-util.c -o util.so 
	gcc -g -O2 -Wall -fPIC --shared -I../../skynet/3rd/lua lua-dandelion-logger.c -o dandelion-logger.so 
push:
	#cp util.so ../../skynet/luaclib/
	cp dandelion-logger.so ../../skynet/luaclib/

test:
	#lua test.lua
clean:
	rm *.so


