
all: build run 
	
build:
	@mkdir -p build 
	@fasm hello.asm build/hello

run: 
	@./build/hello

clean:
	@rm -rf build

.PHONY: all, build, run, clean