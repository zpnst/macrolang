
all: build run 
	
build: clean
	@mkdir -p build 
	@fasm smuclib.asm build/smuclib

clean:
	@rm -rf build

.PHONY: all, build, run, clean