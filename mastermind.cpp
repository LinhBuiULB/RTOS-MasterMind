#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include "master.hpp"
#include "player.hpp"

int main(int argc, char **argv) {

	int value, rank, size;
	MPI_Status status;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("I am %d \n", rank);

	int colorNumber = 6;
	int numberSpots = 3;
	std::vector<int> colors;
	std::vector<std::vector<int>> *solutions;

	for (int i = 0; i < colorNumber; i++){
		colors.push_back(i);
	}

	if(rank == 0){
		Master* master = new Master(colorNumber, numberSpots);
		printf("Number of color is %d \n", master->getColorNumber());
		master->printSolution();

		int *solution;

		solution = (int *) malloc(numberSpots * sizeof(int));

		solutions = (std::vector<int> *) malloc(size-1 * sizeof(solution));

		MPI_Gather()
		


		/*
		while(){
			MPI_Recv(&solution);
			master->evaluate(solution);
			MPI_Bcast();
		}
		*/
	}

	else {
		std::vector<int> attributedColors;

		int j = 0;
		for (int i = 0; i < colorNumber; i++){

			if(rank == j+1){
				attributedColors.push_back(i);
			}

			j++;
			if(j > (size-2)){
				j = 0;
			}
		}

		Player* player = new Player(colorNumber, attributedColors, numberSpots);
		player->printAttributedColors();
		player->generateSolutions();
		std::vector<int> nodeSolution = player->getPlausibleSolution(true);

		printf("Node %d sending the solution : ", rank);
		for(std::vector<int>::iterator it = nodeSolution.begin(); it != nodeSolution.end(); it++)
			printf("%d ", *it);
		printf("\n");

	}


	MPI_Finalize();
	return 0;

}