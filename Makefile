
all: build run 
	
build: clean
	@mkdir -p build 
	@fasm src/main.asm build/main

run: 
	@./build/main

clean:
	@rm -rf build

.PHONY: all, build, run, clean