#COMPLIERS
GCC = /usr/bin/gcc
G++ = /usr/bin/g++

#FLAGS
CFLAGS = -O2 -Wall -std=c++17
LDFLAGS = -L./src/libs/libuv/src/unix/ -L./src/libs/libuv/src/win/src/core/ -L./src/libs/libuv/src/win/src -L./src/libs/libuv/src/ -L./src/libs/libuv/src/unix/ -L./src/libs/libuv/src/win/ -L./src/libs/libuv/src/unix/src/ -L./src
LDLIBS = -luv -luv_a
CDEBUG = -g -DDEBUG


.PHONY: all clean utils main test togg

#MAKE UTILS
utils:
	$(G++) $(CFLAGS) -c ./src/utils.cpp -o ./output/utils.o

#MAKE HNSW
hnsw:
	$(G++) $(CFLAGS) -c ./src/hnsw.cpp -o ./output/hnsw.o

#MAKE TOGG
togg:
	$(G++) $(CFLAGS) -c ./src/togg.cpp -o ./output/togg.o

#MAKE MAIN
main: main.o 
	$(G++) ./output/main.o ./output/utils.o ./output/hnsw.o ./output/togg.o -o ./output/main

main.o: ./src/main.cpp utils hnsw togg
	$(G++) $(CFLAGS) -c ./src/main.cpp -o ./output/main.o

debugmain: debugmain.o
	$(G++) ./output/main.o ./output/utils.o ./output/hnsw.o ./output/togg.o -o ./output/main

debugmain.o: ./src/main.cpp utils hnsw togg
	$(G++) $(CFLAGS) -c $(CDEBUG) ./src/main.cpp -o ./output/main.o

#MAKE TEST
test: test.o 
	$(G++) ./output/test.o ./output/utils.o ./output/hnsw.o ./output/togg.o -o ./output/test

test.o: ./src/test.cpp utils hnsw togg
	$(G++) $(CFLAGS) -c $(CDEBUG) ./src/test.cpp -o ./output/test.o	


#ALL
all: utils main test

#CLEAN
clean:
	rm -rf ./output/test.o
	rm -rf ./output/utils.o
	rm -rf ./output/test
	rm -rf ./output/main.o
	rm -rf ./output/main
	rm -rf ./output/togg.o
