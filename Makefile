all:
	clear && mkdir -p build && cd build && cmake -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ .. && make
clean:
	rm -rf ./build/
