CXX = g++

all: simulate input/Program/ICache.txt

simulate: simulate.cpp
	$(CXX) simulate.cpp -o simulate

assembler: assembler.cpp
	$(CXX) assembler.cpp -o assembler

input/Program/ICache.txt: assembler program.s
	./assembler
	rm assembler
