CFLAGS = -Wall -Werror -std=c99

.PHONY: all
all: mkdir bin/st

.PHONY: mkdir
mkdir:
	mkdir -p build bin

bin/st: build/main.test.o build/even.test.o build/odd.test.o
	cc $^ -o $@

build/odd.test.o: example/odd.test.c
	cc $(CFLAGS) -c $^ -o $@

build/even.test.o: example/even.test.c
	cc $(CFLAGS) -c $^ -o $@

build/main.test.o: example/main.test.c
	cc $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf build bin
