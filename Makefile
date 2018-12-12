all:
	mpiCC player.cpp master.cpp mastermind.cpp -o jeu

test:
	mpirun -np 9 ./jeu