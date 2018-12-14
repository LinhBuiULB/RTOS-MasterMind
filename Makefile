all:
	mpiCC player.cpp master.cpp mastermind.cpp -o game

test:
	mpirun -q -np 8 ./game