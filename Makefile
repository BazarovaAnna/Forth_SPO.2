all: build

build: clean forth.o
	ld -o bin/forth bin/forth.o

forth.o: forth.asm bin_dir
	nasm -f elf64 -g -F dwarf forth.asm -o bin/forth.o

bin_dir: 
	mkdir -p bin 

clean:
	rm -rf bin

run: build
	cat branches.forth - | ./forth
