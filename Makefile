CFLAGS = -Wall -Werror -std=c99

.PHONY: all
all: mkdir bin/test

.PHONY: mkdir
mkdir:
	mkdir -p build bin

bin/test: build/main.test.o build/even.test.o
	cc $^ -o $@

build/even.test.o: src/even.test.c
	cc $(CFLAGS) -c $^ -o $@

build/main.test.o: src/main.test.c
	cc $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf build bin